import sys
import socket
import re
address = ('127.0.0.1',12345)
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(address)
print("successed!")
while True:
    _str = input('\>: ')
    #_str = "read asdasd"
    _strarr = _str.split()
    command = ''
    for item in _strarr:
            #encode = bytes(item,'GBK') 
            command+='%s\n'%item
    print(command)
    s.send(bytes(command,'GBK'))
    #s.send(bytes(_str,'GBK'));
    data = s.recv(512)
    print(data)
    print(data.decode('GBK'))
filehandle.close()
s.close()
