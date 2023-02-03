// kian ghassemi 401102264 ; 

#include <stdio.h>
#include <sys\stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <dirent.h> 

#define MAX_LENGTH  302
#define MAX_LINE 200 
#define Empty_line  "#@!$$$*KIAN_GSVIM$$$!@#"
#define MAX_FINDCASE 1000 
#define MAX_FILES 10 
#define MAX_NUMBER  10000
#define MAX_SIZE 90000
#define MAX_WORDS 50
#define MAX_DIGIT 10

int current_command = 0 ;
char temp_save[MAX_SIZE] =  {0} ; 
char arman_commandline[MAX_LENGTH] = {0} ; 
bool Arman_activation = false ; 
int  last_arman = false ; 

char arman_sign[3] = "=D\0"      ;  
char at_sign[4] = "-at\0"        ;
char count_sign[7] = "-count\0"  ;
char all_sign[5] = "-all\0"      ; 
char byword_sign[8] ="-byword\0" ;
char c_grep_sign[3] = "-c\0"     ; 
char l_grep_sign[3] = "-l\0"     ; 
char remove_flag1[3] = "-f\0"    ;
char remove_flag2[3] = "-b\0"    ;

void create_dir(const char * address) ;
void translate_dir(char * address) ; 
void translate_string(char * string) ;
void translate_to_word(char * address , int *pointer ) ; 
int  lines_of_file(char * adress  ) ; 
int first_brace_finder(char * string) ; 
int end_brace_finder(char * string , int* check) ; 
bool generate_brace(char * address , int def , int  start[2] , int end[2]) ; 
void correct_first_brace(char * address , int line , int byte , int* end  ) ; 
void correct_end_brace(char * address , int*line , int byte  ) ; 
int arrange_by_space(char * string , int start , int end) ; 
int start_printing(char * string) ;
void show_cmp1(char * a , char * b  , int m ) ; 
void show_cmp2(char * a  , int n ,  int m ) ;
void show_cmp3(char * a , char * b , int m , int p) ;
void wildcard_fixer(char * a ); 

void createfile(char * address) ; 
void insert_str(char * address  , char * string  , int line  , int byte ) ; 
void cat(char * address) ; 
void remove_str(char * address , int line ,  int byte , int size , int ward) ; 
void copy_str(char * address , int line  , int byte , int size , int ward ) ; 
void cut_str(char * address , int line  , int byte , int size , int ward) ; 
void paste_str(char * address , int line , int byte) ; 
void find_str( char * address , char * string)  ;
void Show_find(int findcase[MAX_FINDCASE] , int , char * address ) ; 
void replace_str(char * address , char * string1 , char * string2  ) ; 
void grep_str( int num_file  , char files[MAX_FILES][MAX_LENGTH]  , char * string) ; 
void closing_pair(char * address  ) ; 
void text_comprator(char * file1 , char * file2) ; 
void dir_tree(char * dirname , int , int ) ; 
int line_counter(char * address) ;
int one_differ(char * string1 , char * string2) ; 



bool Search_Commands(char * command) ; 
bool Dir_exist(char * address) ; 
bool In_commandline(char * string  , char * pattern) ; 
void rec_wildcard(char * string) ; 

struct option1{
    bool byword ; 
    int at ; 
    bool count ; 
    bool all ;  
}opt_find ; 

struct option2{
    bool l ; 
    bool c ; 
}opt_grep ; 

struct mode{
    bool mode  ; 
    bool first_multichar  ; 
    bool end_multichar ;
}Wildcard ; 


char* COMMANDS[14] = {
    "createfile",
    "insertstr",
    "cat",
    "removestr",
    "copystr",
    "cutstr",
    "pastestr",
    "find",
    "replace",
    "grep",
    "undo",
    "auto-indent",
    "compare",
    "tree"};

  
