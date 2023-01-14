// kian ghassemi 401102264 ; 

#include <stdio.h>
#include <sys\stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_length  102
#define MAX_LINE 200 
#define Empty_line  "#@!$$$*KIAN_GSVIM$$$!@#"
void createdir(const char * address) ;
void translate_dir(char * address) ; 
void translate_string(char * string) ;

void createfile(char * address) ; 
void insertstr(char * address  , char * string  , int line  , int byte ) ; 
void cat(char * address) ; 
void removestr(char * address , int line ,  int byte , int size , int ward) ; 

int main(){ 
    char a[100] ; 
    int l , b , s ;
    scanf("%s%d%d%d" , a , &l , &b , &s ) ; 
    removestr(a , l , b , s , 1 ) ; 
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
 char address2[MAX_length] ; 
 strcpy(address2 , address) ; 
 strcat(address2 , "temp") ; 
 FILE * my_file = fopen(address , "r") ; 
 FILE * temp_file = fopen(address2 , "w") ; 
 char newline[MAX_length] = {0} ; 
 for(int i = 0 ; i < byte ; i ++ ){
    newline[i] = ' ' ; 
 }
 translate_string(string) ; 
 strcat(newline , string ) ; 
 char buffer[MAX_length] ; 
 int current_line = 1 ;  
 bool keep_reading = true ; 
 while(keep_reading == true){
    fgets( buffer , MAX_length , my_file )  ; 
    if(feof(my_file)){
        if(current_line == line){
            fputs(newline , temp_file) ; 
            keep_reading = false ;
        }
        if(current_line > line ) keep_reading = false ;
        if(current_line < line ) fputs("\n" , temp_file ) ;  
    }else{
        if(current_line == line){
            strcat(newline , buffer) ; 
            fputs(newline , temp_file) ; 
        }else{
            fputs(buffer , temp_file) ; 
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
    FILE * my_file = fopen(address , "r") ; 
    char buffer[MAX_length] ; 
    bool keep_reading = true ; 
    while(keep_reading == true){
        fgets(buffer , MAX_length , my_file) ; 
        printf("%s" , buffer) ;
        if(feof(my_file)) keep_reading = false ;  
    }


 }


void removestr(char * address , int line ,  int byte , int size , int ward  ) {
    FILE * my_file = fopen(address  , "r+") ; 
    char address2[MAX_length] ; 
    strcpy(address2 , address) ; 
    strcat(address2 , "temp") ; 
    FILE * temp_file = fopen(address2 , "w") ; 
   //section 1 : 
    char ** buffer = (char **)malloc(MAX_LINE * sizeof(char *)) ; 
    int k = 0 ; 
    bool keep_reading = true ; 
    while(keep_reading){
     *(buffer + k) = malloc(MAX_length * sizeof(char)) ; 
     fgets(buffer[k] , MAX_length , my_file) ; 
     if(feof(my_file)) keep_reading = false ; 
     k ++ ; 
    }

    //section 2 :
    int pointer_byte = byte-1 ;
    int start = pointer_byte ; 
    int pointer_line = line - 1 ; 
    while(1){
         start += ward ;
         size -- ;   
         if(start == -2 ){
            char temp_buffer[MAX_length] = {0} ; 
            for(int i = 0 ; ; i ++){
                if(ward == -1){
                if(buffer[pointer_line][i + pointer_byte +1 ] == '\0') break ; 
                temp_buffer[i] = buffer[pointer_line][i+pointer_byte + 1 ] ;
                }
            }
           pointer_line -- ; 
           pointer_byte = strlen(buffer[pointer_line]) - 1 ; 
           start = pointer_byte ;
           buffer[pointer_line][strlen(buffer[pointer_line]) - 1] =  buffer[pointer_line][strlen(buffer[pointer_line])] ; 
           strcat(buffer[pointer_line] , temp_buffer) ; 
           strcpy(buffer[pointer_line + 1 ] , Empty_line) ; 
         }
         
         if(start ==  (strlen(buffer[pointer_line]))){
            start -- ; 
            buffer[pointer_line][strlen(buffer[pointer_line]) - 1] = buffer[pointer_line][strlen(buffer[pointer_line])] ; 
            strcat(buffer[pointer_line] , buffer[pointer_line + 1]) ; 
            for(int i = pointer_line + 1 ; i < k - 1 ; i ++ ) {
            strcpy(buffer[i] , buffer[i+1]) ; 
            }
            k -- ; 
         }

         if(size == 0 ){
                char temp_buffer[MAX_length] = {0} ; 
            if(ward == -1){
            for(int i = 0 ; i <= start ; i ++ ){
                temp_buffer[i] = buffer[pointer_line][i] ; 
            }

            int distance = pointer_byte   - start ;
            for(int i = start+1 ; ; i ++ ){
              if(buffer[pointer_line][i + distance] == '\0') break ;
              temp_buffer[i] = buffer[pointer_line][i + distance] ;  
            }
         
         strcpy(buffer[pointer_line] , temp_buffer) ;
         break ;  
           }else{
            for(int i = 0 ; i < pointer_byte ; i++){
                temp_buffer[i] = buffer[pointer_line][i] ; 
             }
            for(int i = 0  ; i < strlen(buffer[pointer_line]) - start ; i ++  ){
            temp_buffer[i + pointer_byte] = buffer[pointer_line][start + i] ; 
            }
            strcpy(buffer[pointer_line] , temp_buffer) ; 
            break ; 
      }
    }
}
    //add to file 
   for(int j = 0 ; j < k ; j ++ ){
    if(strcmp(buffer[j] , Empty_line) != 0 ) {
            fputs(buffer[j] , temp_file) ;
    }
   }
   fclose(my_file) ; 
   fclose(temp_file) ; 
   remove(address) ; 
   rename(address2 , address ) ; 
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
        char * tree_path = (char * )calloc(MAX_length  ,  sizeof(char))  ; 
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



