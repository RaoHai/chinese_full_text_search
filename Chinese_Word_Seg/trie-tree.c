#include "trie_tree.h"
#include "cJSON.h"
#include <iconv.h>
/*
	4e1695f46240670976f89047
	90fd662f4e45522b91cd9022
*/
CSTL_LIST_INTERFACE(CharList, char *)
CSTL_LIST_IMPLEMENT(CharList, char *)

CSTL_MAP_INTERFACE(StrMap,const char *, const char*)
CSTL_MAP_IMPLEMENT(StrMap,const char *, const char*, strcmp)

CSTL_MAP_INTERFACE(BackMap,char *, char*)
CSTL_MAP_IMPLEMENT(BackMap,char *, char*, strcmp)

//struct BackMap*  backup_map;
tjson* backup_map;
void backup_save(char* key,char* value){
    /*char*save_key = sub_allocate(strlen(key));
    memset(save_key,0,strlen(key));
    strcpy(save_key,key);
    char*save_value = sub_allocate(strlen(value));
    memset(save_value,0,strlen(value));
    strcpy(save_value,value);
    //printf("BackMap_insert:%s,%s\n",save_key,save_value);
    BackMap_insert(backup_map,save_key,save_value,NULL);
    */
    backup_map = json_add(backup_map,key,value);

}
void CharList_printf(CharList* list)
{
        CharListIterator pos;
        for(pos = CharList_begin(list); pos!=CharList_end(list); pos = CharList_next(pos)) {
                printf("print->%s\n",*CharList_data(pos));
        }

}
char* CharList_out(CharList* list)
{
        int size = CharList_size(list) + 1;
        int bufferlen = 2*size*sizeof(char);
        char * word = (char*)malloc(bufferlen);
        CharListIterator pos;
        memset(word,0,bufferlen);
        for(pos = CharList_begin(list); pos!=CharList_end(list); pos = CharList_next(pos)) {
                sprintf(word,"%s%s",word,*CharList_data(pos));

        }
        //printf("char list out:%s\n",word);
        return word;
}

unsigned int arg_hash(char *str,int size)
{
        register unsigned int h;
        register unsigned char *p;

        for(h=0, p = (unsigned char *)str; *p ; p++)
                h = 31 * h + *p;

        return h % size;
}

//根节点
Trie * create_root_trie()
{
        Trie * new_trie = __key_value_init();
        //backup_map =BackMap_new();
        backup_map = json_create();
        if(new_trie) return new_trie;
        return NULL;
}
Trie* root_insert(Trie* trie,char* key,char* value)
{
        return __key_value_insert(trie,key,value);
}