void run(char * string){
    while(1){
        //Section 1 : 
        char commandline[MAX_LENGTH] = {0} ; 
        char temp_commandline[MAX_LENGTH] = {0} ;
        if(Arman_activation == false) strcpy(commandline , string) ; 
        strcpy(temp_commandline , commandline) ;  
        char * command ;
        char tok1[2] = " " ;  
        command = strtok(temp_commandline , tok1) ; 
        if(Search_Commands(command) == false){
            Arman_activation = false ; 
             printf("Invalid Command\n") ;
             break  ;
        }
        //section 2  :
        int arg =  0 ;
        char tok ; 
        int number_of_files = 0 ; 
        int number_of_strings = 0 ; 
        int SIZE = 0 ; 
        char POS[MAX_LENGTH] = {0} ; 
        char STR[2][MAX_LENGTH] ; 
        char FILE_ADD[MAX_FILES][MAX_LENGTH] ;
        bool pos = false  ;  
        bool arman_use = false ;
        for(int i = strlen(command) - 1 ; i < strlen(commandline) ; i ++ ){
            if(commandline[i] == '-' && commandline[i+1] == '-' && commandline[i+2] == 'f' && commandline[i+3] == 'i' && commandline[i+4] == 'l' && commandline[i+5] == 'e' ){
                i += 6 ; 
                while(1){
                  if(commandline[i] == ' ') i ++ ;  else break  ; 
                }
                arg = i ; 
                if(commandline[i] == '\"') tok = '\"' ;  else tok = ' ' ; 
                while(1){
                    if(tok == ' '){
                        if(commandline[i] == tok){
                            FILE_ADD[number_of_files][i - arg] = '\0' ;
                            break ; 
                        }
                    }else{
                        if(arg != i && commandline[i] == tok && commandline[i-1] != '\\'){
                             FILE_ADD[number_of_files][i - arg] = commandline[i] ;
                             FILE_ADD[number_of_files][i+1 - arg] = '\0' ;  
                             break ;
                        }   
                    }
                    FILE_ADD[number_of_files][i - arg] =  commandline[i] ; 
                    i ++ ; 
                }
                number_of_files ++ ; 
             }
            if(commandline[i] == '-' && commandline[i+1] == '-' && commandline[i+2] == 's' && commandline[i+3] == 't' && commandline[i+4] == 'r'){
                i += 5 ; 
                while(1){
                  if(commandline[i] == ' ') i ++ ;  else break  ; 
                }
                arg = i ; 
                if(commandline[i] == '\"') tok = '\"' ;  else tok = ' ' ; 
                while(1){
                    if(tok == ' '){
                        if(commandline[i] == tok){
                            STR[number_of_strings][i - arg] = '\0' ;
                            break ; 
                        }
                    }else{
                        if(arg != i && commandline[i] == tok && commandline[i-1] != '\\'){
                             STR[number_of_strings][i - arg] = commandline[i] ;
                             STR[number_of_strings][i+1 - arg] = '\0' ;  
                             break ;
                        }   
                    }
                    STR[number_of_strings][i - arg] =  commandline[i] ; 
                    i ++ ; 
                }
                number_of_strings ++ ; 
             }

            if(commandline[i] == '-' && commandline[i+1] == '-' && commandline[i+2] == 'p' && commandline[i+3] == 'o' && commandline[i+4] == 's'){
                pos = true ;
                i += 5 ; 
                while(1){
                  if(commandline[i] == ' ') i ++ ;  else break  ; 
                }
                arg = i ; 
                tok = ' ' ; 
                while(1){
                    if(commandline[i] == tok ) break ; 
                    POS[i - arg] = commandline[i] ; 
                    i ++; 
                }
             }
            if(commandline[i] == '-' && commandline[i+1] == 's' && commandline[i+2] == 'i' && commandline[i+3] == 'z' && commandline[i+4] == 'e'){
                i += 5 ; 
                while(1){
                  if(commandline[i] == ' ') i ++ ;  else break  ; 
                }
                arg = i ; 
                tok = ' ' ; 
                while(1){
                    if(commandline[i] == tok ) break ; 
                    SIZE *= 10 ;
                    SIZE += (int)(commandline[i] - '0') ; 
                    i ++; 
                }
             }
            if(commandline[i] == '=' && commandline[i+1] == 'D'){
                i += 2 ; 
                while(1){
                if(commandline[i] == ' ') i ++ ;  else break  ; 
                }
                Arman_activation = true ;
                arman_use = true  ; 
                for(int j = i ; j < strlen(commandline) ; j ++){
                    arman_commandline[j - i] = commandline[j] ;
                    if(j+1 == strlen(commandline)) arman_commandline[j+1 - i] = '\0' ; 
                }
             }
        }
        //check errors : 
        bool access = true ; 
        if(arman_use == false ) last_arman = true ; 
        if(strcmp(command , "createfile")){
            for(int i = 0 ; i < number_of_files ; i ++){
                translate_dir(FILE_ADD[i]) ; 
                if(Dir_exist(FILE_ADD[i]) == false){
                    access = false ;
                    Arman_activation = false ;
                    printf("Directoy doesn't exist\n") ; 
                    break ; 
                }
                FILE* my_file = fopen(FILE_ADD[i] , "r") ; 
                if(my_file == NULL){
                    printf("File doesn't exist\n") ; 
                    access = false ; 
                    Arman_activation = false ; 
                    break ; 
                } else fclose(my_file) ; 
            }
        }
        //run 
        if(access){
            if(strcmp(command , "cat") == 0 ) cat(FILE_ADD[0]) ;
            if(strcmp(command , "auto-indent") == 0) closing_pair(FILE_ADD[0]) ; 
            if(strcmp(command , "createfile") == 0 ) createfile(FILE_ADD[0]) ;
            if(strcmp(command , "compare") == 0 ) text_comprator(FILE_ADD[0] , FILE_ADD[1]) ;  
            if(strcmp(command , "tree") == 0 ){
                int depth = 0 ; 
                bool sign = true ; 
                int i = strlen(command) ; 
                while(1){
                    if(commandline[i] == ' ') i ++  ; else break ; 
                }
                for( i ; i < strlen(commandline) ; i++ ){
                    if(commandline[i] == ' ') break ;
                        if(commandline[i] == '-'){
                            sign = false ; 
                            continue ; 
                        }
                        depth *= 10 ; 
                        depth += (int)(commandline[i] - '0') ; 
                }
                char address[MAX_LENGTH] ; 
                strcpy(address , ".") ; 
                if(sign == false) depth *= -1 ; 
                dir_tree(address ,  0 , depth) ; 
             }
            if(strcmp(command , "find") == 0){
                translate_string(STR[0]) ; 
                rec_wildcard(STR[0]) ; 
                if(In_commandline(commandline , count_sign)) opt_find.count = true ; else opt_find.count = false ;  
                if(In_commandline(commandline , all_sign)) opt_find.all = true ; else opt_find.all = false ;  
                if(In_commandline(commandline , byword_sign)) opt_find.byword = true ; else opt_find.byword = false ;  
                if(In_commandline(commandline , at_sign) == false ) opt_find.at = 0 ; else {
                    for(int i = 0 ; i < strlen(commandline) ; i ++){
                        if(commandline[i] == '-' && commandline[i+1] == 'a' && commandline[i+2] == 't' ){
                            i += 3 ; 
                            while(1){
                                if(commandline[i] == ' ') i ++ ; else break ; 
                            }
                            opt_find.at = 0 ; 
                            for(int j = i ; j < strlen(commandline) ; j ++) {
                                if(commandline[j] == ' ') break ; 
                                opt_find.at *= 10 ; 
                                opt_find.at += (int)(commandline[j] - '0') ; 
                            }
                            break ; 
                        }
                    }
                } 
                if((opt_find.at != 0 && opt_find.all == true) || (opt_find.count == true && (opt_find.at != 0  | opt_find.all == true | opt_find.byword == true))){
                    printf("Those options doesn't match\n") ; 
                    continue ; 
                }
                find_str(FILE_ADD[0] , STR[0])  ;
             }
            if(strcmp(command , "replace") == 0 ){ 
                 translate_string(STR[0]) ; 
                 translate_string(STR[1]) ; 
               // rec_wildcard(STR[0]) ;
                if(In_commandline(commandline , all_sign)) opt_find.all = true ; else opt_find.all = false ;  
                if(In_commandline(commandline , at_sign) == false ) opt_find.at = 0 ; else {
                    for(int i = 0 ; i < strlen(commandline) ; i ++){
                        if(commandline[i] == '-' && commandline[i+1] == 'a' && commandline[i+2] == 't' ){
                            i += 3 ; 
                            while(1){
                                if(commandline[i] == ' ') i ++ ; else break ; 
                            }
                            opt_find.at = 0 ; 
                            for(int j = i ; j < strlen(commandline) ; j ++) {
                                if(commandline[j] == ' ') break ; 
                                opt_find.at *= 10 ; 
                                opt_find.at += (int)(commandline[j] - '0') ; 
                            }
                            break ; 
                        }
                    }
                } 
                if((opt_find.at != 0 && opt_find.all == true)){
                    printf("Those options doesn't match\n") ; 
                    continue ; 
                }
                replace_str(FILE_ADD[0]  , STR[0] , STR[1]) ;                 
             }
            if(strcmp(command , "grep") == 0 ){
                if(In_commandline(commandline , l_grep_sign)) opt_grep.l = true ; else opt_grep.l = false ;  
                if(In_commandline(commandline , c_grep_sign)) opt_grep.c = true ; else opt_grep.c = false ;  
                grep_str(number_of_files , FILE_ADD , STR[0] ) ;

             }

            if(pos){
                int line = 0 ; 
                int  byte = 0 ; 
                char tokken[2] = ":" ; 
                char * help  = strtok(POS , tokken ) ; 
                for(int i = 0 ; i < strlen(help) ; i ++){
                    line *= 10 ; 
                    line += (int)(help[i] - '0') ; 
            
                }
                help = strtok(NULL , tokken) ; 
                for(int i = 0 ; i < strlen(help) ; i ++){
                    byte *= 10 ; 
                    byte += (int)(help[i] - '0') ; 
                }
                if(strcmp(command , "insertstr") == 0 ) insert_str(FILE_ADD[0] , STR[0] , line , byte ) ; 
                if(strcmp(command , "pastestr") == 0 ) paste_str(FILE_ADD[0] , line, byte) ; else{
                        int ward = 0 ;
                        if(In_commandline(commandline , remove_flag1)) ward = 1 ;
                        if(In_commandline(commandline , remove_flag2)) ward = -1 ;
                        if(strcmp(command , "removestr") == 0 ) remove_str(FILE_ADD[0] ,line , byte , SIZE , ward ) ;
                        if(strcmp(command , "copystr") == 0) copy_str(FILE_ADD[0] ,line , byte , SIZE , ward ) ;
                        if(strcmp(command , "cutstr") == 0 ) cut_str(FILE_ADD[0] ,line , byte , SIZE , ward ) ;


                }

             }

            if(arman_use == false ){
                 break ; 
                 Arman_activation = false ; 
                temp_save[0] = '\0' ; 
             }
        }
    } 
    return ; 
}

