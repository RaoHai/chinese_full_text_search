import sys
import socket
import re

p = re.compile('read|write|dump|recvr')
print("Welcome to CWS client....")
print("Connecting to 127.0.0.1:22222....")
address = ('127.0.0.1',12345)
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(address)
print("successed!")
filehandle = open('dict.txt')
count = 0
while True:
#while count<5000:
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
    str = 'write\n' + strlist[0] + '\n'+strlist[1]+'\n'
    #print(str)
    count=count+1
    m = p.match(str)
    while(p.match(str) != None):
        s.send(bytes(str, 'GBK'))
        data = s.recv(512)
        #print(data)
        break
    else:
        print("invalid syntax:"+str)
filehandle.close()
#s.send(bytes('dump\r\n', 'GBK'))
print(count)
s.close()