Trie * create_child_trie(char* key,char* value)
{
        k_log("creating child trie:%s,%s\n",key,value);
        int size_key = strlen(key);
        int size_value = strlen(value);
        Trie * new_trie =  (Trie *)malloc(sizeof(Trie));
        memset(new_trie,0,sizeof(Trie));
        new_trie->key =  (char*)malloc(size_key+1);
        memset(new_trie->key,'\0',size_key+1);
        strcpy(new_trie->key,key);
        //strcpy_s(new_trie->key,key);
        new_trie->value =  (char*)malloc( size_value+1);
        memset(new_trie->value,'\0',size_value+1);
        strcpy(new_trie->value,value);
        new_trie->size  = 1;
        //printf("create new child_trie:key:%s,value:%s\n",new_trie->key,new_trie->value);
        return new_trie;
}
void trie_cpy(Trie* Src,Trie* Rec)
{
        k_log("----------------\n[%p]Key:%s,Value:%s::SRC[%p]\n----------------\n",Rec,Rec->key,Rec->value,Src);
        //if(Src->key) free(Src->key);
        Src->key = (char*)malloc(strlen(Rec->key)+1);
        memset(Src->key,'\0',strlen(Rec->key)+1);
        //if(Src->value) free(Src->value);
        Src->value = (char*)malloc(strlen(Rec->value)+1);
        memset(Src->value,'\0',strlen(Rec->value));

        strcpy(Src->key,Rec->key);
        strcpy(Src->value,Rec->value);
        Src->child = Rec->child;
        Src->size = Rec->size;

}
Trie * trie_add_child(Trie* parent ,char* key,char* value)
{
                //Trie * elder_trie = parent->child;

                int elder_size = parent->child->size;
                int  new_size  = elder_size + 1;
                Trie * new_trie = (Trie*)malloc( new_size * sizeof(Trie));
                Trie * new_child = create_child_trie(key,value);
                int i;
                register unsigned int hashed;
                memset(new_trie,0, new_size * sizeof(Trie));
                new_trie->size = new_size;
                k_log("$$$-> dump afert new\n");
                trie_print_ln(new_trie);
                k_log("$$$-> END\n");
                for( i = 0; i < elder_size; i++ ) {
                        hashed = arg_hash(parent->child[i].key,new_size);
                        while( new_trie[hashed].key!=NULL ) {
                                if(hashed >= new_size-1) hashed = 0;
                                else hashed++;
                        }
                        trie_cpy( & new_trie[hashed],& parent->child[i]);
                }
                hashed = arg_hash(key,new_size);
                k_log("%% HASH GET: %d %%\n",hashed);
                while( new_trie[hashed].key!=NULL ) {
                        if(hashed >= new_size-1) hashed = 0;
                        else hashed++;
                         k_log("%% HASH GET: %d %%\n",hashed);
                }

                k_log("before copy(%d):%s,%s>>\n",hashed,new_child->key,new_child->value);
                //trie_print_ln(new_child);
                k_log("hashed:%d,address:[%p]\n",hashed,&new_trie[hashed]);
                trie_cpy(&new_trie[hashed], new_child);
                k_log("after copy>>>\n");
                trie_print_ln(new_trie);
                new_trie->size = new_size;



                //parent->size = new_size;
                //parent->child = new_trie;
                return new_trie;





}
char* trie_search(Trie * trie,char* key)
{
        int size = trie->size;
        int search_count = 0;
        register unsigned int hashed = arg_hash(key,size);
        Trie  iter = trie[hashed];
        while(strcmp(iter.key,key)!=0) {
                if(hashed>size) hashed = 0;
                hashed++;
                search_count++;
                iter = trie[hashed];
                if(search_count >= size) return NO_VALUE;
        }
        return iter.value;

}

Trie * get_root_trie_by_key(Trie* trie,char* key)
{
        return __key_value_get_node(trie,key);
}

Trie * get_child_trie_by_key(Trie * trie,char * key)
{

        register unsigned int hashed;
        int count  = 0;
        if(!trie) return NULL;

        if(!trie->size || trie->size==0) return NULL;
        if(!key)return NULL;
        hashed = arg_hash(key,trie->size);
        //iter = & trie[hashed];
        //if(!iter) return NULL;
        //k_log(" get_child_trie_by_key:\n");
        while(trie[hashed].key!=NULL && strcmp(trie[hashed].key,key)!=0) {
                //k_log("cmp trie[%d]%s -> %s\n",hashed,trie[hashed].key,key);
                count ++;
                if(count > trie->size) return NULL;

                hashed = hashed + 1;
                if(hashed >= trie->size ) hashed = 0;
                //iter = & trie[hashed];
        }
        return & trie[hashed];

}
Trie* trie_insert(Trie* parent,Trie* child)
{
    int count = 0;
        //printf("----->trie insert: %s,size:%d\n",parent->key,parent->size);
        while(! parent->lock){
            count++;
            parent->lock = 1;
            if( parent->child==NULL ) {

                    parent->child = child;
                    if(!parent->size) {
                            parent->size = child->size;
                    }
            }
            parent->lock = 0;
            break;
        }
        if(count>1){
            printf("get lock...%d\n",count);
        }
         return parent;

}

void trie_print_file(Trie* trie,FILE* fp){

}
void trie_print(Trie * trie)
{
        int i;
        if(empty(trie)) return;
        if(trie->size==0 && trie->key) {
                printf("key:%s,\t value:%s\n",trie->key,trie->value);
        }
        printf("size of Trie: %d\n",trie->size);
        for(i=0; i< trie->size; i++) {
                if(trie[i].key) {
                        printf("key:%s,\t value:%s\n",trie[i].key,trie[i].value);
                }
                if(trie[i].child) {
                        printf("\t");
                        trie_print(trie[i].child);
                }
        }
}
void trie_print_ln(Trie * trie)
{
    if(!trie)return;
        int i;

        for(i=0; i<trie->size; i++) {
            if(trie[i].key && trie[i].value){
                    k_log("[%p]{%s(%d)|%s}",&trie[i],trie[i].key,i,trie[i].value);
                }
                //if(trie[i].child) trie_print_ln(trie[i].child);
        }
        k_log("  <<\n");
}