// int main (){
//     char string[MAX_LENGTH] ; 
//     scanf(" %[^\n]s" , string ) ; 
//     run(string) ; 
// }

void createfile(char * address){
    translate_dir(address) ; 
    create_dir(address)  ;    
    if (access(address , F_OK) == 0) {
        printf("file already exists\n") ; 
    }else{
    FILE*  my_file = fopen( address  , "w") ; 
    fclose(my_file) ;        
    } 
 }

void insert_str(char * address  , char * string  , int line  , int byte ) {
 char address2[MAX_LENGTH] ; 
 strcpy(address2 , address) ; 
 strcat(address2 , "temp") ; 
 FILE * my_file = fopen(address , "r") ; 
 FILE * temp_file = fopen(address2 , "w") ; 
 char newline[MAX_SIZE] = {0} ; 
 translate_string(string) ; 
 char buffer_line[MAX_LENGTH] ; 
 int current_line = 1 ;
 bool ISend = true ;   
 bool keep_reading = true ;
 int file_size = line_counter(address) ;
 while(keep_reading == true){
        fgets(buffer_line , MAX_LENGTH , my_file) ;
        if(current_line > file_size){
        if(current_line < line ) {
            fputs("\n" , temp_file ) ;
            }
        if(current_line == line){
            for(int i = 0 ; i < byte ; i ++ ) newline[i] = ' ' ; 
            if(Arman_activation == false) strcat(newline , string ) ; else strcat(newline , temp_save) ; 
            fputs(newline , temp_file) ; 
            keep_reading = false ;
        }
        if(current_line > line ){
            keep_reading = false ; 
        } 
    }else{
        if(current_line == line){
           if(byte > strlen(buffer_line) - 1 ){
            buffer_line[strlen(buffer_line) - 1 ] = buffer_line[strlen(buffer_line)] ; 
            strcpy(newline , buffer_line) ; 
            for(int i = 0 ; i < byte - strlen(buffer_line)   ; i ++ ) strcat(newline , " ")  ; 
            if(Arman_activation == false ) strcat(newline , string) ; else strcat(newline , temp_save) ;  
            if(file_size != current_line) strcat(newline , "\n") ; 
            fputs(newline , temp_file) ; 
           }else{
            memcpy(newline  , buffer_line , byte) ; 
            if(Arman_activation == false ) strcat(newline , string) ; else strcat(newline , temp_save) ;  
            int temp_size = strlen(newline) ;  
            for(int i  = 0 ; i+byte < strlen(buffer_line) ; i++){
                newline[temp_size + i] = buffer_line[i + byte] ; 
            }
            fputs(newline , temp_file) ; 
           }
        }else{
            if(current_line == file_size && line > file_size){
                if(buffer_line[strlen(buffer_line) - 1] != '\n') strcat(buffer_line , "\n") ; 
            }
            fputs(buffer_line , temp_file) ; 
        }
    }
    current_line ++ ; 
   }
 fclose(my_file) ; 
 fclose(temp_file) ;
   remove(address) ; 
   rename(address2 , address) ; 
 }
 
void cat(char * address) {
    translate_dir(address) ; 
    FILE * my_file = fopen(address , "r") ; 
    char buffer_line[MAX_LENGTH] ; 
    bool keep_reading = true ; 
    while(keep_reading == true){
        fgets(buffer_line , MAX_LENGTH , my_file) ; 
        if(Arman_activation == false )printf("%s" , buffer_line) ;
        if(Arman_activation == true ) strcat(temp_save  , buffer_line) ; 
        if(feof(my_file)) keep_reading = false ;  
    }
 }

void remove_str(char * address , int line ,  int byte , int size , int ward  ) {
    translate_dir(address) ; 
    FILE * my_file = fopen(address  , "r+") ; 
    char address2[MAX_LENGTH] ; 
    strcpy(address2 , address) ; 
    strcat(address2 , "temp") ; 
    FILE * temp_file = fopen(address2 , "w") ; 
   //section 1 : 
    char ** buffer_line = (char **)malloc(MAX_LINE * sizeof(char *)) ; 
    int k = 0 ; 
    bool keep_reading = true ; 
    while(keep_reading){
     *(buffer_line + k) = calloc(MAX_LENGTH  ,  sizeof(char)) ; 
     fgets(buffer_line[k] , MAX_LENGTH , my_file) ; 
     if(feof(my_file)) keep_reading = false ; 
     k ++ ; 
    }

    //section 2 :
    int pointer_byte = byte-1 ;
    if(ward == 1) pointer_byte ++  ;
    int start = pointer_byte ; 
    int pointer_line = line - 1 ; 
    while(1){
         start += ward ;
         size -- ;   
         if(start == -2 ){
            char temp_buffer_line[MAX_LENGTH] = {0} ; 
            for(int i = 0 ; ; i ++){
                if(ward == -1){
                if(buffer_line[pointer_line][i + pointer_byte +1 ] == '\0') break ; 
                temp_buffer_line[i] = buffer_line[pointer_line][i+pointer_byte + 1 ] ;
                }
            }
           pointer_line -- ; 
           buffer_line[pointer_line][strlen(buffer_line[pointer_line]) - 1] =  buffer_line[pointer_line][strlen(buffer_line[pointer_line])] ; 
           pointer_byte = strlen(buffer_line[pointer_line]) - 1 ; 
           start = pointer_byte ;
           strcat(buffer_line[pointer_line] , temp_buffer_line) ; 
           strcpy(buffer_line[pointer_line + 1 ] , Empty_line) ; 
         }
         
         if(start ==  (strlen(buffer_line[pointer_line]))){ 
            strcat(buffer_line[pointer_line] , buffer_line[pointer_line + 1]) ; 
            for(int i = pointer_line + 1 ; i < k - 1 ; i ++ ) {
            strcpy(buffer_line[i] , buffer_line[i+1]) ; 
            }
            k -- ; 
         }

         if(size == 0 ){
                char temp_buffer_line[MAX_LENGTH] = {0} ; 
            if(ward == -1){
            for(int i = 0 ; i <= start ; i ++ ){
                temp_buffer_line[i] = buffer_line[pointer_line][i] ; 
            }

            int distance = pointer_byte   - start ;
            for(int i = start+1 ; ; i ++ ){
              if(buffer_line[pointer_line][i + distance] == '\0') break ;
              temp_buffer_line[i] = buffer_line[pointer_line][i + distance] ;  
            }
         
         strcpy(buffer_line[pointer_line] , temp_buffer_line) ;
         break ;  
           }else{
            for(int i = 0 ; i < pointer_byte ; i++){
                temp_buffer_line[i] = buffer_line[pointer_line][i] ; 
             }
            for(int i = 0  ; i < strlen(buffer_line[pointer_line]) - start + 1 ; i ++  ){
            temp_buffer_line[i + pointer_byte] = buffer_line[pointer_line][start + i] ; 
            }
            strcpy(buffer_line[pointer_line] , temp_buffer_line) ; 
            break ; 
      }
     }
   }
    //add to file 
   for(int j = 0 ; j < k ; j ++ ){
    if(strcmp(buffer_line[j] , Empty_line) != 0 ) {
            fputs(buffer_line[j] , temp_file) ;
    }
   }
   for(int i = 0 ; i < k ; i ++) free(buffer_line[i]) ;
   free(buffer_line) ; 
   fclose(my_file) ; 
   fclose(temp_file) ; 
   remove(address) ; 
   rename(address2 , address ) ; 
 }


