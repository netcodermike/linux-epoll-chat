import socket,sys,threading
with socket.create_connection(('127.0.0.1',int(sys.argv[1])))as s:
    def send():
        try:
            for line in sys.stdin:s.sendall(line.encode())
            s.shutdown(socket.SHUT_WR)
        except OSError:pass
    threading.Thread(target=send,daemon=True).start()
    while True:
        data=s.recv(4096)
        if not data:break
        print(data.decode(errors='replace'),end='',flush=True)