char* get_next_word(char* word,int* pos)
{
        char * retval;
        if(*pos > strlen(word))return NULL;
        if(word[*pos]=='\0')return NULL;
        if(IS_ALPHA(word[*pos])) {
                retval = (char*)malloc(sizeof(char)+1);
                sprintf(retval,"%c\0",word[*pos]);
                (*pos)++;
        } else {
                retval = (char*)malloc(2*sizeof(char)+1);
                sprintf(retval,"%c%c\0",word[*pos],word[*pos+1]);
                (*pos)+=2;
        }

        //printf("get next word %s",retval);
        return retval;
}
struct StrMap* trie_get(Trie* trie,char* word) {
//	int len = strlen(word);
       //k_log(printf("trie_get:%s.",word));
        //FILE *fp = fopen("out.txt","a");
        //k_log("trie_get:%s\n", word);

        Trie * root_trie = trie;
        CharList* word_list = CharList_new();
        StrMap *map = StrMap_new();
        Trie * child_trie;
        Trie * cur_trie;
        char * word_to_get;
        int pos = 0;
        word_to_get = get_next_word(word,&pos);

        if(word_to_get) {
                root_trie = get_root_trie_by_key(trie,word_to_get);
                if(!root_trie) {
                        return NULL;
                }

                //list_add(word_list,word_to_get);
                CharList_push_back(word_list,word_to_get);
                if(root_trie->value && strcmp(root_trie->value,"0")!=0) {
                                StrMap_insert(map,CharList_out(word_list),root_trie->value,NULL);
                }
                child_trie = root_trie->child;
                //printf("%s",word_to_get);

        }
        while(word_to_get) {
                //printf(".");
                word_to_get = get_next_word(word,&pos);
                if(child_trie){
                    k_log("get_child_trie_by_key:key:%s,%s\n",child_trie->key,word_to_get);
                    child_trie = get_child_trie_by_key(child_trie,word_to_get);
                }
                if(!child_trie) {

                          k_log("-----NULL CHILD----\n");

                        //   printf("successed\n");
                        return map;
                } else {
                    k_log("-----CUR CHILD----\n");
                    k_log("%s\n", word_to_get);
                    k_log("key:%s\n",child_trie->key);
                    k_log("value:%s\n",child_trie->value);
                    k_log("size:%d\n",child_trie->size);
                    CharList_push_back(word_list,word_to_get);
                    if(child_trie->value && strcmp(child_trie->value,"0")!=0) {
                            k_log("value get %s,%s\n",CharList_out(word_list),child_trie->value);
                            StrMap_insert(map,CharList_out(word_list),child_trie->value,NULL);
                            //list_out(word_list);
                    }
                        //if(child_trie == child_trie->child)return map;;
                         //printf("pointer of child_trie:%p,child:%p\n",&child_trie,&(child_trie->child));
                         k_log("child size:%d\n",child_trie->size);
                         child_trie = child_trie->child;

                }
        }
        //printf("\n");

        printf("successed\n");
        return map;

}

