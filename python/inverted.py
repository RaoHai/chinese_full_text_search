#-*- coding:utf-8 -*-
# inverted table
import amb
import fdict
import operator
class Ivtb:
	def __init__(self):
		self.__table = {}
	def insert(self,fileID,word):
		if word in self.__table:
			self.__table[word].append(fileID)
		else:
			self.__table[word] = [fileID]
	def get(self,word):
		if word in self.__table:
			return self.__table[word]
		else:
			return []
	def _print(self):
		print(self.__table)

def saveIntoIvtb(filename,word):
	global fDict
	global invertedTable
	fileID = fDict.save(filename)
	invertedTable.insert(fileID,word)
	#invertedTable._print()

def init():
	global fDict 
	global invertedTable
	fDict = fdict.FileDict()
	invertedTable = Ivtb()
def save(filename,senten):
	word_arr = senten.split(' ')
	for item in word_arr:
		saveIntoIvtb(filename,item)
def search(word):
	global invertedTable
	global fDict
	print('search:>',word)
	wordarr = amb.seg(word).split(' ');
	result = {}
	resultmark = {}
	retval = ''
	for item in wordarr:
		_list = invertedTable.get(item)
		for index in _list:
			if index in resultmark:
				resultmark[index] = resultmark[index] + len(item)
			else:
				resultmark[index] = len(item)
	sorted_x =  sorted(resultmark.items(), key=lambda x: x[1],reverse = True)
	for item in sorted_x:
		if item[1] > 0:
			filename = fDict.get(item[0])
			percent = item[1] / len(word) * 100
			pair_start = 0;
			pair_end = 0;
			tf = open(filename).read()
			for _word in wordarr:
				start = tf.find(_word)
				end = start + len(_word)
				if pair_start == 0 or pair_start > start:
					pair_start = start
				if pair_end == 0 or pair_end < end:
					pair_end = end
			if pair_start > 5:
				pair_start = pair_start - 5;
			if pair_end < len(tf) - 5:
				pair_end = pair_end + 5
			retval = retval + '<li>'+filename+'[adaptation:%f'%percent + ']\r\r<p>...' + tf[pair_start:pair_end] + '...</p></li>\r\n'
	return retval

#是我国社会主义改革开放



