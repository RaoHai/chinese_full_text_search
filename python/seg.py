#-*- coding:utf-8 -*-
import json
import socket

class Questlist:

	def __init__(self,word):
		self.__word = word


def main():
	address = ('127.0.0.1',12345)
	s= socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.connect(address)
	print("successed!")
	while True:
		sentence  = input('\>:')