void copy_str(char * address , int line  , int byte , int size , int ward ) {
    translate_dir(address) ; 
    char readfile[MAX_SIZE] = {0} ; 
    char buffer_line[MAX_LENGTH] ; 
    int first_p_line[line] ; 
     FILE * my_file = fopen(address  , "r") ;
     for(int i = 0 ; i < line - 1 ; i ++ ){
        first_p_line[i] = ftell(my_file) ;
        fgets(buffer_line , MAX_LENGTH  , my_file) ; 
     }
     int of_the_line = ftell(my_file) ; 
     first_p_line[line - 1] = of_the_line ; 
     of_the_line += byte ; 
     if(ward == -1){
        of_the_line -= size ; 
        int base = line - 1 ;
        for(int i = line -1 ; ;i -- ){
            if(of_the_line >= first_p_line[i]){
                of_the_line -= (base - i) ; 
                base = i ; 
                if(of_the_line >= first_p_line[i]) break ; 
            }
        }
     }
     fseek(my_file , of_the_line   , SEEK_SET)  ;
     fread( readfile , sizeof(char) , size  , my_file) ; 
     const size_t len = strlen(readfile) + 1;
     HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
     memcpy(GlobalLock(hMem), readfile, len);
     GlobalUnlock(hMem);
     OpenClipboard(0);
     EmptyClipboard();
     SetClipboardData(CF_TEXT, hMem);
     CloseClipboard(); 
     fclose(my_file) ;       

 }

void cut_str(char * address , int line , int byte , int size ,  int ward){
    copy_str(address , line , byte , size , ward) ; 
    remove_str(address , line , byte , size , ward) ; 
 }
void paste_str(char * address , int line , int byte ){  
    char string[MAX_LENGTH] ; 
    HANDLE h;
    OpenClipboard(NULL);
    h = GetClipboardData(CF_TEXT);
    strcpy(string , (char *)h) ; 
    CloseClipboard();
   insert_str(address , string , line , byte ) ; 
 }

void find_str( char * address , char * string ){
    int findcase[MAX_FINDCASE]  ;
    int cnt = 0 ; 
    int size = strlen(string) ;
    translate_dir(address) ; 
    translate_string(string); 
    FILE * my_file = fopen(address , "r") ; 
    char buffer_line[MAX_LENGTH] ;
    bool keep_reading = true  ;  
    int line = 0 ;
    //section  1 : find 
    if(Wildcard.mode == false){ 
        while(keep_reading){
            int pointer = ftell(my_file) - line  ;  
            fgets(buffer_line  , MAX_LENGTH , my_file) ; 
            line ++ ; 
            int current_letter = 0  ; 
            for(int i = 0 ; i < strlen(buffer_line) ; i ++ ){
                if(string[current_letter] != buffer_line[i]){
                pointer += current_letter+1 ;
                current_letter = 0  ;
                }else{
                current_letter ++ ; 
                }

             if(current_letter == size ){
                if(opt_find.count != true ) findcase[cnt] = pointer ; 
                cnt ++ ; 
                current_letter = 0 ;
                pointer += size ;  
                if( (opt_find.all != true && opt_find.at == 0 && opt_find.count == false  ) || opt_find.at == cnt ){
                keep_reading  = false ; 
                 break ;                
                  } 
                }
            }
            if(feof(my_file)) break ;  
        }
    }else{
        wildcard_fixer(string) ; 
        char string_separate[MAX_WORDS][MAX_LENGTH] ; 
        int number_of_words  = 1 ; 
        int tok = 0 ;
        size = strlen(string) ;
        for(int i = 0 ; i < size ; i++){
            if(string[i] == '*'){
                tok =  0; 
                number_of_words ++ ; 
                continue ; 
            }
            if(string[i] == '\\' && string[i+1]== '*') ++i ;
            string_separate[number_of_words - 1][tok] = string[i] ;
            tok ++ ; 
        }

        while(keep_reading){
            int pointer = ftell(my_file) - line  ;  
            int pointer_temp = pointer ; 
            fgets(buffer_line  , MAX_LENGTH , my_file) ; 
            line ++ ; 
            int current_letter = 0  ;
            int current_word = 0 ;  
            for(int i = 0 ; i < strlen(buffer_line) ; i ++ ){
                if( string_separate[current_word][current_letter] != buffer_line[i]){
                    if(buffer_line[i] == ' ') current_word = 0 ; 
                    if(current_word == 0) pointer = pointer_temp + i+1 ;
                    current_letter = 0  ;
                }else{
                current_letter ++ ; 
                }

                if(current_letter == strlen(string_separate[current_word])){
                     current_word ++ ;
                     current_letter = 0 ;
                }
                if(current_word == number_of_words ){
                    if(Wildcard.first_multichar){
                        while(1){
                            if(buffer_line[pointer - pointer_temp - 1] != ' ' && (pointer - pointer_temp == 1)) pointer -- ; else
                             break ; 
                        }
                    }
                    if(opt_find.count != true ) findcase[cnt] = pointer ; 
                    cnt ++ ; 
                    current_letter = 0 ;
                    current_word = 0 ;
                    pointer ++ ;
                    if( (opt_find.all != true && opt_find.at == 0 && opt_find.count == false  ) || opt_find.at == cnt ){
                    keep_reading  = false ; 
                     break ;                
                  } 
                }
            }
            if(feof(my_file)) break ;  
        }
    }
    //scetion 2 : show
    Show_find(findcase , cnt , address) ; 
    fclose(my_file) ;
 }  
