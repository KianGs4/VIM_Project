// kian ghassemi 401102264 ; 

#include <stdio.h>
#include <sys\stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define MAX_LENGTH  102
#define MAX_LINE 200 
#define Empty_line  "#@!$$$*KIAN_GSVIM$$$!@#"
#define MAX_FINDCASE 1000 

void createdir(const char * address) ;
void translate_dir(char * address) ; 
void translate_string(char * string) ;
void translate_to_word(char * address , int *pointer ) ; 

void createfile(char * address) ; 
void insertstr(char * address  , char * string  , int line  , int byte ) ; 
void cat(char * address) ; 
void remove_str(char * address , int line ,  int byte , int size , int ward) ; 
void copystr(char * address , int line  , int byte , int size , int ward ) ; 
void cutstr(char * address , int line  , int byte , int size , int ward) ; 
void paste_str(char * address , int line , int byte) ; 
void find_str( char * address , char * string)  ;
void replace_str(char * address , char * string1 , char * string2  ) ; 

struct option{
    bool byword ; 
    int at ; 
    bool count ; 
    bool all ;  
}opt_find ; 



int main(){ 
    opt_find.byword = false    ;
    opt_find.at = 0 ; 
    opt_find.count = false ; 
    opt_find.all = true  ; 
    char * a ;
    char string[100]  ;
   // int b = 149 ;
    scanf("%[^\n]s" , string) ;
    scanf("%s"  , a) ; 
    char string2[100] = "[?]"    ; 
  //  replace_str(a  , string  , string2) ; 
   //find_str(a , string)  ;
   remove_str(a  , 2 , 0 , 6 , 1) ;
    return 0 ; 
}


void createfile(char * address){
    translate_dir(address) ; 
    createdir(address)  ;    
    if (access(address , F_OK) == 0) {
        printf("file already exists\n") ; 
    }else{
    FILE*  my_file = fopen( address  , "w") ; 
    fclose(my_file) ;        
    } 
 }

void insertstr(char * address  , char * string  , int line  , int byte ) {
 translate_dir(address) ; 
 char address2[MAX_LENGTH] ; 
 strcpy(address2 , address) ; 
 strcat(address2 , "temp") ; 
 FILE * my_file = fopen(address , "r") ; 
 FILE * temp_file = fopen(address2 , "w") ; 
 char newline[MAX_LENGTH] = {0} ; 
 translate_string(string) ; 
 char buffer_line[MAX_LENGTH] ; 
 int current_line = 1 ;
 bool ISend = true ;   
 bool keep_reading = true ; 
 while(keep_reading == true){
        fgets(buffer_line , MAX_LENGTH , my_file) ;    
        if(feof(my_file)){
        if(current_line < line ) {
            if(ISend && strcmp(buffer_line , "\n")!= 0){
                fputs(buffer_line , temp_file)  ; 
                ISend = false ; 
            }
            fputs("\n" , temp_file ) ;
            }
        if(current_line == line){
            for(int i = 0 ; i < byte ; i ++ ) newline[i] = ' ' ; 
            strcat(newline , string ) ; 
            fputs(newline , temp_file) ; 
            keep_reading = false ;
        }
        if(current_line > line ){
            keep_reading = false ; 
            if(strcmp(buffer_line , "\n") != 0 )fputs(buffer_line , temp_file) ; 
        } 
    }else{
        if(current_line == line){
           if(byte > strlen(buffer_line) - 1 ){
            buffer_line[strlen(buffer_line) - 1 ] = buffer_line[strlen(buffer_line)] ; 
            strcpy(newline , buffer_line) ; 
            for(int i = 0 ; i < byte - strlen(buffer_line)   ; i ++ ) strcat(newline , " ")  ; 
            strcat(newline , string) ; 
            strcat(newline , "\n") ; 
            fputs(newline , temp_file) ; 
           }else{
            memcpy(newline  , buffer_line , byte) ; 
            strcat(newline , string) ;
            int temp_size = strlen(newline) ;  
            for(int i  = 0 ; i+byte < strlen(buffer_line) ; i++){
                newline[temp_size + i] = buffer_line[i + byte] ; 
            }
            fputs(newline , temp_file) ; 
           }
        }else{
            fputs(buffer_line , temp_file) ; 
        }
    }
    current_line ++ ; 
   }
 fclose(my_file) ; 
 fclose(temp_file) ;
   remove(address) ; 
   rename(address2 , address ) ; 
 }
 
