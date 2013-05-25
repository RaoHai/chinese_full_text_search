#-*- coding:utf-8 -*-
#word seg 
import copy
import json
import socket
from collections import OrderedDict


def get_json(str):
	#print("get json:"+str)
	global s
	global cache
	retval = ""
	if str in cache:
		print('read from cache'+str)
		retval = cache[str]
	else:
		str = 'read\n'+ str + '\n'
		s.send(bytes(str,'GBK'))
		data = s.recv(512)
		try:
			retval =  data.decode('GBK')
		except:
			print('ERROR!:')
			print(data)
		retval  = retval.replace('\r','\\r')
		retval  = retval.replace('\t','')
		retval  = retval.replace('\n','')
		retval  = retval.replace('\'','\"')
		retval  = retval.replace('\u0000',':')
		cache[str]=retval
	#print(retval)
	return retval
def copy_item(src):
	desc = word_list(src.get_word_rest())
	desc.set_list(src.get_list())
	return desc
class word_and_weight:
	def __init__(self,word,weight):
		self.__word = word
		self.__weight = int(weight) / 100000 #* len(word)
	def get_word(self):
		return self.__word
	def get_weight(self):
		return self.__weight
class word_list:
	def __init__(self,whole_word):
		self.__list = []
		self.__weighted = 0
		self.__word_rest = whole_word;
	def __str__(self):
		retval = ''
		for item in self.__list:
			retval+=">>key:"+item.get_word()+",value:"+"%f"%item.get_weight()+"<<\n"
		return retval
	def get_word_rest(self):
		return self.__word_rest
	def set_word_rest(self,word):
		self.__word_rest = word
	def set_list(self,list):
		self.__list = copy.deepcopy(list)
	def get_list(self):
		return self.__list
	def add_word(self,word,weight):
		self.__list.append(word_and_weight(word,weight)) 
	
	def print_list(self):
		print("print_list:")
		for item in self.__list:
			print(">>"+item.get_word()+":"+"%d"%int(item.get_weight()))
		print("print_list end");
	
	def get_weighted_value(self):
		value = 1;
		for item in self.__list:
			#weight  = ():
			#
			value = value* item.get_weight()
		self.__weighted = value;
		return value
class posible_node:
	#__max_weight = 0
	def __init__(self,word,weight):
		self.__pre_node = {}
		self.__word = word
		self.__weight = weight
		self.__most_weight = 1
		self.__most_posible_word = ""
		self.__counted = 0
		#self.__true_weight = weight;
		#print('create node:'+word+","+weight)
	def __str__(self):
		retval = ''
		for node in self.__pre_node:
			retval +=">>"+node+":%f"%self.__pre_node[node]+"\n"
		return retval
	def set_counted(self,weight):
		self.__counted = 1
		self.__most_weight =weight 
	def get_counted(self):
		return self.__counted;
	def get_most(self):
		return self.__most_posible_word
	def get_most_weight(self):
		return self.__most_weight
	def set_most(self,word,weight):
		#print("set most of:"+self.__word+":"+word+":%f"%weight)
		self.__most_posible_word = word
		self.__most_weight = weight
	def get_word(self):
		return self.__word
	def get_weight(self):
		return self.__weight
	def add_pre_node(self,node):
		#print(self.__word+":pre:"+node.get_word()+"%f"%node.get_weight()+"* %f"%self.__weight)
		self.__pre_node[node.get_word()] =node.get_weight()#*self.__weight
		#if(node.get_weight()>self.__max_weight):
		#	self.__max_weight = node.get_weight()
		#	self.__true_weight = self.__weight * self.max_weight

		#print(self.__pre_node)
	def get_pre_node(self):
		return self.__pre_node

