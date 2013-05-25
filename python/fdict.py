#-*- coding:utf-8 -*-
# FILE DICT

class FileDict:
	def __init__(self):
		self.__force_dict = {}
		self.__last_dict = {}
	def _print(self):
		print(self.__force_dict)
		print(self.__last_dict)
	def save(self,filename):
		n = len(self.__force_dict)
		if filename in self.__force_dict:
			return self.__force_dict[filename]
		self.__force_dict[filename] = n
		self.__last_dict[len(self.__last_dict)] = filename
		return n+1
	def get(self,Id):
		if Id in self.__last_dict:
			return self.__last_dict[Id]



def main():
	fdict = FileDict()
	fdict.save("1.txt")
	fdict.save("2.txt")
	fdict.save("3.txt")
	fdict.save("4.txt")
	fdict.save("5.txt")
	fdict._print()
#main()