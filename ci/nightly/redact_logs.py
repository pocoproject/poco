from pathlib import Path
import socket
import sys


def redact(contents):
    return contents.replace(str(Path.home()), "$HOME").replace(socket.gethostname(), "[runner]")


if __name__ == "__main__":
    if len(sys.argv) == 1:
        for line in sys.stdin:
            print(redact(line), end="", flush=True)
    else:
        source, destination = map(Path, sys.argv[1:])
        destination.mkdir(parents=True, exist_ok=True)
        for source_file in source.rglob("*"):
            if source_file.is_file() and not source_file.is_symlink():
                target = destination / source_file.relative_to(source)
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_text(redact(source_file.read_text(errors="replace")))
