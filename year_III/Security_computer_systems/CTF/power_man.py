import sys
import socket

def buffered_readline(socket):
    line = ""
    while True:
        part = socket.recv(1)
        if part != "\n":
            line+=part
        elif part == "\n":
            break
    print(line)
    return line


def res(data):
    a = int(data[0].split()[2])
    b = int(data[1].split()[2])
    m = int(data[2].split()[2])

    return pow(a, b, m)

def read_part(sock):
    return [buffered_readline(sock) for i in range(4)]

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("jhtc4bsk.jhtc.pl", 42100))
  [buffered_readline(s) for i in range(2)]
    while True:
        data = read_part(s)
        ans = str(res(data)) + "\n"
        print(ans)
        s.send(ans)



if __name__ == "__main__":
    main()
