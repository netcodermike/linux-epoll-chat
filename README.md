# Linux Epoll Chat

Run a small loopback chat broadcast server using Linux epoll and a terminal client.

[![Build and verify](https://github.com/netcodermike/linux-epoll-chat/actions/workflows/verify.yml/badge.svg)](https://github.com/netcodermike/linux-epoll-chat/actions/workflows/verify.yml)

**Language:** C · **Platform:** Linux · **App type:** Linux event-driven terminal chat server/client

First-release learning implementation, with explicit scope below. This is one independently runnable app, not a placeholder in a combined repository.

## Features

- Up to 64 clients
- Binary stream broadcast
- Loopback-only TCP
- Signal shutdown

## Requirements

Linux, a C11 compiler, make, and Python 3 for real integration tests.

## Quick start

```sh
git clone https://github.com/netcodermike/linux-epoll-chat.git
cd linux-epoll-chat
make
./app 4040
# Other terminals:
python3 client.py 4040
```

Commands run from the repository root. Lines marked for a particular operating system apply only there. Language tools are prerequisites, not installed by the app. Close desktop windows normally; terminal/server apps stop with Ctrl+C unless their usage notes say otherwise.

## Example workflow

Run ./app 4040, then python3 client.py 4040 in two terminals. Type a message and confirm it appears on both connections.

## Architecture, configuration, and data

main.c accepts a numeric port (0 chooses an available port), binds 127.0.0.1, watches clients with epoll, and broadcasts each received block including to its sender. Slow/partial-send peers are disconnected rather than queued. client.py provides a Python stdin/socket bridge. Integration tests connect two clients and verify broadcasts/cleanup.

## Verification

Build with the quick-start command where needed, then run:

```sh
make test
```

Manual acceptance: Run ./app 4040, then python3 client.py 4040 in two terminals. Type a message and confirm it appears on both connections. Also test malformed inputs and an unwritable output destination.

See [validation record](docs/VALIDATION.md) for observed checks and their limits. CI results verify only what the workflow runs; they do not establish native GUI correctness or production readiness.

## Limitations and next steps

Educational byte-stream chat, without message framing, names, authentication, TLS, history, remote binding, queued backpressure, or production DoS protection. UTF-8 block boundaries can display replacement characters in the simple client. Slow peers can lose data and are disconnected.

## Troubleshooting

Use the exact platform/toolchain above. Preserve originals and use synthetic fixtures first. Errors are displayed in the UI or stderr. A headless machine can run the model tests but cannot verify native GUI/audio behavior. Output paths are chosen explicitly; keep backups of important data.

## Source map

- `client.py`
- `main.c`
- `Makefile`
- `tests/integration.py`

## Development and license

Read [contributing](CONTRIBUTING.md) before changing the app, and [security/privacy](SECURITY.md) before supplying private data. Use synthetic fixtures for tests. No telemetry, paid API key, or account is required by the app itself. Dependencies, when present, are stated in Requirements.

[MIT license](LICENSE), copyright 2026 netcodermike.
