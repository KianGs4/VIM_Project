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
#define MAX_FILES 10 
int current_command = 0 ;

void create_dir(const char * address) ;
void translate_dir(char * address) ; 
void translate_string(char * string) ;
void translate_to_word(char * address , int *pointer ) ; 

int first_brace_finder(char * string) ; 
int end_brace_finder(char * string , int* check) ; 
bool generate_brace(char * address , int def , int  start[2] , int end[2]) ; 
void correct_first_brace(char * address , int line , int byte , int* end  ) ; 
void correct_end_brace(char * address , int*line , int byte  ) ; 
int arrangr_by_space(char * string , int start , int end) ; 
int start_printing(char * string) ;

void createfile(char * address) ; 
void insertstr(char * address  , char * string  , int line  , int byte ) ; 
void cat(char * address) ; 
void remove_str(char * address , int line ,  int byte , int size , int ward) ; 
void copystr(char * address , int line  , int byte , int size , int ward ) ; 
void cutstr(char * address , int line  , int byte , int size , int ward) ; 
void paste_str(char * address , int line , int byte) ; 
void find_str( char * address , char * string)  ;
void replace_str(char * address , char * string1 , char * string2  ) ; 
void grep_str( int num_file  , char files[MAX_FILES][MAX_LENGTH]  , char * string) ; 
void closing_pair(char * address  ) ; 


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


int main(){ 
    // opt_grep.c = false  ; 
    // opt_grep.l = false ; 
    // char * string ; 
    // char  address[MAX_FILES][MAX_LENGTH] ; 
    // scanf("%s" , string) ;
    // scanf("%s" , address[0]) ; 
    // scanf("%s" , address[1]) ; 
    // scanf("%s" , address[2]) ;
    // grep_str(3 , address , string) ; 
    // char address[MAX_LENGTH] = "file.txt" ; 
    // char * string = "add" ; 
    // insertstr(address , string  , 1 , 0 ) ;
    char address[MAX_LENGTH] ; 
    int start[2] ; 
    int  end[2] ; 
    scanf("%s" , address) ; 
    //printf("%d" , generate_brace(address , -1 , start  , end ) ) ; 
    closing_pair(address) ;
    return 0 ; 
}


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
   rename(address2 , address) ; 
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

void grep_str(int num_file  , char files[MAX_FILES][MAX_LENGTH]  , char * string) {
    int lines_with_word = 0 ; 
    for(int i = 0 ; i < num_file ; i ++ ){
        FILE * my_file = fopen(files[i] , "r") ;
        bool keep_reading = true ; 
        char buffer_line[MAX_LENGTH] ; 
        int current_letter = 0 ;
        int size = strlen(string) ;
        while(fgets(buffer_line , MAX_LENGTH , my_file) != NULL && keep_reading == true ){
            for(int j = 0 ; j < strlen(buffer_line) ; j ++ ){
                if(current_letter == size){
                    lines_with_word ++ ;
                    current_letter = 0 ;  
                    if(opt_grep.c != true){
                        if(opt_grep.l != true){
                         printf("%s: %s" , files[i]  , buffer_line ) ;
                            if(buffer_line[strlen(buffer_line) - 1] != '\n') printf("\n") ; 
                            }else{
                            printf("%s\n"  , files[i]) ; 
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
    if(opt_grep.c) printf("%d\n" , lines_with_word) ;
 } 
void closing_pair(char * address ) {
     int dif_line = -1 ; 
     int start[2] , end[2] ;
     while(generate_brace(address , dif_line , start , end)) {
          correct_end_brace(address , &end[0] , end[1])  ;
          correct_first_brace(address , start[0] , start[1] , &end[0]) ;
          arrangr_by_space(address , start[0] , end[0]) ;
         
         dif_line = start[0] ; 
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

int arrangr_by_space(char * address , int start , int end ) {
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