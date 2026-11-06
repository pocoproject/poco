import fcntl
import json
import os
from pathlib import Path
import select
import subprocess
import sys


def process_identity(process_id):
    try:
        fields = Path(f"/proc/{process_id}/stat").read_text().rsplit(")", 1)[1].split()
        if fields[0] == "Z":
            return None
        return fields[19]
    except FileNotFoundError:
        return None


def find_worker():
    process_id = os.getppid()
    while process_id > 1:
        arguments = Path(f"/proc/{process_id}/cmdline").read_bytes().split(b"\0")
        if Path(os.fsdecode(arguments[0])).name == "Runner.Worker":
            return process_id
        fields = Path(f"/proc/{process_id}/stat").read_text().rsplit(")", 1)[1].split()
        process_id = int(fields[1])
    raise RuntimeError("Job lock must run as a runner job-start hook")


def hold_lock(process_id, signal_fd, lock_path):
    identity = process_identity(process_id)
    if identity is None:
        raise RuntimeError("Worker exited before acquiring the job lock")
    worker_fd = os.pidfd_open(process_id)
    if process_identity(process_id) != identity:
        raise RuntimeError("Worker exited before acquiring the job lock")
    lock_path.parent.mkdir(parents=True, exist_ok=True)
    with lock_path.open("a+") as lock:
        while True:
            if select.select([worker_fd], [], [], 0)[0]:
                raise RuntimeError("Worker exited while waiting for the job lock")
            try:
                fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
                break
            except BlockingIOError:
                select.select([worker_fd], [], [], 0.2)
        lock.seek(0)
        previous = lock.read()
        if previous:
            owner = json.loads(previous)
            if process_identity(owner["pid"]) == owner["start"]:
                owner_fd = os.pidfd_open(owner["pid"])
                while process_identity(owner["pid"]) == owner["start"]:
                    if select.select([worker_fd], [], [], 0)[0]:
                        raise RuntimeError("Worker exited while waiting for the previous owner")
                    select.select([owner_fd], [], [], 0.2)
                os.close(owner_fd)
        if process_identity(process_id) != identity:
            raise RuntimeError("Worker exited before the job lock became available")
        lock.seek(0)
        lock.truncate()
        json.dump({"pid": process_id, "start": identity}, lock)
        lock.flush()
        os.fsync(lock.fileno())
        os.write(signal_fd, b"1")
        os.close(signal_fd)
        select.select([worker_fd], [], [])
    os.close(worker_fd)


def acquire_lock(process_id, lock_path):
    read_fd, write_fd = os.pipe()
    environment = {name: value for name, value in os.environ.items() if name != "RUNNER_TRACKING_ID"}
    holder = subprocess.Popen(
        [sys.executable, __file__, "hold", str(process_id), str(write_fd), str(lock_path)],
        pass_fds=(write_fd,), start_new_session=True, env=environment,
        stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
    )
    os.close(write_fd)
    try:
        ready = os.read(read_fd, 1)
    finally:
        os.close(read_fd)
    if ready != b"1":
        holder.wait()
        raise RuntimeError("Could not acquire the host job lock; refusing to start the job")
    return holder


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "hold":
        hold_lock(int(sys.argv[2]), int(sys.argv[3]), Path(sys.argv[4]))
    elif len(sys.argv) > 1 and sys.argv[1] == "verify":
        worker = find_worker()
        owner = json.loads((Path.home() / ".cache" / "nightly-jobs.lock").read_text())
        if owner != {"pid": worker, "start": process_identity(worker)}:
            raise RuntimeError("Runner job-start hook did not acquire the host job lock")
    else:
        acquire_lock(find_worker(), Path.home() / ".cache" / "nightly-jobs.lock")
        print("Host job lock acquired")
