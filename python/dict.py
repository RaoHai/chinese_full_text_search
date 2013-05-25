import sys
import socket
import re
from time import clock

p = re.compile('read|write|dump|recvr')
print("Welcome to CWS client......")
print("connecting to 127.0.0.1:22222")
address = ('127.0.0.1',12345)
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(address)
print("successed!")
filehandle = open('Mandarin1.fre','r', encoding='UTF-8')
count = 0
start=clock()
while True:
    rdline = filehandle.readline()
    if(rdline==' '):
        break
    strlist = rdline.split()
    if(len(strlist)<2):
        break
    str = 'write ' + strlist[1] + ' '+strlist[0]
    count = count + 1
    m=p.match(str)
    while (p.match(str) != None):
        #_strarr = str.split()
        #command = '*%d\r\n'%len(_strarr)
        #for item in _strarr:
         #   encode  = bytes(item,'utf-8')
          #  command+='$%d\r\n%s\r\n'%(len(encode),item)
        #print(command)
        s.send(bytes(str,'GBK'))
        data = s.recv(512)
        #print(data)
        break
    else:
        print("invalid syntax:"+str)
finish=clock()
print((finish-start)/1000000)
s.close()


