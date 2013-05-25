import sys
import socket
import re

p = re.compile('read|write|dump|recvr')
print("Welcome to CWS client....")
print("Connecting to 127.0.0.1:22222....")
address = ('127.0.0.1',22222)
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(address)
print("successed!")
filehandle = open('Mandarin.fre')
count = 0
while True:
    rdline = filehandle.readline()
    if(rdline==''):
        break;
    strlist = rdline.split()
    #print(strlist[1])
    #str = input('\>: ')
    if(len(strlist)<2):
        print(count)
        print(rdline)
        continue
    str = 'write ' + strlist[1] + ' '+strlist[0]
    #print(str)
    count=count+1
    m = p.match(str)
    while(p.match(str) != None):
        s.send(bytes(str+'\r\n', 'GBK'))
        data = s.recv(512)
        print(data)
        break
    else:
        print("invalid syntax:"+str)
filehandle.close()
#s.send(bytes('dump\r\n', 'GBK'))
print(count)
s.close()