class Posible_list:
	def __init__(self,whole_word):
		print("Init:"+whole_word)
		self.__word_list = []
		self.__whole_word = whole_word
		self.__word_list.append(word_list(whole_word))
		self.__refer_count = {}
		self.__count = 0
		self.p_map = OrderedDict()
		self.__weighted_list = {}
		print("succeed")
	def __str__(self):
		retval = '>>printing posible_list:\n'
		for item in self.__word_list:
			retval+=item.__str__()+'\n'
		retval +='<<printing end\n'
		return retval
	def init_refer_count(self,key):
		if key in self.__refer_count:
			return
		else:
			self.__refer_count[key]=0
	def add_refer_count(self,key):
		if key in self.__refer_count:
			self.__refer_count[key]+=1
		else:
			self.__refer_count[key]=1
	def get_refer_count(self):
		return self.__refer_count
	def add_word_list(self,word_list):
		self.__word_list.append(word_list)
	def get_word_weight(self):
		for item in self.__word_list:
			print("%E"%get_weighted_value(item));

	def combine_single_word(self,word_list):
		list = word_list.get_list();
		print('combine:')
		for item in list:
			print(item.get_word())
		#print(word_list)
	def create_map(self):
		for item in self.p_map:
			self.get_true_weight(item,1)
			#print("after:"+item)
			#print(self.__weighted_list)






	def get_true_weight(self,node,refer_count):
		if node in self.__weighted_list:
			return self.__weighted_list[node] # *(0.9**refer_count)
		else:
			nodes = self.p_map[node]
			pre_node = nodes.get_pre_node()
			#print(">> GET "+node+":%E"%nodes.get_weight())
			#print(pre_node)
			#print("<<END")
			if len(pre_node)==0:
				_weight = nodes.get_weight()
				self.__weighted_list[node] = _weight 
				return _weight #*(0.9**refer_count)
			else:
				_max_value = 0
				_max_node = ""
				for pre in pre_node:
					if pre == node:
						break
					_t_weight = self.get_true_weight(pre,refer_count+1)
					if _t_weight > _max_value:
						_max_value = _t_weight
						_max_node = pre

				self.__weighted_list[node] =_max_value * nodes.get_weight()
				nodes.set_most(_max_node, _max_value* nodes.get_weight())
				return _max_value* nodes.get_weight()# *(0.9**refer_count)

	def _get_count(self,node):

		self.__count +=1
		nodes = self.p_map[node]
		if nodes.get_counted ==1:
			return nodes.get_most_weight();
		pre_node = nodes.get_pre_node()
		if len(pre_node) == 0:
			#in-degree equals 0 means it's the start word
			self.p_map[node].set_counted(nodes.get_weight())
			return nodes.get_weight()
		else:
			print(node,pre_node)
			max_posible = 0;
			max_posible_word = "";
			for pre in pre_node:
					_weight = self.get_count(pre)* nodes.get_weight()
					pre_node[pre]  = _weight
				#print("posible word "+pre+"of"+node+"is %E"% _weight )
					if  _weight>max_posible:
			
						max_posible = _weight
						max_posible_word = pre
			#self.p_map[node].set_counted(max_posible)
			#print("max posible word of "+node+" is "+max_posible_word+" :%E"%max_posible)
			return max_posible
					



	def run(self):
		for item in self.__word_list:
			#result = quest(item.__word_rest)
			#imagine that returned json was 
			# {
			# 	ruan jian gong cheng: 120,
			# 	ruan jian:200, 
			# }

			while len(item.get_word_rest())>0:
				word_to_get = item.get_word_rest()
				result = get_json(word_to_get )# {'ruan jian gong cheng':120,'ruan jian':200}
				if result == '-NO RESULT' :
					if len(word_to_get) ==1:
						result = '{"'+word_to_get +'":"1"}'
					else:
						result = '{"'+word_to_get +'":"0"}'
				#print("request:"+item.get_word_rest())
				#resultjson = json.dumps(result)
				#print(resultjson)
				#print(result)
				try:
					resultobj = json.loads(result)
				except:
					print("ERROR:")
					print(esult)
					return [];
				key = list(resultobj.keys())
				#if  len(key)>1:#more than one key
				backupitem = copy_item(item)
				item.add_word(key[0],resultobj[key[0]])
				n = len(key[0])
				item.set_word_rest(item.get_word_rest()[n:])
				if  len(key)>1:
					for  i in range(1,len(key)):
						#print(i)
						newitem = copy_item(backupitem)
						newitem.add_word(key[i],resultobj[key[i]])
						m = len(key[i])
						newitem.set_word_rest(newitem.get_word_rest()[m:])
						self.add_word_list(newitem)
		
		#max_weight = 0
		#max_it = ""
		#for item in self.__word_list:
		#	print(item)
		#	cur_weight = item.get_weighted_value()
		#	if cur_weight>max_weight:
		#		max_weight = cur_weight
		#		max_it = item
		#return max_it

		max_weight = 0
		max_item =""

		for item in self.__word_list:
			_w = item.get_weighted_value()
			if _w>max_weight:
				max_weight = _w
				max_item = item
		#print('>>>>>>>>>>>>>>>>')
		#print(max_item)
		for item in  self.__word_list:
			t_list = item.get_list()
			for i in range(0 , len(t_list)):
				word = t_list[i].get_word()
				weight = t_list[i].get_weight()

				word = '%d'%i+'_'+word;
				self.init_refer_count(word)
				if word in self.p_map:
					#print('select:'+word)
					p_node = self.p_map[word]
				else:
					p_node = posible_node(word,weight)
				if i>0:
					pre_word = '%d'%(i-1)+'_'+t_list[i-1].get_word()
					pre_weight =  t_list[i-1].get_weight()
					pre_node = posible_node(pre_word, pre_weight)
					p_node.add_pre_node(pre_node)
					
					self.add_refer_count(pre_word)
				#print(p_node)
				self.p_map[word] = p_node
		# p_map is the linked table for posible map..

		#for node in self.p_map:
			#print(node,self.p_map[node].get_pre_node())
			
			#nodes = p_map[node]
		#	self.get_count(node)
		#print(self.p_map)
		self.create_map()
		#print('pre end')
		#get weight of map and get tail
		refer = self.__refer_count
		
		max_tail = dict((key,value) for key, value in refer.items() if value == 0)

		max_tail_posible = {};
		
		max_weight = 0
		max_tail_item = 0
		for item in max_tail:
			_weight = self.p_map[item].get_most_weight()
			if _weight > max_weight:
				max_weight = _weight
				max_tail_item = item

		#print("MAX ITEM::"+max_tail_item)
		ret_list = []
		node = max_tail_item
		ret_list.append(node)
		while True:
			node = self.p_map[node].get_most()
			if len(node)==0:
				break
			ret_list.append(node)			
		return ret_list
			#last_weight = p_map[node].get_most_weight()
			#max_weight = 0;
			#t_pre_node = p_map[node].get_pre_node()
			#for t_node in t_pre_node:
			#	print("last_weight:%d"%last_weight)
			#	
			#	if float(t_pre_node[t_node]) * last_weight > max_weight:
			#		max_weight =float(t_pre_node[t_node]) * last_weight
			#		p_map[node].set_most(t_node,max_weight)	
			#for word in t_list:
			#	if word.get_word() in p_map:
			#		print('in')
			#	else:
			#		p_node = posible_node(word.get_word(),word.get_weight())
					#p_map[ word.get_word() ] = posible_node(word.get_word(),word.get_weight())
			#if item.get_weighted_value() > max_weight:
			#	max_weight = item.get_weighted_value()
			#	max_item = item
		#for node in p_map:
		#	print(node)
			
		#return max_item




#w_list = word_list()
#e.g. full sentence is "ruan jian gong cheng shi"
#first quest return "ruan jian" and "ruan jian gong cheng"
#so the list should be below:
print('this module imported')
def run():
	address = ('127.0.0.1',12345)
	global s 
	global cache 
	cache= {}
	print("connecting to server...")
	s= socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.connect(address)
	print("Word Seg init successed!")
run()
def seg(sentence):
	if len(sentence)==0: 
		return ''
	t_list = Posible_list(sentence)
	seg = t_list.run()
	retval = ""
	for word in seg:#.get_list():
		retval = word[word.find('_')+1:] + " "+retval
	return retval
#print(t_list)
#w_list.add_word("hello",123)
#w_list.add_word("world",456)
#w_list.print_list()