import subprocess,socket
p=subprocess.Popen(['./app','0'],stdout=subprocess.PIPE,text=True)
try:
    port=int(p.stdout.readline().strip().rsplit(':',1)[1])
    with socket.create_connection(('127.0.0.1',port),timeout=3)as a,socket.create_connection(('127.0.0.1',port),timeout=3)as b:
        import time;time.sleep(.1);a.sendall(b'hello\n');assert a.recv(100)==b'hello\n';assert b.recv(100)==b'hello\n';b.sendall(b'world\n');assert a.recv(100)==b'world\n'
finally:p.terminate();p.wait(timeout=3)
print('Loopback broadcast and signal cleanup passed')
