#include "trie_tree.h"
CSTL_LIST_INTERFACE(CharList, char *)
CSTL_LIST_IMPLEMENT(CharList, char *)

void CharList_printf(CharList* list){
	CharListIterator pos;
	for(pos = CharList_begin(list);pos!=CharList_end(list);pos = CharList_next(pos)){
		printf("print->%s\n",*CharList_data(pos));
	}

}
char* CharList_out(CharList* list){
	int size = CharList_size(list);
	char * word = (char*)malloc(size*sizeof(char));
	CharListIterator pos;
	for(pos = CharList_begin(list);pos!=CharList_end(list);pos = CharList_next(pos)){
		sprintf(word,"%s%s",word,*CharList_data(pos));
	}
}
unsigned int arg_hash(char *str,int size)
{
	register unsigned int h;
	register unsigned char *p; 

	for(h=0, p = (unsigned char *)str; *p ; p++)
	h = 31 * h + *p; 

	return h % size;
}

//¸ù½Úµã
Trie * create_root_trie(){
	Trie * new_trie = __key_value_init();
	if(new_trie) return new_trie;
}
Trie* root_insert(Trie* trie,char* key,char* value){
	return __key_value_insert(trie,key,value);
}

Trie * create_child_trie(char* key,char* value){
	Trie * new_trie =  (Trie *)sub_allocate(sizeof(Trie));
	memset(new_trie,0,sizeof(Trie));
	new_trie->key =  (char*)malloc(strlen(key));
	strcpy(new_trie->key,key);
	new_trie->value =  (char*)malloc(strlen(value));
	strcpy(new_trie->value,value);
	new_trie->size  = 1;
	return new_trie;
}
void trie_cpy(Trie* Src,Trie* Rec){
	Src->key = (char*)malloc(strlen(Rec->key));
	memset(Src->key,0,strlen(Rec->key));
	Src->value = (char*)malloc(strlen(Rec->value));
	memset(Src->value,0,strlen(Rec->value));

	strcpy(Src->key,Rec->key);
	strcpy(Src->value,Rec->value);
	Src->child = Rec->child;
	Src->size = Rec->size;
}
Trie ** trie_add_child(Trie** parent ,char* key,char* value){
	Trie * elder_trie = *parent;//->child;
	Trie * trie_iter  = elder_trie;
	int elder_size = elder_trie->size;
	int  new_size  = elder_size + 1;
	Trie * new_trie = (Trie*)sub_allocate( new_size * sizeof(Trie));
	Trie * new_child = create_child_trie(key,value);
	int i;
	register unsigned int hashed;
	memset(new_trie,0, new_size * sizeof(Trie));

	for( i = 0; i < elder_size; i++ ){
		hashed = arg_hash(elder_trie[i].key,new_size);
		while( new_trie[hashed].key!=NULL ){
			if(hashed >= new_size) hashed = 0;
			else hashed++;
		}
		trie_cpy( & new_trie[hashed],& elder_trie[i]);
	}
	hashed = arg_hash(key,new_size);
	while( new_trie[hashed].key!=NULL ){
		if(hashed >= new_size) hashed = 0;
		else hashed++;
	}

	trie_cpy( & new_trie[hashed], new_child);
	new_trie->size = new_size;
	
	*parent = new_trie;

	//trie_print(new_trie);
	
	
	return &new_trie;
	
}
char* trie_search(Trie * trie,char* key){
	int size = trie->size;
	int search_count = 0;
	register unsigned int hashed = arg_hash(key,size);
	Trie  iter = trie[hashed];
	while(strcmp(iter.key,key)!=0){
		if(hashed>size) hashed = 0;
		hashed++;
		search_count++;
		iter = trie[hashed];
		if(search_count >= size) return NO_VALUE;
	}
	return iter.value;

}

Trie * get_root_trie_by_key(Trie* trie,char* key){
	return __key_value_get_node(trie,key);
}

Trie * get_child_trie_by_key(Trie * trie,char * key){
	register unsigned int hashed;
	int count  = 0;
	Trie * iter;
	if(!trie) return NULL;
	if(trie->size==0) return NULL;
	hashed = arg_hash(key,trie->size);
	iter = & trie[hashed];
	if(!iter) return NULL;
	while(iter->key!=NULL && strcmp(iter->key,key)!=0){
		//printf("cmp %s -> %s\n",iter->key,key);
		count ++;
		if(count > trie->size) return NULL;
		hashed = hashed + 1;	
		if(hashed >= trie->size) hashed = 0;
		iter = & trie[hashed];
	
	}
	return &trie[hashed];
	
}
Trie* trie_insert(Trie* parent,Trie* child){
	printf("----->trie insert: %s,size:%d\n",parent->key,parent->size);
	if( parent->child==NULL ){
		parent->child = child;
		if(!parent->size){
			parent->size = child->size;
		}
	}
	return parent;
}

