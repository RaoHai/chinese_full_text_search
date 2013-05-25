import re
p = re.compile(r'[\u4e00-\u9fa5]')
class sentence:
	def __init__(self,word,type):
		self.__word = word
		self.__type = type
	def get_word(self):
		return self.__word
	def get_type(self):
		return self.__type
def alpha(word):
	if p.match(word) :
		return True;
	else:
		return False
def Cut(cutlist,lines):  
    l = []  
    line = []  
    last_is_alpha = False
    for i in lines:
    	
    	if (i in cutlist):
    		line_obj = sentence(''.join(line),last_is_alpha);
    		l.append(line_obj)
    		line = []
    	else:
    		if alpha(i) ^ last_is_alpha :
    			line_obj = sentence(''.join(line),last_is_alpha);
    			l.append(line_obj)
    			line = []
    			last_is_alpha =alpha(i)
    			line.append(i);
    		else:
    			line.append(i);
    return l
cutlist = "。，（）().,"#.decode('gbk')  
cutdict = {}
for token in cutlist:
    cutdict[token] = 1
inputfilename = 'test.txt'
t_file = open(inputfilename);
for lines in t_file: 
    print(Cut(cutdict,lines))
