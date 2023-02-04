#include <curses.h>
#include <string.h>
#include <sys\stat.h>
#include <stdbool.h>
#include "source.c" 

#define ctl(x) ((x) & 0x1f) 


char blank[MAX_LENGTH] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" ; 
char clipborad[MAX_SIZE] ;
char buffer[MAX_LINE][MAX_LENGTH] ; 
int number_of_line = 0 ; 
char filename[MAX_LENGTH]= {0}; 
char dirname[MAX_LENGTH] = {0} ;
int row , col ;


int get_digit(int x) ; 
void show() ; 
void init_1(char * string)  ; 
int main(int argc, char *argv[]){

  initscr();
  raw() ;
  keypad(stdscr, TRUE) ;
  noecho();  			
	start_color();	
  int x = 0 ;
  int y = 5 ; 
  int line_start , byte_start , line_end , byte_end ; 
  char mode[3][100] = {"NORMAL\0" , "INSERT\0" , "VISUAL\0"} ; 
  int mode_counter = 0 ;  
  getmaxyx(stdscr,row,col);
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2 , COLOR_WHITE , COLOR_RED) ;
  show(argv[1]) ; 
  init_1(argv[1]) ;
  char save = '+' ; 
  mvprintw(number_of_line + 5   , 0 , "%s   %s %c" , mode , filename , save) ;
	mvchgat(number_of_line + 5 , 0, -1, A_BLINK, 1, NULL);
	refresh();
  char ch = 'i' ;
  bool keep_reading = true ; 
  while(keep_reading){
   ch = getch();
  if(ch == 'i'){
      mode_counter += 1 ;
      mode_counter %= 3 ; 
      mvprintw(number_of_line + 5  , 0 , "%s   %s %c" , mode[mode_counter] , filename , save) ;  
      mvchgat(number_of_line  + 5 , 0, -1, A_BLINK, 1, NULL);
      line_start = -1; 
      byte_end   = -1 ; 
      byte_start = -1 ;
      line_end   = -1 ; 
      number_of_line = 0 ;
      show(argv[1]) ;
     }
  if(strcmp(mode[mode_counter] , "VISUAL\0") == 0 ){
      int digit = get_digit(x+1) ; 
      if(line_start == -1 && byte_start == -1){
        line_start = x+1 ; 
        byte_start = y - 5  - digit ; 
        line_end = line_start ; 
        byte_end = byte_start;
      }else{
        if(x+1 > line_start || ((x+1 == line_start) && (y - 5 - digit >= byte_start))){
          line_end = x+1 ; 
          byte_end = y - 5 - digit; 
        }else{
          line_end = line_start ; 
          byte_end = byte_start;
          line_start = x+1 ; 
          byte_start = y - 5 - digit;           
        }
      }
      if(line_start != -1 ){
        attron(COLOR_PAIR(2)) ; 
        int start = 0 ; 
        if(byte_start < 0) byte_start = 0 ; 
        for(int i = line_start ; i <= line_end ; i ++){
          if(i == line_start) start = byte_start  ; else start = 0 ; 
          for(int j = start  ; j < strlen(buffer[i-1]) ; j++){
            if(i == line_end){
              if(j == byte_end) break ;
            }
            mvprintw(i-1 , j + 5 + get_digit(i) , "%c" , buffer[i-1][j] ) ; 
          }
        }
        move(x , y) ;
        refresh() ;
        attroff(COLOR_PAIR(2)) ; 
      }
     } 
  if(ch == ctl('x')) break ; 
  if(ch == ':'){
      char string[MAX_LENGTH] ;
      char string_use[MAX_LENGTH] ; 
      mvprintw(number_of_line + 6 , 0 , ":") ; 
      echo() ; 
      save = '+' ; 
      getstr(string) ; 
      char tok[2] = " " ; 
      strcpy(string_use , string) ; 
      char * command = strtok(string , tok) ; 
      if(strcmp(command, "save") == 0 ) save = ' ' ;
      
      if(strcmp(command , "saveas") == 0 ){
        command = strtok(NULL  , tok) ; 
        save = ' ' ; 
        char temp_add[MAX_LENGTH] = {0}  ;
        strcpy(temp_add , dirname) ; 
        strcpy(filename , command) ; 
        strcat(temp_add , filename) ;
        rename(argv[1] ,temp_add ) ; 
        strcpy(argv[1] , temp_add) ;
      }

      if(strcmp(command , "open") ==  0){
        command = strtok(NULL , tok) ;
        FILE * use = fopen(command , "r") ; 
        if(use !=  NULL){
            erase() ; 
            number_of_line = 0 ;
            show(command) ; 
            init_1(command) ; 
            strcpy(argv[1] , command) ;  
            save = '+' ; 
            fclose(use) ;             
        }else{
          mvprintw(number_of_line+6 , 0  , "INVALID FILE NAME ") ; 
          getch() ; 
        }
      }

      if(strcmp(command ,  "delete") == 0  || strcmp(command , "cut") == 0 || strcmp(command , "d") == 0  || strcmp(command , "y") == 0 || strcmp(command , "copy") == 0){
          mode_counter = 0 ; 
          if(line_start != -1){
          int size = 0 ; 
          if(line_start == line_end){
            size = byte_end - byte_start ; 
          }else{
            for(int i = line_start  ; i <= line_end ; i ++){
              if(i == line_start){
                size += strlen(buffer[i-1])-byte_start ; 
              }else{
                if(i != line_end){
                  size += strlen(buffer[i-1]) ; 
                }else{
                  size += byte_end ; 
                 }
              }
            }
          } 
          if(strcmp(command , "copy") == 0 || strcmp(command , "y") == 0 ) copy_str(argv[1]  , line_start , byte_start , size , 1)  ; else{
             cut_str(argv[1] , line_start , byte_start , size , 1) ; 
           }
          number_of_line = 0  ;
          erase()  ; 
          show(argv[1]) ;
          } 
      }

      if(strcmp(command , "paste") == 0 && strcmp(mode[mode_counter] ,  "NORMAL\0") ==0 ){
        paste_str(argv[1] , x+1 , y - 5 - get_digit(x+1)  ) ; 
        erase() ; 
        number_of_line = 0 ; 
        show(argv[1])  ; 
      }      

      if((strcmp(command , "u") == 0 || strcmp(command , "undo") == 0 ) && strcmp(mode[mode_counter] , "NORMAL\0") == 0){
        Find_backup(argv[1]) ; 
        erase() ; 
        number_of_line = 0 ;
        show(argv[1]) ;
      }

      if(strcmp(command , "insertstr") == 0){
        run(string_use) ; 
        number_of_line = 0 ; 
        erase() ; 
        show(argv[1]) ;
      }


      mvprintw(number_of_line + 6  , 0, "%s" , blank) ; 
      move(number_of_line + 6 , 0 ) ; 
      mvprintw(number_of_line + 5  , 0 , "%s   %s %c" , mode[mode_counter] , filename , save) ;  
      mvchgat(number_of_line  + 5 , 0, -1, A_BLINK, 1, NULL);
      
    noecho() ;  
    }

  if((ch == 'h' || ch == 'l' || ch == 'j' || ch == 'k') && strcmp(mode[mode_counter] , "INSERT\0") != 0 ){
      if(x >= number_of_line + 5) {
        x = 0 ; 
        y = 5 ; 
      }
      if(ch == 'h'){
        if(y > 5 + get_digit(x+1)) move(x , --y) ;
      }
      if(ch == 'l'){
        if(y < (strlen(buffer[x]) + 5 + get_digit(x))) move(x , ++y) ;
      }
      if(ch == 'j'){
        if(x < number_of_line - 1 ){
          x ++ ;
          if(y < strlen(buffer[x]) + 5 + get_digit(x+1)) move(x , y) ; else{ 
            y = strlen(buffer[x]) + 4 + get_digit(x+1) ;
            move(x  , y  ) ;
          } 
        }else{
          if(x < number_of_line + 5){
            if(x == number_of_line - 1 ){
              x ++ ;
              y = 0 ;
              move(x , y) ; 
            }else{  
            mvprintw(x , 0  , " " ) ; 
            if(x != number_of_line + 4 ) x ++ ; 
            move(x , 0 ) ;
            }
          }
        }
      }
      if(ch == 'k'){
        if(x != 0 ){
          if(x <= number_of_line - 1 ){
          x -- ; 
          if(y < strlen(buffer[x]) + 5 + get_digit(x+1) && y > get_digit(x+1) + 5  ) move(x , y) ; else{
             y = strlen(buffer[x]) + 5 + get_digit(x+1) ; 
             move(x  , y ) ;
            }
          }else{
            attron(A_PROTECT) ;
            mvprintw(x , 0 , "~") ; 
            attroff(A_PROTECT) ;
            x -- ; 
            if(x != number_of_line-1) move(x ,  0 ) ; else {
              y = strlen(buffer[x]) + 5 + get_digit(x+1)  ; 
              move(x , y) ; 
            }
          } 
        }
      }
     }
  }
	endwin();			
	return 0;
}



