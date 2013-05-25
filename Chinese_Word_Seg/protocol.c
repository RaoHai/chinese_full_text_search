#include "protocol.h"
#include "cstl/list.h"
#include "cstl/map.h"
CSTL_LIST_INTERFACE(CmdList, char *)
CSTL_LIST_IMPLEMENT(CmdList, char *)


cmd_map command_lines[] = {
	{"read",1,read_command},
	{"write",2,write_command}
};
void* read_command(Trie* trie,void* command_list){
	CmdListIterator pos = CmdList_begin(command_list);
	int i = 0;
	char *arg1,*tmp;
	for(pos = CmdList_next(pos);pos!=CmdList_end(command_list);pos = CmdList_next(pos),i++){
		if( i==0){
			tmp = *CmdList_data(pos);
			int len = strlen(tmp);
			arg1 = malloc(len);
			memset(arg1,0,len);
			strncpy(arg1,tmp,len);
		}
	}
	if(arg1){
		 return map_to_str(trie_get(trie,arg1));
	}
	else return "-";
}
void* write_command(Trie* trie,void* command_list){
	CmdListIterator pos = CmdList_begin(command_list);
	int i = 0;
	char * arg1,*arg2,*tmp;
	for(pos = CmdList_next(pos);pos!=CmdList_end(command_list);pos = CmdList_next(pos),i++){
	   // printf("action:%d\n",i);
		if( i==0){
			tmp = *CmdList_data(pos);
			int len = strlen(tmp);
			arg1 = malloc(len);
			memset(arg1,0,len);
			strncpy(arg1,tmp,len);

		}
		if(i==1){
			tmp = *CmdList_data(pos);
			int len = strlen(tmp);
			arg2= malloc(len);
			memset(arg2,0,len);
			strncpy(arg2,tmp,len);
		}
 	}
 	if(arg1 && arg2){
 		return (void*)trie_save(trie,arg1,arg2);
 		//return "+";
 	}
 	else return "-";
}
void func_test(){
	printf("function test\n");
}
void register_command(){
	int i;
	int num_command = sizeof(command_lines)/sizeof(cmd_map);
	command_dict =__key_value_init();
	for( i=0; i<num_command; i++){
		cmd_map * command_line = command_lines + i;
		//printf("%s:%p\n",command_line->command,command_line->callback_func);
		__key_value_insert_refer(command_dict,command_line->command,command_line->callback_func,1);
	}

}

void* p_command_parser(char* command)
/* 	COMMAND PROTOCOL:
 *	*int \r\n 		-->number of arguments(with command).
 *	$int \r\n 		-->strlen of command.
 *	command \r\n 		-->command
 *	$int \r\n 		-->strlen of first argument.
 *	argument1 \r\n 	-->first argument.
 *	....
 *	$int \r\n 		-->strlen of  argument N.
 *	argumentN \r\n 	-->argument N.
 *
 * 	BUT
 *	user input in console can also be inline format:
 *	just like:
 *	read hello \n
 *	or
 *	write hello world \n
 */
 {
 	int num_arguments = 0;
 	int strlen_commamd = 0;
 	int i;
 	int tmp_word_len;
 	char* tmp_word;
 	CmdList* command_list = CmdList_new();
 	char* tmp = command;
//printf("Start to parser:%s\n",command);
 	if( *tmp == '*'  ) num_arguments = get_next_int(&tmp);
 	if(num_arguments==0) return "+";
 	for( i = 0 ; i< num_arguments ; i++){
 		//printf(">Get:%c\n",*tmp );
 		if( *tmp == '$' ) {
 		    tmp_word = trim(get_next_str(&tmp));
 			//tmp_word[tmp_word_len+1] = '\0';
 			printf("parser:%s\n",tmp_word);
 			CmdList_push_back(command_list,tmp_word);
 			//tmp += 2;
 		}

 	}
 	CmdListIterator pos = CmdList_begin(command_list);
 	func pf;
    //printf("get command:%s\n",*CmdList_data(pos));
 	pf =  __key_value_get(command_dict,*CmdList_data(pos));

 	//for(pos = CmdList_next(pos);pos!=CmdList_end(command_list);pos = CmdList_next(pos)){
 		//printf("%s\n",*CmdList_data(pos));
 	//}

 	if(!pf){
 	    printf("pf get null:%s\n",*CmdList_data(pos));
 	     return "-";
        }
 	if(pf && *pf != NO_VALUE){
 		return pf(root,command_list);
 	}
 	//return "+";
 }
char*  get_next_str(char** pt){
    int i = 0;
    char * retval;
    char* tmp = *pt;
    char* desc = tmp;
    for(;*tmp!='\r' && * (tmp++) != '\n'; i++,(*pt)++){
        printf("%c",*tmp);
    }
    i = i -1;
    retval = (char*) malloc (i * sizeof(char));
    memset(retval,0,i);
    strncpy(retval,desc+1,i);
     (*pt)+=2;
    return retval;
}
 int get_next_int(char** pt){
 	int i = 0;
 	int retint = 0;
 	char* tmp = *pt;
 	char* desc = tmp;
 	char* retval;
    /*
     *      i is the length of int...
     *      e.g.  $65536\r\n
     *      i  is  5.
     *      ^_^
     */
     for(;*tmp!='\r' && * (tmp++) != '\n'; tmp++,i++,(*pt)++);
     	retval = (char*) malloc (i * sizeof(char));
     memset(retval,0,i * sizeof(char));
     strncpy(retval,desc+1,i);
     (*pt)+=3;
     return atoi(retval);


 }
