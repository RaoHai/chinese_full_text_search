def is_zh (c):
        x = ord (c)
        if c.isdigit():
        	return False
        # Punct & Radicals
        if x >= 0x2e80 and x <= 0x33ff:
                return True

        # Fullwidth Latin Characters
        elif x >= 0xff00 and x <= 0xffef:
                return True

        # CJK Unified Ideographs &
        # CJK Unified Ideographs Extension A
        elif x >= 0x4e00 and x <= 0x9fbb:
                return True
        # CJK Compatibility Ideographs
        elif x >= 0xf900 and x <= 0xfad9:
                return True

        # CJK Unified Ideographs Extension B
        elif x >= 0x20000 and x <= 0x2a6d6:
                return True

        # CJK Compatibility Supplement
        elif x >= 0x2f800 and x <= 0x2fa1d:
                return True

        else:
                return False
class sentence_lang:
	def __init__(self,sentence,_type):
		self.__sentence = sentence
		self.__type = _type
	def get_sentence(self):
		return self.__sentence
	def get_type(self):
		return self.__type
def cut_line(line,s_dict):
	current_line = []
	cuted_lines = []
	last_is_zh = is_zh(line[0])
	count = 0;
	for letter in line:
		if count > 25:
			s = sentence_lang(''.join(current_line),last_is_zh)
			cuted_lines.append(s)
			current_line.append(letter)
			count = 0
			current_line=[]
		if letter in s_dict:
			s = sentence_lang(''.join(current_line),last_is_zh)
			cuted_lines.append(s)
			last_is_zh = is_zh(letter)
			current_line=[]
			count = 0
		else:
			if last_is_zh ^ is_zh(letter):
				if len(current_line)>0:
					s = sentence_lang(''.join(current_line),last_is_zh)
					cuted_lines.append(s)
				last_is_zh = is_zh(letter)
				current_line=[]
				count = 0
				current_line.append(letter)
			else:
				current_line.append(letter)
		count +=1
	return cuted_lines
cutlist = '。，“”‘’；：【】~！@#￥％%…&*（）、《》／？℃－'
cutdict = {}
for x in cutlist:
	cutdict[x] = 1
def cut(line):
	return cut_line(line,cutdict)
