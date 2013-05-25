import sys
import socket
import re
import threading 

def test(i,j):
    address = ('127.0.0.1',12345)
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    s.connect(address)
    command = '*3\r\n$5\r\nwrite\r\n$5\r\nhello\r\n$5\r\nworld\r\n'
    s.send(bytes(command,'gbk'))
    data = s.recv(512)
    print(data.decode('gbk'))
    s.close()

thread_pool = [] 
for i in range(1000):
    th = threading.Thread(target=test,args=(i,i+1) ) ; 
    thread_pool.append(th) 

for i in range(1000): 
    thread_pool[i].start() 