// kian ghassemi 401102264 ; 

#include <stdio.h>
#include <sys\stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_length  102

void createdir(const char * address) ;
void translate_dir(char * address) ; 
void translate_string(char * string) ; 
void createfile(char * address) ; 
void insertstr(char * address  , char * string  , int line  , int byte ) ; 


int main(){ 
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
 FILE * my_file = fopen(address , "r") ; 
 FILE * temp_file = fopen("temp.txt" , "w") ; 
 char newline[MAX_length] ; 
 for(int i = 0 ; i < byte ; i ++ ){
    newline[i] = ' ' ; 
 }
 translate_string(string) ; 
 strcat(newline , string ) ; 
 char buffer[MAX_length] ; 
 int current_line = 1 ;  
 bool keep_reading = true ; 
 while(keep_reading == true){
    fgets( buffer , MAX_length , my_file ) ; 
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
    int k = 1 ; 
    if(address[0] == '\"') k ++ ; 

    for(int i = 0 ; i < strlen(address)  ; i++ ){
    address[i] = address[i+k] ; 
   }
   if(k == 2) address[strlen(address) - 1] = address[strlen(address)] ; 

 } 
