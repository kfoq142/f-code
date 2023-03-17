import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("192.168.0.112", 22221))
num=input("Client >> ")
sock.send(num.encode())
rp=sock.recv(1024)
print("Server <<", rp.decode())
sock.close()