void trie_print(Trie * trie){
	int i;
	if(empty(trie)) return;
	if(trie->size==0 && trie->key){
		printf("key:%s,\t value:%s\n",trie->key,trie->value);
	}
	printf("size of Trie: %d\n",trie->size);
	for(i=0;i< trie->size;i++){
		if(trie[i].key){
			printf("key:%s,\t value:%s\n",trie[i].key,trie[i].value);
		}
		if(trie[i].child){
			printf("\t");
			trie_print(trie[i].child);
		}
	}
}
void trie_print_ln(Trie * trie){
	int i;
	printf("trie_print_ln>>\t");
	for(i=0;i<trie->size;i++){
		printf("%s|",trie[i].key);
	}
	printf("  <<\n");
}

char* get_next_word(char* word,int* pos){
	char * retval;
	if(*pos > strlen(word))return NULL;
	if(word[*pos]=='\0')return NULL;
	if(IS_ALPHA(word[*pos])){
		retval = (char*)malloc(sizeof(char));
		sprintf(retval,"%c",word[*pos]);
		(*pos)++;
	}else{
		retval = (char*)malloc(2*sizeof(char));
		sprintf(retval,"%c%c",word[*pos],word[*pos+1]);
		(*pos)+=2;
	}
	//printf("get next word %s",retval);
	return retval;
}
char* trie_get(Trie* trie,char* word){
	int len = strlen(word);
	Trie * root_trie = trie;
	CharList* word_list = CharList_new();

	Trie * child_trie;
	char * word_to_get;
	int pos = 0;
	word_to_get = get_next_word(word,&pos);
	//printf("get word-> %s \n",word_to_get);
	if(word_to_get){
		root_trie = get_root_trie_by_key(trie,word_to_get);
		if(!root_trie){
			return NO_VALUE;
		}
		//list_add(word_list,word_to_get);
		CharList_push_back(word_list,word_to_get);
		child_trie = root_trie->child;

	}
	while(word_to_get){
		
		word_to_get = get_next_word(word,&pos);
		CharList_push_back(word_list,word_to_get);
		child_trie = get_child_trie_by_key(child_trie,word_to_get);
		if(!child_trie){
			CharList_printf(word_list);
			return NO_VALUE;
		}else{
			if(strcmp(child_trie->value,"0")!=0){
				printf("value get %s\n",child_trie->value);
				//list_out(word_list);
			}
			child_trie = child_trie->child;
		}
	}
	;

}

int trie_save(Trie* trie,char* word,char* value){

	int len = strlen(word);
	Trie * root_trie = trie;
	Trie * cur_trie;
	Trie * child_trie;
	Trie * new_trie;
	char * word_to_send;
	int pos=0;
	if(!trie->size)return 0;

	word_to_send = get_next_word(word,&pos);
	
	if(word_to_send){
		root_trie = get_root_trie_by_key(trie,word_to_send);
		if(!root_trie){
			root_trie = root_insert(trie,word_to_send,"0");
		}
		child_trie = root_trie;
		cur_trie = root_trie;
		word_to_send = get_next_word(word,&pos);
		while(word_to_send){
			trie_print_ln(child_trie);
			if(!child_trie->child){
				printf(">> child empty in %s: key:%s\n",child_trie->key,word_to_send);
				new_trie = create_child_trie(word_to_send,"0");
				child_trie= trie_insert(child_trie,new_trie);
				child_trie = child_trie->child;
				
			}
			else if(!get_child_trie_by_key(child_trie->child,word_to_send)){
				printf(">> not has key in %s child:%s\n",child_trie->key, word_to_send);
				trie_add_child(&(child_trie->child),word_to_send,"0");
				child_trie = get_child_trie_by_key(child_trie->child,word_to_send);
				printf(">>NEXT key:%s\n",child_trie->key);

			}
			else{
				printf(">>already has this key%s,in: %s\n",word_to_send,child_trie->key);
				child_trie = get_child_trie_by_key(child_trie->child,word_to_send);

			}
			word_to_send = get_next_word(word,&pos);
			trie_print(cur_trie);
		}
		child_trie->value = value;
	}
	

	
}

int empty(Trie* trie){
	if(!trie || trie == NULL || !trie->key) return 1;
	else return 0;
}
