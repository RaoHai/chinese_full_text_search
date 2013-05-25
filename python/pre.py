#-*- coding:utf-8 -*-
#Preprocessor  of Chinese_word_seg

import sentenceSeg
import amb
import inverted
from time import clock
#_str = input('\>: ')
flist = ['test.txt','text2.txt']
start=clock()
inverted.init()
for item in flist:
    _str = item
    tf = open(_str)
    outfile = open('out.txt','w')

    for line in tf:
        s_arr = sentenceSeg.cut(line)
        for sentence in s_arr:
            if sentence.get_type():
                inverted.save(_str,amb.seg(sentence.get_sentence()))
                #outfile.write(amb.seg(sentence.get_sentence()))

            else:
                inverted.save(_str,sentence.get_sentence())
                #outfile.write(' '+sentence.get_sentence())
finish=clock()
print ('ALL TIME:',(finish-start)/1000000)
outfile.close()