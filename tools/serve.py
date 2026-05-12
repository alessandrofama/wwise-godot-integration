#!/usr/bin/env python3
"""Simple HTTP server with COOP/COEP headers for Godot Web exports
using SharedArrayBuffer (required by Wwise AudioWorklet + pthreads).

Usage:
    python3 tools/serve.py [--root <dir>] [--port <port>]

Defaults:
    --root  tests/GodotProject/export
    --port  8080
"""

import argparse
import http.server
import os
import sys


class CORPHandler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header("Cache-Control", "no-store")
        super().end_headers()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", default="tests/GodotProject/export")
    parser.add_argument("--port", type=int, default=8080)
    parser.add_argument("--bind", default="127.0.0.1")
    args = parser.parse_args()

    root = os.path.abspath(args.root)
    if not os.path.isdir(root):
        print(f"error: root directory not found: {root}", file=sys.stderr)
        return 1

    os.chdir(root)

    print(f"Serving {root} on http://{args.bind}:{args.port}/")
    print("Press Ctrl+C to stop.")
    server = http.server.ThreadingHTTPServer((args.bind, args.port), CORPHandler)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down.")
    finally:
        server.server_close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