void Show_find(int findcase[MAX_FINDCASE] , int cnt , char * address){
     if(opt_find.count){
       if(Arman_activation == false ) printf("%d\n" , cnt ) ; 
       if(Arman_activation == true){
        char word[15] ; 
        sprintf(word  , "%d"  , cnt) ;
        strcat(word , "\n") ; 
        strcat(temp_save , word) ; 
       }
    }else{
        if(cnt == 0){
            if(Arman_activation == false ) printf("%d\n" , -1) ;
            if(Arman_activation == true ){
                strcat(temp_save , "-1\n") ;
            }
            printf("The string was not found in the text\n") ; 
        }else {
            if(opt_find.byword){
                for(int i = 0 ; i < cnt ; i ++){
                    translate_to_word(address  , &findcase[i]) ; 
                }
            }
        char word[15] ; 
        if(opt_find.all){
            for(int i = 0 ; i < cnt - 1 ; i ++){
                if(Arman_activation == false) printf("%d, " , findcase[i]) ;
                if(Arman_activation == true){
                    sprintf(word , "%d" , findcase[i] ) ; 
                    strcat(word , ", ") ; 
                    strcat(temp_save , word) ; 
                } 
            }
            if(Arman_activation == false)  printf("%d\n" , findcase[cnt - 1]) ;
            if(Arman_activation == true){
                sprintf(word , "%d" , findcase[cnt - 1] ) ;
                strcat(word  , "\n") ;
                strcat(temp_save , word) ;
            } 
        }else{
            if(opt_find.at != 0){
                if(Arman_activation == false) printf("%d\n" , findcase[cnt - 1]) ;
                if(Arman_activation == true ){
                    sprintf(word , "%d" , findcase[cnt - 1] ) ; 
                    strcat(word , "\n") ; 
                    strcat(temp_save , word) ;
                }
            }else{
                if(Arman_activation == false ) printf("%d\n" , findcase[0]) ;
                if(Arman_activation == true ){
                    sprintf(word , "%d" , findcase[0] ) ; 
                    strcat(word , "\n") ; 
                    strcat(temp_save , word) ;
                } 
            }
        } 
    }
  }
 }
void replace_str(char * address , char * string1 , char * string2  ){
    int findcase[MAX_FINDCASE][2]  ;
    int cnt = 0 ; 
    int size = strlen(string1) ;
    int size_add = strlen(string2) - size ;  
    translate_dir(address) ; 
    translate_string(string1); 
    FILE * my_file = fopen(address , "r") ; 
    char buffer_line[MAX_LENGTH] ;
    bool keep_reading = true  ;  
    int line = 0 ;
    int byte = 0 ;  
    //section  1 : find  
    while(keep_reading){
        int pointer = ftell(my_file) ;
        byte = pointer ;  
        fgets(buffer_line  , MAX_LENGTH , my_file) ;
        printf("%s\n" , buffer_line) ;  
        line ++ ;
        int current_letter = 0  ; 
        for(int i = 0 ; i < strlen(buffer_line) ; i ++ ){
            if(string1[current_letter] != buffer_line[i]){
                pointer += current_letter+1 ;
                current_letter = 0  ;
            }else{
                current_letter ++ ; 
            }
            if(current_letter == size ){
                findcase[cnt][0] = line  ;
                findcase[cnt][1] = pointer - byte ;  
            cnt ++ ; 
            current_letter = 0 ;
            pointer += size ;  
            if( (opt_find.all != true && opt_find.at == 0 ) || opt_find.at == cnt ){
                keep_reading  = false ; 
                 break ;                
             } 
           }
        }
        if(feof(my_file)) break ;  
    }
    fclose(my_file) ; 
    //section 2 : replace
    if(cnt == 0){
        printf("The string was not found in the text\n") ; 
    } else {
        if(opt_find.at == 0 ){
            int in_same_line = 0 ; 
            for(int i = 0 ; i < cnt ; i ++  ){
                if(i != 0 ){
                    if(findcase[i][0] == findcase[i-1][0]) in_same_line ++ ; else 
                    in_same_line = 0 ; 
                }
                remove_str(address , findcase[i][0] , findcase[i][1] + size_add * in_same_line   , strlen(string1) , 1  ) ; 
                insert_str(address ,  string2  ,  findcase[i][0]  , findcase[i][1] + size_add * in_same_line) ; 
            }
        }else{
            remove_str(address , findcase[cnt - 1][0] , findcase[cnt - 1 ][1]  , strlen(string1) , 1  ) ; 
            insert_str(address ,  string2  ,  findcase[cnt - 1 ][0] , findcase[cnt - 1 ][1]) ; 
        }
    }

 } 

void grep_str(int num_file  , char files[MAX_FILES][MAX_LENGTH]  , char * string) {
    int lines_with_word = 0 ; 
    for(int i = 0 ; i < num_file ; i ++ ){
        FILE * my_file = fopen(files[i] , "r") ;
        bool keep_reading = true ; 
        char buffer_line[MAX_LENGTH] ; 
        int current_letter = 0 ;
        if(Arman_activation == true && last_arman == false  ) strcpy(string , temp_save) ; 
        if(string[strlen(string) - 1] == '\n') string[strlen(string) - 1] = '\0' ; 
        int size = strlen(string) ;
        while(fgets(buffer_line , MAX_LENGTH , my_file) != NULL && keep_reading == true ){
            for(int j = 0 ; j < strlen(buffer_line) ; j ++ ){
                if(current_letter == size){
                    lines_with_word ++ ;
                    current_letter = 0 ;  
                    if(opt_grep.c != true){
                        if(opt_grep.l != true){
                            if(Arman_activation == false || last_arman) printf("%s: %s" , files[i]  , buffer_line ) ;
                            if(Arman_activation == true && last_arman == false ){
                                strcat(temp_save , files[i]) ;
                                strcat(temp_save , ": ") ;
                                strcat(temp_save , buffer_line) ;
                            }
                            if(buffer_line[strlen(buffer_line) - 1] != '\n'){
                                if(Arman_activation == false || last_arman ) printf("\n") ;
                                if(Arman_activation == true && last_arman == false  ) strcat(temp_save , "\n") ;  
                                }
                            }else{
                                if(Arman_activation == false || last_arman ) printf("%s\n"  , files[i]) ;
                                if(Arman_activation == true && last_arman == false  ){
                                    strcat(temp_save , files[i]) ;
                                    strcat(temp_save , "\n") ; 
                                } 
                                keep_reading = false ;  
                        }
                    }
                    break ; 
                }
                if(string[current_letter] != buffer_line[j]) current_letter = 0 ; else 
                 current_letter ++ ; 
            }
        }
        fclose(my_file) ; 
    }
    if(opt_grep.c){
        if(Arman_activation == false || last_arman ) printf("%d\n" , lines_with_word) ;
        if(Arman_activation == true  && last_arman == false  ) {
            char word[15] ;
            sprintf(word , "%d" , lines_of_file) ; 
            strcat(word , "\n") ;
            strcat(temp_save , word) ; 
        }
    }
 } 

void closing_pair(char * address ) {
     int dif_line = -1 ; 
     int start[2] , end[2] ;
     while(generate_brace(address , dif_line , start , end)) {
          correct_end_brace(address , &end[0] , end[1])  ;
          correct_first_brace(address , start[0] , start[1] , &end[0]) ;
          arrange_by_space(address , start[0] , end[0]) ;
         
         dif_line = start[0] ; 
     }
 } 
