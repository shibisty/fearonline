import socket
import threading

tcp_sock = None

def tcp_client(conn, addr):
    print(f"[TCP CONNECT] {addr}")

    conn.settimeout(10)

    try:
        while True:
            try:
                data = conn.recv(4096)

                if not data:
                    print(f"[TCP CLOSED] {addr}")
                    break

                print(f"[TCP RX] {addr}")
                print("HEX :", data.hex(" "))
                print("LEN :", len(data))
                
                conn.sendall(data)
                print("[TCP TX] echo")

            except socket.timeout:
                continue

    except Exception as e:
        print("TCP CLIENT ERROR:", e)

    finally:
        conn.close()
        print(f"[TCP DISCONNECT] {addr}")


def tcp_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server.bind(("0.0.0.0", 30003))
    server.listen(16)

    print("[TCP] Listening on 0.0.0.0:30003")

    while True:
        try:
            conn, addr = server.accept()

            print(f"[TCP ACCEPT] {addr}")

            threading.Thread(
                target=tcp_client,
                args=(conn, addr),
                daemon=True
            ).start()

        except KeyboardInterrupt:
            print("Stopping...")
            break

        except Exception as e:
            print("TCP ACCEPT ERROR:", e)

    server.close()

def udp_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server.bind(("0.0.0.0", 30003))

    print("[UDP ] Listening on 0.0.0.0:30003")

    while True:
        try:
            data, addr = server.recvfrom(65535)

            print(f"[UDP RX] {addr}")
            print("HEX :", data.hex(" "))
            print("LEN :", len(data))

            server.sendto(data, addr)
            print("[UDP TX] echo")

        except KeyboardInterrupt:
            break

        except Exception as e:
            print("UDP ERROR:", e)

    server.close()

threads = [
    threading.Thread(target=tcp_server, daemon=True),
    threading.Thread(target=udp_server, daemon=True),
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
    if tcp_sock:
        tcp_sock.close()
    print("Stopped.")