int get_digit(int x){
  int out = 0 ;
  while(x != 0){
    x/= 10 ; 
    out ++ ; 
  }
  return out ; 
}

void show(char * address){
  FILE * myfile = fopen(address , "r") ;
	while(fgets(buffer[number_of_line] ,  MAX_LENGTH , myfile) != NULL){
      while(strlen(buffer[number_of_line]) > col - 7){
        for(int i = col - 7 ; i < strlen(buffer[number_of_line]) ; i ++){
          buffer[number_of_line +1][i - col + 7] = buffer[number_of_line][i] ; 
        }
        buffer[number_of_line][col - 7] = '\0' ;
         mvprintw(number_of_line , 0 ,  "    %d %s" , number_of_line+1 , buffer[number_of_line]) ;
        number_of_line ++  ;
      }
      mvprintw(number_of_line , 0 ,  "    %d %s" , number_of_line+1 , buffer[number_of_line]) ;
      number_of_line ++ ; 

  }
  attron(A_PROTECT) ;  
  for(int i = 0 ; i < 5 ; i ++){
    mvprintw(number_of_line+i , 0 , "~") ;
  }
  attroff(A_PROTECT) ; 
  fclose(myfile)  ;
 }


void init_1(char * string ){
  strcpy(filename , " ") ; 
  strcpy(dirname , " ") ; 
  int base ; 
  bool get_filename = true ; 
  for(int i = strlen(string) - 1 ; i >= 0 ; i --  ){
    if(string[i] == '/'){
      get_filename = false ;
      i-- ; 
      base = i ; 
    }
    if(get_filename) filename[strlen(string) - 1 - i ]  = string[i] ;  else dirname[base - i] = string[i] ; 
  }
  strrev(filename) ; 
  strrev(dirname) ; 
 }