void text_comprator(char * file1 , char * file2){
    int len1 = lines_of_file(file1) ; 
    int len2 = lines_of_file(file2) ; 
    FILE * file_1 = fopen(file1  , "r") ;
    FILE * file_2 = fopen(file2  , "r") ; 
    char buffer_1[MAX_LENGTH] ; 
    char buffer_2[MAX_LENGTH] ; 
    int max = (len2  > len1) ? len2 : len1 ; 
    int min = (len2 <= len1) ? len2 : len1 ; 
    for(int i = 0 ; i < min ; i ++ ){
        fgets(buffer_1 , MAX_LENGTH , file_1) ; 
        fgets(buffer_2 , MAX_LENGTH , file_2) ; 
        if(strcmp(buffer_1 , buffer_2)){
            int point_to_word =  one_differ(buffer_1 , buffer_2) ;
            if(buffer_1[strlen(buffer_1) -1 ] == '\n' ) buffer_1[strlen(buffer_1) - 1] = buffer_1[strlen(buffer_1)] ; 
            if(buffer_2[strlen(buffer_2) -1 ] == '\n' ) buffer_2[strlen(buffer_2) - 1] = buffer_2[strlen(buffer_2)] ;
            if(point_to_word == -1) show_cmp1(buffer_1 , buffer_2 , i+1) ;  else
               show_cmp3(buffer_1  , buffer_2 , i+1 , point_to_word) ; 
        }    
    }
    for(int i = min ; i < max  ; i++ ){
        if(len1 == max)  fgets(buffer_1 , MAX_LENGTH , file_1) ; else fgets(buffer_1 , MAX_LENGTH , file_2) ; 
        if(buffer_1[strlen(buffer_1) -1 ] == '\n' ) buffer_1[strlen(buffer_1) - 1] = buffer_1[strlen(buffer_1)] ; 
        show_cmp2(buffer_1 , i + 1 , max) ; 
    }
    fclose(file_1) ;
    fclose(file_2) ;
 } 
