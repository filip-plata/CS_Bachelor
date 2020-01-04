import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect(("jhtc4bsk.jhtc.pl", 40011))

result = []

for _ in range(33):
    s.send("r\np".encode())
    result.append(chr(int(s.recv(5).decode("ascii")[2:].strip(), 16)))

result.reverse()

print("".join(result))
    
s.close()