int trie_save(Trie* trie,char* word,char* value)
{

        tp = fopen("out.txt","a");
        k_log("------------------\ntrie save:%s:%s\n",word,value);

        Trie * root_trie = trie;
        Trie * cur_trie;
        Trie * child_trie;
        Trie * new_trie;
        char * word_to_send;
        if(!trie->size)return 0;
        int pos=0;
        //return 0;
        backup_save(word,value);
        word_to_send = get_next_word(word,&pos);

        if(word_to_send) {
                    root_trie = get_root_trie_by_key(trie,word_to_send);

                    if(!root_trie) {
                            root_trie = root_insert(trie,word_to_send,"0");
                    }

                    child_trie = root_trie;
                    cur_trie = root_trie;
                    word_to_send = get_next_word(word,&pos);
                    while(word_to_send) {
                            k_log("-->\n");
                            trie_print_ln(child_trie);
                            trie_print_ln(child_trie->child);
                            k_log("<--\n");
                            if(!child_trie->child) {
                                    //Have no children, create a new one
                                    new_trie = create_child_trie(word_to_send,"0");
                                    child_trie= trie_insert(child_trie,new_trie);
                                    k_log("have no children:key:%s,value:%s\n",child_trie->key,child_trie->value);
                                    trie_print_ln(child_trie->child);
                                    child_trie = child_trie->child;

                            } else if(!get_child_trie_by_key(child_trie->child,word_to_send)) {
                                    //cant't find this letter
                                    //lock....
                                    k_log("can't find this leter :key:%s,value:%s\n",child_trie->key,child_trie->value);
                                    child_trie->child = trie_add_child(child_trie,word_to_send,"0");
                                    trie_print_ln(child_trie->child);
                                    //child_trie->size =child_trie->child->size;
                                    k_log("set child of child_trie:%s(%s) as:",child_trie->key,child_trie->value);
                                    trie_print_ln(child_trie->child);
                                    trie_print_ln(child_trie);
                                    child_trie = get_child_trie_by_key(child_trie->child,word_to_send);

                                   //k_log(">>after::size:%d\n",child_trie->size);

                                    //child_trie = get_child_trie_by_key(child_trie->child,word_to_send);



                            } else {
                                    child_trie = get_child_trie_by_key(child_trie->child,word_to_send);
                                    k_log("founded this leter%s :key:%s,value:%s\n",word_to_send,child_trie->key,child_trie->value);
                            }
                              if(!child_trie) fclose(tp);
                              free(word_to_send);
                            word_to_send = get_next_word(word,&pos);
                            //trie_print(cur_trie);
                    }

                child_trie->value = (char*)malloc(strlen(value)*sizeof(char)+1);
                memset(child_trie->value,'\0',strlen(value)*sizeof(char)+1);
                strcpy(child_trie->value,value);
                k_log("save succeed:key:%s,value:%s\n",child_trie->key,value);

                //StrMap_insert(backup,word,value,NULL);
        }

        return 1;


}

int empty(Trie* trie)
{
        if(!trie || trie == NULL || !trie->key) return 1;
        else return 0;
}