void dir_tree(char * dirname , int depth , int limit  ) {
    if(limit == -1) limit = MAX_NUMBER ;
    if(limit < -1){
        if(Arman_activation == false) printf("INVALID DEPTH\n") ;
        if(Arman_activation == true) strcat(temp_save , "INVALID DEPTH") ;  
        return ; 
    } 
    chdir(dirname) ;
    if(depth <= limit){
    DIR* dir = opendir(".") ;
    struct dirent* entity  ; 
    entity = readdir(dir);
    while (entity != NULL) {
      int attr = GetFileAttributes(entity->d_name);
      if (((attr & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN) && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0 ) {
          for(int i = 0 ; i < depth - 1  ; i ++ ){
            if(Arman_activation == false) printf("    ") ; 
            if(Arman_activation == true ) strcat(temp_save , "    ") ; 
          }
          if(depth != 0){
          for(int i = 0 ; i < 4 ; i++){
            if(Arman_activation == false){
                if(i == 0 ) printf("%c" , 195) ;
                if(i != 0 ) printf("%c" , 196) ;
             }else{
                strcat(temp_save , "-") ; 
             }
           }
          }
           if(Arman_activation == false) printf("%s\n" , entity->d_name) ;
           if(Arman_activation == true ){
            strcat(temp_save , entity->d_name) ;
            strcat(temp_save , "\n") ;
           }    
           if (entity->d_type == DT_DIR ) {
            char path[100] = {0};
            strcat(path, ".");
            strcat(path, "/");
            strcat(path, entity->d_name);
            dir_tree(path , depth+1 , limit);
            chdir("..") ; 
         }
    }
      
    entity = readdir(dir);
    }
    closedir(dir);
  }
 }
void translate_string(char * string) {
    if(string[0] == '\"'){
        for(int i = 0 ; i < strlen(string) ; i++) {
            string[i] = string[i+1] ; 
        }
        string[strlen(string) - 1] = string[strlen(string)] ; 
    }

    for(int i = 0 ; i < strlen(string) ; i ++){
        if(string[i] == '\\'){
            if(string[i+1] == 'n'){
                string[i] = '\n' ; 
                int j = i+1  ;
                for(j ; j < strlen(string) ; j ++ ){
                    string[j] = string[j + 1] ; 
                } 
            }else{
                if(string[i+1] == '\\' , string[i+2] == 'n' ){
                    int j = i+1  ;
                    for(j ; j < strlen(string) ; j ++ ){
                    string[j] = string[j + 1] ; 
                    }     
                }
                i++  ; 
            }
        }
    }
 }
void create_dir(const char * address) {

    int slashes = 0 , i = 0  ; 
     while(1){
        if(address[i] == '\0') break ; 
        if(address[i] == '/') slashes ++ ;
        i ++ ; 
      } 
       if(slashes != 0 ){
        char * tree_path = (char * )calloc(MAX_LENGTH  ,  sizeof(char))  ; 
        for (int j = 0 ; ; j++) {
         if(address[j] == '/'){
           if(mkdir(tree_path) != -1) mkdir(tree_path) ; 
             slashes -- ; 
             if(slashes == 0 ) break ;  
         }
          tree_path[j] = address[j] ; 
     }
     free(tree_path) ; 
    }
 }

void translate_dir(char * address) {
    int k = 0 ; 
    if(address[0] == '\"') k ++ ; 
    if(address[0] == '/' || (address[0] == '\"' && address[1] =='/') ) k++ ;
    for(int i = 0 ; i < strlen(address)  ; i++ ){
    address[i] = address[i+k] ; 
   }
    if(address[strlen(address) - 1] == '\"' )address[strlen(address) - 1] = address[strlen(address)] ; 

 } 

void translate_to_word(char * address , int *pointer ) {
    translate_dir(address) ; 
    FILE * my_file = fopen(address , "r") ;
    char buffer_word[MAX_LENGTH] ;  
    char buffer_line[MAX_LENGTH];
    bool keep_reading = true ; 
    int words = 1  ; 
    int number_of_lines = 0 ; 
    while(keep_reading){
        int point = ftell(my_file) - number_of_lines ;
        fgets(buffer_line , MAX_LENGTH , my_file)  ;
        number_of_lines ++ ; 
        char * w = strtok(buffer_line , " ") ;
        if(strcmp(w , "\n") == 0 ) continue ; 
        while(1){
            if(point >= *pointer){
                if(point == *pointer) *pointer = words ; else
                *pointer = words-1 ; 
                keep_reading = false ; 
                break ; 
           }
         words ++ ; 
         if(w[strlen(w) - 1] != '\n') point += strlen(w) + 1 ; else 
         point += strlen(w) ; 
         w = strtok(NULL, " ") ;
         if(w == NULL || strcmp(w , "\n") == 0 || strcmp(w , " ") == 0) break ; 
        }
    }
  fclose(my_file) ; 
 }

int  lines_of_file(char * address  ) {
 FILE * my_file = fopen(address , "r") ; 
 int lines =  0 ;
 char get_line[MAX_LENGTH] ; 
  while(fgets(get_line , MAX_LENGTH , my_file) != NULL){
      lines ++  ;
  } 
     fclose(my_file) ;
     return lines  ; 
 }
bool generate_brace(char * address , int dif_line  , int start[2] , int end[2] ){
    translate_dir(address) ; 
    FILE * my_file = fopen(address , "r") ; 
    char buffer_line[MAX_LENGTH] ;  
    int start_line = 1 ; 
    int line_end = 1 ;
    int check = 0  ; 
    while(fgets(buffer_line , MAX_LENGTH , my_file) != NULL){

       if(start_line <= dif_line){
        start_line ++ ; 
       }else{
        if(check == 0){
           start[1] = first_brace_finder(buffer_line) ;
           if(start[1] != -1){
            start[0] = start_line ; 
            check = 1 ; 
            line_end = start_line ; 
           }else start_line ++ ; 
        }
        if(check != 0 ){
            if(line_end == start_line){
                for(int i = start[1]+1 ; i < strlen(buffer_line) ; i ++ ){
                    if(buffer_line[i] == '{') check ++ ;
                    if(buffer_line[i] == '}') check -- ; 
                    if(check == 0 ){
                        end[0] = line_end ; 
                        end[1] = i ;
                        check = 10000 ;  
                        break ; 
                    }
                }
            }else{
                end[1] = end_brace_finder(buffer_line , &check) ; 
                if(end[1] != -1){
                     end[0] = line_end ; 
                        check = 10000 ;  
                }
            }
            line_end ++ ; 
        }

        if(check == 10000){
            fclose(my_file) ;
            return true ; 
        }
      }
    }
    fclose(my_file) ; 
    return false ; 
 }

int first_brace_finder(char * string){
   for(int i = 0 ; i < strlen(string) ; i ++){
    if(string[i] =='{') return i ;
   }
   return -1 ; 
 }

int end_brace_finder(char * string  , int* check ) {

    for(int i = 0 ; i < strlen(string) ; i ++){
    if(string[i] =='}') (*check)-- ; 
    if(string[i] =='{') (*check)++ ; 
    if(*check == 0) return i ; 
   }
   return -1 ; 
 }
void correct_first_brace(char * address , int line , int byte , int* end ){
    translate_dir(address) ; 
    char address2[MAX_LENGTH] ={0} ; 
    strcpy(address2 , address) ; 
    strcat(address2 , "temp") ;
    FILE * my_file = fopen(address , "r") ; 
    FILE * temp_file = fopen(address2 , "w") ; 
    char buffer_line[MAX_LENGTH] ;
    int current_line = 1 ; 
 while(fgets(buffer_line , MAX_LENGTH , my_file) != NULL){
    if(current_line != line){
        fputs(buffer_line , temp_file) ; 
    }else{
        char before_brace[MAX_LENGTH] = {0}  ; 
        memcpy(before_brace , buffer_line  , byte+1 ) ;
        for(int x = byte-1 ; ; x-- ){
             if(x == 0 || x == -1){
            strcat(before_brace  , "\n") ;
            fputs(before_brace , temp_file) ;
            break ;
            }

            if(before_brace[x] != ' '){
                before_brace[x+1] = ' ' ; 
                before_brace[x+2] = '{' ;
                before_brace[x+3] = '\n' ;
                before_brace[x+4] = '\0' ; 
                fputs(before_brace , temp_file) ; 
                break ; 
            }
        }
        for(int i = byte+1 ; i < strlen(buffer_line) ; i++){
            if(i == byte+1 && buffer_line[i] == '\n') continue ; 
            fputc(buffer_line[i] , temp_file) ; 
            if(i == byte+1) (*end)++ ;
        } 
    }
    current_line ++ ; 
 }
 fclose(my_file) ;
 fclose(temp_file) ;
 remove(address) ;
 rename(address2 , address) ; 

 }
void correct_end_brace(char * address , int* line , int byte  ){ 
 translate_dir(address) ; 
    char address2[MAX_LENGTH] ={0} ; 
    strcpy(address2 , address) ; 
    strcat(address2 , "temp") ;
    FILE * my_file = fopen(address , "r") ; 
    FILE * temp_file = fopen(address2 , "w") ; 
    char buffer_line[MAX_LENGTH] ;
    int current_line = 1 ; 
 while(fgets(buffer_line , MAX_LENGTH , my_file) != NULL){
    if(current_line != *line){
        fputs(buffer_line , temp_file) ; 
    }else{
        if(byte != 0 ){
        char before_brace[MAX_LENGTH] ={0} ;
        memcpy(before_brace , buffer_line , byte) ;
        for(int x = byte - 1 ; ;x-- ){
            if(x == -1) break ;
            if(before_brace[x] != ' '){
              strcat(before_brace , "\n") ;
              fputs(before_brace , temp_file) ;
               (*line) ++ ; 
              current_line ++ ;
              break ;
           } 
        } 
      }
      char  brace_line[MAX_LENGTH] ={0} ;
      for(int j = 0 ; j < 1 ; j ++){
        brace_line[j] = ' ' ;
      }
      strcat(brace_line , "}\n") ; 
      fputs(brace_line , temp_file) ; 
      for(int z = byte + 1 ; z < strlen(buffer_line) ; z ++){
        if(z == byte+1 && buffer_line[z] == '\n') continue ; 
        fputc(buffer_line[z] , temp_file) ;
      }  
    }
    current_line ++ ; 
 }
 fclose(my_file) ;
 fclose(temp_file) ;
 remove(address) ;
 rename(address2 , address) ; 

 }

int arrange_by_space(char * address , int start , int end ) {
     translate_dir(address) ; 
    char address2[MAX_LENGTH] ={0} ; 
    strcpy(address2 , address) ; 
    strcat(address2 , "temp") ;
    FILE * my_file = fopen(address , "r") ; 
    FILE * temp_file = fopen(address2 , "w") ; 
     int current_line = 1 ;
     int handle_size = 4 ;  
     char buffer_line[MAX_LENGTH] ; 
     for(int i = 0 ; i < start ; i++){
       fgets(buffer_line , MAX_LENGTH , my_file) ; 
       fputs(buffer_line , temp_file) ;
        current_line ++ ;
     }
     for(int i = 0 ;  ; i ++ ){
        if(buffer_line[i] != ' '){
             handle_size += i ;
             break ;
       }
     }
     char * pattern = (char *)calloc((handle_size + 2)  , sizeof(char))  ; 
     for(int j = 0 ; j < handle_size ; j ++) pattern[j] = ' ' ;
     int printing_letter = 0 ; 
     for(int i = 0 ; i < end - start - 1 ; i ++  ){
        fgets(buffer_line , MAX_LENGTH  , my_file) ;
        char * newline = (char * )calloc(MAX_LENGTH  ,  sizeof(char)) ; 
        printing_letter = start_printing(buffer_line) ; 
        strcpy(newline , pattern) ; 
        for(int x = 0 ; printing_letter + x < strlen(buffer_line) ; x ++){
            newline[x + handle_size] = buffer_line[printing_letter + x] ; 
        }
        fputs(newline , temp_file) ; 
        current_line ++ ;  
        free(newline) ;     
     }
     pattern[strlen(pattern) - 4] = '\0' ; 

     fgets(buffer_line , MAX_LENGTH , my_file) ; 
     char * newline = (char * )malloc(MAX_LENGTH * sizeof(char)) ; 
       if(handle_size > 4){ 
        strcpy(newline , pattern) ;
        strcat(newline , "}\n"); 
       }else strcpy(newline , "}\n") ; 
        fputs(newline , temp_file) ;
        free(newline) ; 
        while(fgets(buffer_line , MAX_LENGTH , my_file) != NULL){
            fputs(buffer_line , temp_file) ; 
        }
    fclose(temp_file) ;
    fclose(my_file) ; 
    remove(address) ; 
    rename(address2 , address) ; 
 }
int start_printing(char * string) {

    for(int i = 0 ; i < strlen(string) ; i ++){
        if(string[i] != ' ') return i ; 
    }
 }

void show_cmp1(char * a , char * b  , int m ){ 
  if(Arman_activation == false){
    printf("============ #%d ============\n" , m) ;
    printf("%s\n%s\n" , a , b ) ; 
  } 
  if(Arman_activation == true){
    char word[15] ; 
    sprintf(word  , "%d" , m) ; 
    strcat(temp_save , "============ #" ) ; 
    strcat(temp_save , word) ; 
    strcat(temp_save ,  " ============\n") ; 
    strcat(temp_save , a) ; 
    strcat(temp_save , "\n") ;
    strcat(temp_save ,b) ;
    strcat(temp_save , "\n") ; 
   }
 }

void show_cmp2(char * a  , int n  ,  int m ) {

    if(Arman_activation == false){
        printf(">>>>>>>>>> #%d - #%d >>>>>>>>>>\n" , n , m) ;
        printf("%s\n" , a) ;
    }
    if(Arman_activation == true){
        char word[15] ; 
        sprintf(word  , "%d" , n) ; 
        strcat(temp_save , ">>>>>>>>>> #" ) ; 
        strcat(temp_save , word) ; 
        sprintf(word  , "%d" , m) ; 
        strcat(temp_save ,  " - #") ; 
        strcat(temp_save , word) ; 
        strcat(temp_save , " >>>>>>>>>>\n") ;
        strcat(temp_save ,a) ;
        strcat(temp_save , "\n") ; 
    }
 }
void show_cmp3(char * a , char * b , int m , int differ){
    char line_1[MAX_LENGTH] = {0} ; 
    char line_2[MAX_LENGTH] = {0} ;
    char tok[2] = " " ;
    char * temp ;
    temp =  strtok(a , tok) ;
    int counter = 0 ; 
    while(temp != NULL){
        counter ++ ; 
        if(counter == differ){
            strcat(line_1 , ">>") ;
            strcat(line_1 , temp) ;
            strcat(line_1 , "<< ") ;
        }else{
            strcat(line_1 , temp) ; 
            strcat(line_1 , " ") ;
        }
        temp = strtok(NULL , tok) ;
    }
    temp = strtok(b , tok) ;
    counter = 0 ; 
    while(temp != NULL){
        counter ++ ; 
        if(counter == differ){
            strcat(line_2 , ">>") ;
            strcat(line_2 , temp) ;
            strcat(line_2 , "<< ") ;
        }else{
            strcat(line_2 , temp) ; 
            strcat(line_2 , " ") ;
        }
    temp = strtok(NULL , tok) ;
    }
    if(Arman_activation == false){
        printf("============ #%d ============\n" , m) ;
        printf("%s\n%s\n"  , line_1  , line_2) ;   
    } 
    if(Arman_activation == true){
        char word[15] ; 
        sprintf(word  , "%d" , m) ; 
        strcat(temp_save , "============ #" ) ; 
        strcat(temp_save , word) ; 
        strcat(temp_save ,  " ============\n") ; 
        strcat(temp_save , line_1) ; 
        strcat(temp_save , "\n") ;
        strcat(temp_save ,line_2) ;
        strcat(temp_save , "\n") ; 
   }
 } 

int line_counter(char * address) {

    translate_dir(address) ; 
    FILE * my_file = fopen(address , "r") ; 
    int l = 0 ; 
    char C_line[MAX_LENGTH] ; 
    while(fgets(C_line  , MAX_LENGTH , my_file) != NULL){
        l ++ ; 
    }
    fclose(my_file) ; 
    return l ; 
 }

int one_differ(char * string1 , char * string2) {
    char S_use_1[MAX_LENGTH] ; 
    char S_use_2[MAX_LENGTH] ;
    strcpy(S_use_1 , string1) ; 
    strcpy(S_use_2 , string2) ;
    int n = 0 ;
    int pnt = 0 ; 
    int size_1 = 0 ; 
    int size_2 = 0 ;
    char tok[2] = " " ; 
    char * use_1 ; 
    char * use_2 ;
    char  words_1[MAX_LINE][MAX_LENGTH] ;
    char  words_2[MAX_LINE][MAX_LENGTH] ; 
    use_1 = strtok(S_use_1 , tok) ; 
    while(use_1 != NULL) {
        strcpy(words_1[size_1] , use_1) ; 
        size_1 ++ ; 
        use_1 = strtok(NULL  , tok) ; 
    }

    use_2 = strtok(S_use_2 , tok) ; 
    while(use_2 != NULL) {
        strcpy(words_2[size_2] , use_2) ; 
        size_2 ++ ; 
        use_2 = strtok(NULL  , tok) ; 
    }

    if(size_1 != size_2 ) return -1 ; 
    for(int i = 0 ; i < size_1 ; i ++){
        if(strcmp(words_1[i] , words_2[i])) {
            n ++ ; 
            pnt = i+1 ; 
        }
        if(n >= 2) return -1 ;
    }
    return pnt ; 
 }

void wildcard_fixer(char * a ){
    char temp_a[MAX_LENGTH] = {0} ; 
    int current = 0 ;
    bool first_end = false  ;
    for(int i = 0 ; i < strlen(a) ; i ++ ){
        first_end = false ; 
        if(i == 0 && a[i] == '*' )first_end = true ; 
        if(a[i] == '*'){
            while(a[i+1] == '*'){
                i ++ ; 
            }
        }
        if(i == strlen(a) - 1  && a[i] == '*' && a[i-1] != '\\') first_end = true ; 
        if(first_end == false) temp_a[current] = a[i] ;
        if(first_end == false ) current ++ ;
    }
    strcpy(a , temp_a) ; 

 }
bool Search_Commands(char * string){
    for(int i = 0 ; i < 14 ; i ++) {
        if(strcmp(string , COMMANDS[i]) == 0) return true ; 
    }
    return false ;
 }

bool Dir_exist(char * address) {
    int slash = 0 ;
  for(int i = 0 ; i < strlen(address) ; i++){
      if(address[i] == '/' ) slash ++ ; 
  }
  if(slash == 0) return true ; 
  int temp_slash = slash ; 
   char __dirname[MAX_LENGTH] = {0} ; 
   for(int i = 0 ; ; i ++){
    if(address[i] == '/') slash -- ; 
    if(slash == 0) break ; 
    __dirname[i] = address[i] ; 
   }
   if(chdir(__dirname) == 0 ) {
        for(int i = 0 ; i < temp_slash ; i ++){
            chdir("..") ; 
        }
   }else return false ;
   return true ; 
 }

bool In_commandline(char * string  , char * pattern) {
    for(int i =  0 ; i < strlen(string) ; i ++) {
        for(int j = i ; j < strlen(pattern) + i ; j ++ ){
            if(pattern[j - i] != string[j] ) break ;
            if(j == strlen(pattern) + i - 1) return true ; 
        }
    }

    return false ; 
 }


void rec_wildcard(char * string){

    if(string[0] == '*') Wildcard.first_multichar = true ; else Wildcard.first_multichar = false ; 
    if(string[strlen(string) - 1] == '*' && string[strlen(string) - 2] != '\\') Wildcard.end_multichar = true ;  else  Wildcard.end_multichar = false ;
    for(int i = 1  ; i < strlen(string) ; i ++){
        if(string[i] == '*' && string[i-1] != '\\'){
         Wildcard.mode = true ; 
         return  ;  
        }
    }
    if(Wildcard.first_multichar == true ) Wildcard.mode = true  ;else Wildcard.mode  = false ; 
 } 

