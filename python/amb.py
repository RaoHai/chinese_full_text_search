#-*- coding:utf-8 -*-
import json
import copy
import socket
from collections import OrderedDict
def get_json(str):
	_word = str
	global s
	global cache
	retval = ""
	if str in cache:
		retval = cache[str]
	else:
		str = 'read\n'+ str + '\n'
		s.send(bytes(str,'GBK'))
		data = s.recv(512)
		try:
			retval =  data.decode('GBK')
		except:
			print('GET JSON ERROR!:')
			print(data)
		retval  = retval.replace('\r','\\r')
		retval  = retval.replace('\t','')
		retval  = retval.replace('\n','')
		retval  = retval.replace('\'','\"')
		retval  = retval.replace('\u0000',':')
		cache[_word]=retval
	#print(retval)
	if retval == '-NO RESULT' :
		if len(_word) ==1:
			retval = '{"'+_word +'":"1"}'
		else:
			retval = '{"'+_word +'":"0"}'
	try:
		resultobj = json.loads(retval)
	except:
		print("ERROR:",retval)
		return  json.loads('{"'+_word +'":"0"}');
	return resultobj
def copy_item(src):
	desc = WordList(src.getWordRest())
	desc.setList(src.getList())
	return desc
class WordAndWeight:
	def __init__(self,word,weight):
		self.__word = word
		self.__weight = weight
	def getWord(self):
		return self.__word
	def getWeight(self):
		return self.__weight
class WordList:
	def __init__(self,wholeWord):
		self.__wholeWord = wholeWord
		self.__wordRest = wholeWord
		self.__trueWeight = 0
		self.__list = []
	def __str__(self):
		str = ''
		for item in self.__list:
			str = str + ' ' + item.getWord()
		return str
	def getWordRest(self):
		return self.__wordRest
	def setWordRest(self,wordrest):
		self.__wordRest = wordrest
	def addWord(self,word,weight):
		weight = int(weight) / 100000
		self.__list.append(WordAndWeight(word,weight))
		if self.__trueWeight == 0:
			self.__trueWeight = weight
		else:
			self.__trueWeight = self.__trueWeight * weight
	def getTrueWeight(self):
		return self.__trueWeight
	def getList(self):
		return self.__list;
	def setList(self,list):
		self.__list  = copy.deepcopy(list)
class Sentence:
	def __init__(self,sentence):
		self.__fullSentence = sentence
		self.__wordList = []
		self.__wordList.append(WordList(sentence))
	def addWordList(self,word_list):
		self.__wordList.append(word_list)
	def _run(self):
		for item in self.__wordList:
			while len(item.getWordRest())>0:
				wordToGet = item . getWordRest()
				resultobj = get_json( wordToGet)
				od = OrderedDict((sorted( resultobj.items() , key=lambda t: len(t[0]),reverse = True )))
				mlpw = list(od.items())[0]
				mlpw = mlpw[0]
				print('addWord',mlpw)
				item.addWord(mlpw,resultobj[mlpw])
				n = len(mlpw)
				item.setWordRest(item.getWordRest()[n:])


	def run(self):
		for item in self.__wordList:
			while len(item.getWordRest())>0:
				wordToGet = item.getWordRest()
				resultobj = get_json(wordToGet )
				od = OrderedDict((sorted( resultobj.items() , key=lambda t: len(t[0]),reverse = True )))
				mlpw = list(od.items())[0]
				mlpw = mlpw[0]
				
				backupitem = copy_item(item) 
				item.addWord(mlpw,resultobj[mlpw])
				n = len(mlpw)
				item.setWordRest(item.getWordRest()[n:])
				if n != len(wordToGet): 
					for t_item in od:
						if t_item!= mlpw:
							rw = self.__fullSentence[self.__fullSentence.index(t_item) + len(t_item):]
							if len(rw)>0:
								rw_query = get_json(rw)
								orw = OrderedDict((sorted(rw_query.items() , key=lambda t: len(t[0]),reverse = True )))
								orw = list(orw.items())[0]
								#print('t_item:',t_item,'query:',rw,'mlpw',mlpw)
								#print(orw[0],mlpw.find(orw[0]))
								if  not(len(rw_query) == 1 or mlpw.find(orw[0])>0 ):
									newitem = copy_item(backupitem)
									#print('___copy___')
									#print(t_item)
									#print('___copy___')
									newitem.addWord(t_item,resultobj[t_item])
									m = len(t_item)
									newitem.setWordRest(newitem.getWordRest()[m:])
									self.addWordList(newitem)



		maxPosible = 0
		maxPosibleItem = ''
		for titem in self.__wordList:	
			__weight =  titem.getTrueWeight()					
			if __weight > maxPosible:
				maxPosible = __weight
				maxPosibleItem = titem 
		return maxPosibleItem.__str__()
								
								



def seg(word):
	if word == '%':
		return word
	sentence = Sentence(word)
	return sentence.run()

def main():
	address = ('127.0.0.1',12345)
	global s 
	global cache
	cache = {}
	print("connecting to server...")
	s= socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.connect(address)
	#_str = input('>')
	#print(seg(_str))
main()