void cat(char * address) {
    translate_dir(address) ; 
    FILE * my_file = fopen(address , "r") ; 
    char buffer_line[MAX_LENGTH] ; 
    bool keep_reading = true ; 
    while(keep_reading == true){
        fgets(buffer_line , MAX_LENGTH , my_file) ; 
        printf("%s" , buffer_line) ;
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
     printf("%s" , buffer_line[k]) ; 
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


void copystr(char * address , int line  , int byte , int size , int ward ) {
    translate_dir(address) ; 
    char readfile[MAX_LENGTH] = {0} ; 
    char buffer_line[MAX_LENGTH] ; 
     FILE * my_file = fopen(address  , "r+") ;
     for(int i = 0 ; i < line - 1 ; i ++ ){
        fgets(buffer_line , MAX_LENGTH  , my_file) ; 
     }
     int of_the_line = ftell(my_file) ; 

     if(ward == -1){
        of_the_line -= size ; 
     }

     fseek(my_file , of_the_line +  byte-1  , SEEK_SET)  ;
     fread( readfile , 1 , size  , my_file) ; 
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

void cutstr(char * address , int line , int byte , int size ,  int ward){
    copystr(address , line , byte , size , ward) ; 
    remove_str(address , line , byte , size , ward) ; 
 }
void paste_str(char * address , int line , int byte ){
    char string[MAX_LENGTH] ; 
    HANDLE h;
    OpenClipboard(NULL);
    h = GetClipboardData(CF_TEXT);
    strcpy(string , (char *)h) ; 
    CloseClipboard();
   insertstr(address , string , line , byte ) ; 
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
    while(keep_reading){
        int pointer = ftell(my_file) - line  ;  
        fgets(buffer_line  , MAX_LENGTH , my_file) ; 
        line ++ ; 
        int current_letter = 0  ; 
        for(int i = 0 ; i < strlen(buffer_line) ; i ++ ){
           if(current_letter == size ){
            if(opt_find.count != true ) findcase[cnt] = pointer ; 
            cnt ++ ; 
            current_letter = 0 ;
            pointer += size ;  
            if( (opt_find.all != true && opt_find.at == 0 ) || opt_find.at == cnt ){
                keep_reading  = false ; 
                 break ;                
             } 
           }
           if(string[current_letter] != buffer_line[i]){
            pointer += current_letter+1 ;
            current_letter = 0  ;
           }else{
            current_letter ++ ; 
           }
        }
        if(feof(my_file)) break ;  
    }
    //scetion 2 : show 
    if(opt_find.count){
        printf("%d\n" , cnt ) ; 
    }else{
        if(cnt == 0) printf("%d\n" , -1) ; else {
            if(opt_find.byword){
                for(int i = 0 ; i < cnt ; i ++){
                    translate_to_word(address  , &findcase[i]) ; 
                }
            }
        if(opt_find.all){
            for(int i = 0 ; i < cnt - 1 ; i ++){
                printf("%d,  " , findcase[i]) ; 
            }
            printf("%d\n" , findcase[cnt - 1]) ; 
        }else{
            if(opt_find.at != 0){
                printf("%d\n" , findcase[cnt - 1]) ;
            }else{
                printf("%d\n" , findcase[0]) ; 
            }
        } 
    }
  }
     fclose(my_file) ; 
 }  

void replace_str(char * address , char * string1 , char * string2  ){
     int findcase[MAX_FINDCASE][2]  ;
    int cnt = 0 ; 
    int size = strlen(string1) ;
    translate_dir(address) ; 
    translate_string(string1); 
    FILE * my_file = fopen(address , "r") ; 
    char buffer_line[MAX_LENGTH] ;
    bool keep_reading = true  ;  
    int line = 0 ;
    int byte ;  
    //section  1 : find  
    while(keep_reading){
        int pointer = ftell(my_file) ;
        byte = pointer ;  
        fgets(buffer_line  , MAX_LENGTH , my_file) ; 
        line ++ ;
        int current_letter = 0  ; 
        for(int i = 0 ; i < strlen(buffer_line) ; i ++ ){
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
           if(string1[current_letter] != buffer_line[i]){
            pointer += current_letter+1 ;
            current_letter = 0  ;
           }else{
            current_letter ++ ; 
           }
        }
        if(feof(my_file)) break ;  
    }
    fclose(my_file) ; 
    //section 2 : replace
    if(opt_find.at == 0 ){
    for(int i = 0 ; i < cnt ; i ++  ){
        printf("%d : %d \t %d\n" , findcase[i][0] , findcase[i][1] , cnt) ; 
        remove_str(address , findcase[i][0] , findcase[i][1]  , strlen(string1) , 1  ) ; 
        insertstr(address ,  string2  ,  findcase[i][0] , findcase[i][1]) ; 
     }
    }else{
          remove_str(address , findcase[cnt - 1][0] , findcase[cnt - 1 ][1]  , strlen(string1) , 1  ) ; 
        insertstr(address ,  string2  ,  findcase[cnt - 1 ][0] , findcase[cnt - 1 ][1]) ; 
    }

 } 
void translate_string(char * string) {
    if(string[0] == '\"'){
        for(int i = 0 ; i < strlen(string) ; i++) {
            string[i] = string[i+1] ; 
        }
        string[strlen(string) - 1] = string[strlen(string)] ; 
    }
 }
void createdir(const char * address) {

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
   if(k == 2) address[strlen(address) - 1] = address[strlen(address)] ; 

 } 

void translate_to_word(char * address , int *pointer ) {

    translate_dir(address) ; 
    FILE * my_file = fopen(address , "r") ;
    char buffer_word[MAX_LENGTH] ;  
    char buffer_line[MAX_LENGTH];
    bool keep_reading = true ; 
    int words = 1  ; 
    while(keep_reading){
        int point = ftell(my_file) ;
        fgets(buffer_line , MAX_LENGTH , my_file)  ;
        char * w = strtok(buffer_line , " ") ;
        while(1){
            if(point >= *pointer){
                if(point == *pointer) *pointer = words ; else
               *pointer = words-1 ; 
                keep_reading = false ; 
            break ; 
           }
         words ++ ; 
         point += strlen(w) + 1 ;
         w = strtok(NULL, " ") ; 
         if(w == NULL) break ; 
        }
    }
  fclose(my_file) ; 
 }



