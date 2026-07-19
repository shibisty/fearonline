import os
import ssl
import socket
import mimetypes
import threading
from urllib.parse import urlparse, unquote, parse_qs
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

ROOT = os.path.abspath("public")

class Handler(BaseHTTPRequestHandler):

    protocol_version = "HTTP/1.1"

    def _print_request(self, body=b""):
        print("=" * 70)
        print(f"{self.command} {self.path}")
        print(f"Client: {self.client_address[0]}:{self.client_address[1]}")

        print("\nHeaders:")
        for k, v in self.headers.items():
            print(f"{k}: {v}")

        if body:
            print("\nBody:")
            try:
                print(body.decode(errors="replace"))
            except:
                print(body)

            print("\nHEX:")
            print(body.hex())

        print("=" * 70)

    def do_GET(self):
        try:
            self._print_request()

            parsed = urlparse(self.path)
            path = unquote(parsed.path)

            if path == "/":
                path = "/index.html"

            if parsed.path == "/content_only_launcher":
                qs = parse_qs(parsed.query)

                code = qs.get("code", [""])[0]
                state = qs.get("state", [""])[0]

                print("\nFINAL CALLBACK")
                print("code :", code)
                print("state:", state)

                body = b"LOGIN OK"

                self.send_response(200)
                self.send_header("Content-Type", "text/plain")
                self.send_header("Content-Length", str(len(body)))
                self.end_headers()
                self.wfile.write(body)
                return

            if parsed.path == "/fogame/notice":
                fallback = os.path.join(
                    ROOT,
                    "fogame",
                    "notice",
                    "index.html"
                )

                if os.path.isfile(fallback):
                    with open(fallback, "rb") as f:
                        data = f.read()

                    self.send_response(200)
                    self.send_header("Content-Type", "text/html")
                    self.send_header("Content-Length", str(len(data)))
                    self.end_headers()
                    self.wfile.write(data)
                else:
                    self.send_error(404)
                return

            if parsed.path == "/dialog/oauth/authorize":
                fallback = os.path.join(
                    ROOT,
                    "dialog",
                    "oauth",
                    "authorize",
                    "index.html"
                )

                if os.path.isfile(fallback):
                    with open(fallback, "rb") as f:
                        data = f.read()

                    self.send_response(200)
                    self.send_header("Content-Type", "text/html")
                    self.send_header("Content-Length", str(len(data)))
                    self.end_headers()
                    self.wfile.write(data)
                else:
                    self.send_error(404)
                return

            filename = os.path.abspath(
                os.path.join(ROOT, path.lstrip("/"))
            )

            if not filename.startswith(ROOT):
                self.send_error(403)
                return

            if os.path.isfile(filename):

                ctype = (
                    mimetypes.guess_type(filename)[0]
                    or "application/octet-stream"
                )

                with open(filename, "rb") as f:
                    data = f.read()

                self.send_response(200)
                self.send_header("Content-Type", ctype)
                self.send_header("Content-Length", str(len(data)))
                self.end_headers()
                self.wfile.write(data)

            else:

                fallback = os.path.join(
                    ROOT,
                    "dialog",
                    "oauth",
                    "index.html"
                )

                if os.path.isfile(fallback):
                    with open(fallback, "rb") as f:
                        data = f.read()

                    self.send_response(200)
                    self.send_header("Content-Type", "text/html")
                    self.send_header("Content-Length", str(len(data)))
                    self.end_headers()
                    self.wfile.write(data)
                else:
                    self.send_error(404)

        except Exception as e:
            print("GET ERROR:", e)

    def do_POST(self):
        try:
            length = int(self.headers.get("Content-Length", 0))
            body = self.rfile.read(length)

            self._print_request(body)

            parsed = urlparse(self.path)
            path = unquote(parsed.path)
            
            if parsed.path == "/dialog/oauth/authorize":
                fallback = os.path.join(
                    ROOT,
                    "dialog",
                    "oauth",
                    "authorize",
                    "index.html"
                )

                if os.path.isfile(fallback):
                    with open(fallback, "rb") as f:
                        data = f.read()

                    self.send_response(200)
                    self.send_header("Content-Type", "text/html")
                    self.send_header("Content-Length", str(len(data)))
                    self.end_headers()
                    self.wfile.write(data)
                else:
                    self.send_error(404)
                return
            else:
                reply = b"OK"

                self.send_response(200)
                self.send_header("Content-Type", "text/plain")
                self.send_header("Content-Length", str(len(reply)))
                self.end_headers()
                self.wfile.write(reply)

        except Exception as e:
            print("POST ERROR:", e)

    def log_message(self, format, *args):
        pass


http_srv = None
https_srv = None
tcp_sock = None


def http_server():
    global http_srv

    http_srv = ThreadingHTTPServer(("0.0.0.0", 80), Handler)
    http_srv.daemon_threads = True
    http_srv.timeout = 1

    print("[HTTP ] Listening on :80")

    try:
        http_srv.serve_forever(poll_interval=0.5)
    finally:
        http_srv.server_close()


def https_server():
    global https_srv

    https_srv = ThreadingHTTPServer(("0.0.0.0", 443), Handler)
    https_srv.daemon_threads = True
    https_srv.timeout = 1

    ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ctx.load_cert_chain(
        certfile="certs/cert.crt",
        keyfile="certs/cert.key"
    )

    https_srv.socket = ctx.wrap_socket(
        https_srv.socket,
        server_side=True
    )

    print("[HTTPS] Listening on :443")

    try:
        https_srv.serve_forever(poll_interval=0.5)
    finally:
        https_srv.server_close()


def tcp_client(conn, addr):
    conn.settimeout(30)

    print("\n[TCP CONNECT]", addr)

    try:
        while True:
            try:
                data = conn.recv(4096)
            except socket.timeout:
                break

            if not data:
                break

            print("\n[TCP RX]", addr)
            print("HEX:", data.hex())

            try:
                print("TEXT:", data.decode(errors="replace"))
            except:
                pass

            conn.sendall(b"OK")

    except Exception as e:
        print("TCP ERROR:", e)

    finally:
        conn.close()
        print("[TCP CLOSED]", addr)


def tcp_server():
    global tcp_sock

    tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    tcp_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    tcp_sock.bind(("0.0.0.0", 30003))
    tcp_sock.listen()

    print("[TCP  ] Listening on :30003")

    while True:
        try:
            conn, addr = tcp_sock.accept()

            threading.Thread(
                target=tcp_client,
                args=(conn, addr),
                daemon=True
            ).start()

        except OSError:
            break
        except Exception as e:
            print("TCP ACCEPT ERROR:", e)

    tcp_sock.close()


threads = [
    threading.Thread(target=http_server, daemon=True),
    threading.Thread(target=https_server, daemon=True),
    threading.Thread(target=tcp_server, daemon=True),
]

for t in threads:
    t.start()

print("Servers started.")

try:
    while any(t.is_alive() for t in threads):
        for t in threads:
            t.join(timeout=0.5)

except KeyboardInterrupt:
    print("\nStopping servers...")
    if http_srv:
        http_srv.shutdown()
    if https_srv:
        https_srv.shutdown()
    if tcp_sock:
        tcp_sock.close()
    print("Stopped.")