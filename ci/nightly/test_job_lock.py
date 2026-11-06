import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

module_path = Path(__file__).with_name("job-lock.py")
spec = importlib.util.spec_from_file_location("job_lock", module_path)
job_lock = importlib.util.module_from_spec(spec)
spec.loader.exec_module(job_lock)


class JobLockTest(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.lock_path = Path(self.directory.name) / "jobs.lock"

    def process(self, arguments):
        process = subprocess.Popen(arguments, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        self.addCleanup(self.stop, process)
        return process

    @staticmethod
    def stop(process):
        if process.poll() is None:
            process.kill()
        process.wait(timeout=5)

    def worker(self):
        return self.process([sys.executable, "-c", "import time; time.sleep(60)"])

    def waiter(self, worker):
        code = "import runpy,sys; from pathlib import Path; module=runpy.run_path(sys.argv[1]); module['acquire_lock'](int(sys.argv[2]), Path(sys.argv[3]))"
        return self.process([sys.executable, "-c", code, str(module_path), str(worker.pid), str(self.lock_path)])

    def test_second_job_waits_until_worker_exits(self):
        first = self.worker()
        holder = job_lock.acquire_lock(first.pid, self.lock_path)
        self.addCleanup(self.stop, holder)
        second = self.worker()
        waiter = self.waiter(second)
        with self.assertRaises(subprocess.TimeoutExpired):
            waiter.wait(timeout=0.5)
        self.stop(first)
        self.assertEqual(waiter.wait(timeout=5), 0)

    def test_killed_holder_does_not_release_a_live_job(self):
        first = self.worker()
        holder = job_lock.acquire_lock(first.pid, self.lock_path)
        self.stop(holder)
        second = self.worker()
        waiter = self.waiter(second)
        with self.assertRaises(subprocess.TimeoutExpired):
            waiter.wait(timeout=0.5)
        self.stop(first)
        self.assertEqual(waiter.wait(timeout=5), 0)

    def test_cancelled_waiter_never_acquires(self):
        first = self.worker()
        holder = job_lock.acquire_lock(first.pid, self.lock_path)
        self.addCleanup(self.stop, holder)
        second = self.worker()
        waiter = self.waiter(second)
        with self.assertRaises(subprocess.TimeoutExpired):
            waiter.wait(timeout=0.5)
        self.stop(second)
        self.assertNotEqual(waiter.wait(timeout=5), 0)

    def test_corrupt_owner_record_fails_closed(self):
        self.lock_path.write_text("corrupt")
        worker = self.worker()
        with self.assertRaises(RuntimeError):
            job_lock.acquire_lock(worker.pid, self.lock_path)

    def fake_runner(self, use_hook):
        code = "import subprocess,sys; "
        if use_hook:
            code += "subprocess.run(['bash',sys.argv[1]],check=True); "
        code += "subprocess.run(['python3',sys.argv[2],'verify'],check=True)"
        environment = dict(os.environ, HOME=self.directory.name)
        return subprocess.run(
            ["Runner.Worker", "-c", code, str(module_path.with_suffix(".sh")), str(module_path)],
            executable=sys.executable, env=environment,
            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=5,
        ).returncode

    def test_shell_hook_discovers_worker_and_verifies_ownership(self):
        self.assertEqual(self.fake_runner(True), 0)

    def test_verification_rejects_missing_hook(self):
        self.assertNotEqual(self.fake_runner(False), 0)

    def test_verification_rejects_wrong_owner(self):
        cache = Path(self.directory.name) / ".cache"
        cache.mkdir()
        (cache / "nightly-jobs.lock").write_text(json.dumps({"pid": 1, "start": "0"}))
        self.assertNotEqual(self.fake_runner(False), 0)


if __name__ == "__main__":
    unittest.main()
