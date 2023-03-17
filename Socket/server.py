import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #tạo đối tượng socket, AF_INET là họ địa chỉ Internet cho IPv4. SOCK_STREAM là giao thức cho TCP.

sock.bind(("192.168.0.112", 22221))
print("Connecting ...")
sock.listen(1) #cho phép máy chủ chấp nhận kết nối TCP
print("Connected ...")
while 1:
    conn, addr = sock.accept() #máy chủ đợi đến khi có các request tiếp theo, trả về socket mới được tạo ra
    res=conn.recv(1024).decode() #đọc kiểu 1024 bytes từ socket
    if res.find("client"):
        s=res.replace("client", "server")
    elif res.find("Client"):
        s=res.replace("Client", "Server")
    else:
        s=res
    conn.send(s.encode())
    if not res: #nếu k nhập vào gì thì dừng
        break
conn.close() #đóng kết nối tới client này (nhưng không cđóng socket)