char* map_to_str(void* map){
    map = ( StrMap *)map;
    StrMapIterator pos;
    cJSON *json = cJSON_CreateObject();
    char * result;
    if(map) {
        for(pos=StrMap_begin(map); pos!=StrMap_end(map); pos = StrMap_next(pos)) {
            cJSON_AddStringToObject(json,*StrMap_key(pos),*StrMap_value(pos));
        }
        result = cJSON_Print(json);
        free(map);
        free(json);
        return result;
    } else {
        free(json);
        result = "-NO RESULT";
        return result;
    }
}
void* command(char* command){

        char* word,*tmp,*result;
        StrMap *map;
        StrMapIterator pos;
        cJSON *json;
        if(!command) return NULL;
      //  printf("COMMADN CALL\n");
        //printf(">>>>%s\nCOMMAND:%s\n",split_by_n(&command,"\n"),command);
        //printf("<<<END\n");
        char** lines =cSplit(command,'\n');
        if(strcmp(lines[0],"read")==0){
            printf(">READ:\n");
            map = trie_get(root,lines[1]);
            json = cJSON_CreateObject();
                if(map) {
                    for(pos=StrMap_begin(map); pos!=StrMap_end(map); pos = StrMap_next(pos)) {
                    cJSON_AddStringToObject(json,*StrMap_key(pos),*StrMap_value(pos));
                    }
                    result = cJSON_Print(json);

                    free(map);
                    free(json);

            } else {

                    free(json);
                    result = "-NO RESULT";
                }
        }if(strcmp(lines[0],"write")==0){

                 trie_save(root,lines[1],lines[2]);
                 //backup_save(key,value);
                //printf("write:%s:%s\n",key,value);
                result = "+";

        }

        //freeStringsMemory(lines);
        return result;

}
void* command_parser(char* command)
{
    char * word, *tmp;
    char* result,*key,*value;
//    char* retval;
//    char* outer_ptr;
//    char* inner_ptr;
    StrMap *map;
    StrMapIterator pos;
    cJSON *json;//, *tmpjson;
   // json *sjson;
        //char * callback_value = ALLOC(char*,sizeof(char*));
    if(!command) return "-";

    word = strtok(command,"\n");
    if(strcmp(L(word),"read")==0) {
                tmp = NULL;
                /*
                 * 读取一个键值对
                 * 语法 read key
                 * 若key不存在则返回 ""k_v_no_value_saved"
                 */
                //memset(callback_value,0,sizeof(char*));
                /*i = atoi(strtok(NULL," "));
                printf("0x%08x\n",callback_value);
                read(i,callback_value);*/
                //tmp = strtok(NULL," ");
                //printf("> ACTION-READ:%s\n",command);
                tmp = strtok(NULL,"\n");
                //printf("waiting for trie get.");
                map = trie_get(root,tmp);
                //printf("trie get successed\n");
                json = cJSON_CreateObject();
                //sjson = json_create();
                if(map) {
                    for(pos=StrMap_begin(map); pos!=StrMap_end(map); pos = StrMap_next(pos)) {
                        //printf("|||%s||||%s|||\n",*StrMap_key(pos),*StrMap_value(pos));
                    cJSON_AddStringToObject(json,*StrMap_key(pos),*StrMap_value(pos));
                        //json_add(sjson,*StrMap_key(pos),*StrMap_value(pos));
                    }
                    result = cJSON_Print(json);
                    // result = json_print(sjson);
                     //printf("json print:%s\n",result);
                        //retval = (char*)malloc(strlen(result)*sizeof(char));
                        //memset(retval,0,strlen(retval)*sizeof(char));


                    free(map);
                    free(json);

                    return result;
                } else {

                    free(json);
                    result = "-NO RESULT";
                    return result;
                }


                //


            }
            if(strcmp(L(word),"write")==0) {
                /*
                 * 写入一个键值对，
                 * 语法 write key value
                 * 如果key已经存在，则替换
                 */
                 key = strtok(NULL,"\n");
                 value =strtok(NULL,"\n");
                 if(!value)return "-";
                //

                 trie_save(root,key,value);
                 //backup_save(key,value);
                //printf("write:%s:%s\n",key,value);
                 return "+";
                /*i = atoi(strtok(NULL," "));
                write(i,strtok(NULL," "));*/
             }
             if(strcmp(L(word),"add")==0) {
                /*
                 * 将值加入键值对中
                 * 语法 add key value
                 * * 例如，原先存储的数据为 hello world
                 * * 执行 add hello world2
                 * * 结果 hello->worldworld2
                 */
                 key = strtok(NULL," ");
                 value = strtok(NULL," ");
                 result = malloc_and_copy(__key_value_get(key_value,key));
                 tmp = (char*)malloc(strlen(result)+strlen(value));
                 sprintf(tmp,"%s%s",result,value);
                 __key_value_insert(key_value,key,tmp);
                 free(tmp);
                 return "+";
             }
             if(strcmp(L(word),"jsonadd")==0) {
                /*
                 * 以json格式添加到键值对中
                */
                /*
                 key = strtok(NULL," ");
                 value = strtok(NULL," ");
                 result = malloc_and_copy(__key_value_get(key_value,key));
                 json = cJSON_Parse(result);
                 if(!json) {
                    json = cJSON_CreateArray();
                    cJSON_AddItemToArray(json,cJSON_CreateString(result));
                }
                cJSON_AddItemToArray(json,cJSON_CreateString(value));
                result=cJSON_Print(json);
                __key_value_insert(key_value,key,result);
                return "+";
        */
            }
            if(strcmp(L(word),"jsonread")==0) {
                /*
                 * 以json格式输出
                 */
                 /*
                 tmp = strtok(NULL," ");
                 result = malloc_and_copy(__key_value_get(key_value,tmp));
                 json=cJSON_Parse(result);
                 if(!json) json =cJSON_CreateString(result);
                 result=cJSON_Print(json);
                 return result;
        */
             }
             if(strcmp(L(word),"dump")==0) {
                __key_value_dump();
                return "+";
            }
            if(strcmp(L(word),"recvr")==0) {
                trie_recover();
                return "+";
            }
            return "-";

        }

        void trie_recover()
        {
            FILE *fp;
            int num;
            int i = 0;
            char* readbuf = (char*)malloc(sizeof(int));
            char* keybuf;
            char* valuebuf;
            memset(readbuf,0,sizeof(int));
            fp = fopen("dict.lib","r");
            printf("recovering trie from dict:\n");
            if(!fp) {
                return;
            }
            do {

                //readbuf = (char*)realloc(readbuf,sizeof(int));
                //readbuf = (char*)malloc(sizeof(int));//(char*)sub_allocate(sizeof(int));
               //printf("%d\n",i);
                memset(readbuf,0,sizeof(int));
                fread(readbuf,sizeof(int),1,fp);
               // printf("read:next->%s\n",readbuf);
                if(!strcmp(readbuf,"1")==0){
                err:
                    printf("data-file format error:recovered:%d\n",i);
                    printf("last key:%s,last value:%s\n",keybuf,valuebuf);
                    printf("read:next->%s\n",readbuf);
                    return;
                }
                fread(readbuf,sizeof(int),1,fp);
                num = atoi(readbuf);
              // printf("read:num->%d\n",num);
                //return;
                if(num>0) {
                    keybuf = (char*)malloc(num);
                    readbuf = (char*)malloc(num+1);
                    memset(readbuf,0,num+1);
                }else return;

                fread(keybuf,num,1,fp);
               // keybuf[num]='\0';
               // printf("read:key->%s,strlen:%d\n",keybuf,strlen(keybuf));
                fread(readbuf,sizeof(int),1,fp);
                num = atoi(readbuf);
               // printf("read:num->%d\n",num);
                if(num>0) {
                    valuebuf = (char*)malloc(num);
                    memset(valuebuf,0,num);
                }else return;
                fread(valuebuf,num,1,fp);
                //valuebuf[num]='\0';
                //printf("read:value->%s,strlen:%d\n",valuebuf,strlen(valuebuf));
                //__key_value_insert(k_v,keybuf,);
               // printf("read from file:line:%d,key:%s,value:%s\n",i,keybuf,valuebuf);
                trie_save(root,keybuf,valuebuf);
                fread(valuebuf,sizeof(char),1,fp);
                i++;
            } while(1);
            //fclose(fp);
            printf("recover count:%d\n",i);
        }

        void _dump_proc(tjson* map)
        {
        //struct __key_value_node* pt = k_v_pool->k_v;
           // struct __key_value_node* temp;
            int i =0,count = 0;

            FILE *fd;

            fd = fopen("dict.lib","w+");
            printf("memory dump begin...\n");

            json_node *iter = map->front;
            do{
                //printf("dump:key:%s;value:%s\n",iter->key,iter->value);
                 __dump(fd,iter->key,iter->value);
                 count++;
                iter = iter->next;
             }while(iter);
      /*      for(pos=BackMap_begin(backup_map); pos!=BackMap_end(backup_map); pos = BackMap_next(pos)) {
                printf("key: %s , value:%s \n",*BackMap_key(pos),*BackMap_value(pos));
                __dump(fp,*BackMap_key(pos),*BackMap_value(pos));
                count++;
        }*/
        /*
        while(k_v && i< KEY_VALUE_SIZE){
        	if(k_v[i].key != NULL && k_v[i].value !=NULL){
        		printf("key-value:%s->%s\n",k_v[i].key,k_v[i].value);
        		count ++;
        		__dump(fp,k_v[i].key,k_v[i].value);
        		temp = k_v[i].next;
        		while(temp->key != NULL && temp->value !=NULL){
        			printf("key-value:%s->%s\n",temp->key,temp->value);
        			__dump(fp,temp->key,temp->value);
        			count ++;
        			temp = temp->next;
        		}
        	}
        	i++;
        }
        */
        fclose(fd);
        printf("\nmemory dump End... dump count:%d \n",count);
        //_endthreadex(0);
    }

    void __key_value_dump()
    {
        unsigned pid;

       //BackMap * backup =backup_map;

        pthread_create(&pid,NULL,(void *) _dump_proc,backup_map);
        //_beginthreadex(NULL,0, (unsigned (__stdcall *) (void *))_dump_proc,(void *)k_v,0,&pid);
    }
