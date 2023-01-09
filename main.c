#include <stdio.h>
#include <sys\stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define MAX_lengh  100

void createdir(const char * address) ;
void using_path(char * address) ; 


void createfile(char * address){
    using_path(address) ; 
    createdir(address)  ;    
    FILE*  my_file = fopen( address  , "w") ; 
    fclose(my_file) ;         
}



int main(){
//    char add[100] ; 
//    scanf("%s" , add) ; 
//    createfile(add) ;  
    

}



void createdir(const char * address) {

    int slashes = 0 , i = 0  ; 
     while(1){
        if(address[i] == '\0') break ; 
        if(address[i] == '/') slashes ++ ;
        i ++ ; 
     }
        char tree_path[MAX_lengh]  ; 
        for (int j = 0 ; ; j++) {
         if(address[j] == '/'){
          if(mkdir(tree_path) != -1) mkdir(tree_path) ; 
             slashes -- ; 
             if(slashes == 0 ) return ;  
         }
          tree_path[j] = address[j] ; 

     }
     
}
void using_path(char * address) {

    for(int i = 0 ; i < strlen(address)  ; i++ ){
    address[i] = address[i+1] ; 
   }
} 
