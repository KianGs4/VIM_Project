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
char temp_file[MAX_LENGTH] = {0} ; 
int row , col ;
int x = 0 ;
int y = 6 ; 

int get_digit(int x) ; 
void show() ; 
void init_1(char * string)  ; 
void find_vim( char * address , char * string ) ; 
void make_copy(char * address , char * address2) ; 
int main(int argc, char *argv[]){
  initscr();
  raw() ;
  keypad(stdscr, TRUE) ;
  noecho();  			
	start_color();	
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
  char ch = 'z' ;
  bool keep_reading = true ; 
  while(keep_reading){
  if(strcmp(mode[mode_counter] , "INSERT\0"))  ch = getch() ; 
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
    } 
  if(strcmp(mode[mode_counter] , "INSERT\0") == 0 ){
      ch = getch() ; 
      while(ch != ctl('i') && ch != ctl('j') && ch != ctl('h') && ch != ctl('l') && ch != ctl('k')){
         if( ch == ctl('d')){
            int y2 = y ; 
            int x2 = x ;
             if(y2 != 5 + get_digit(x+1)){
              y2 -- ; 
             }else{
              x2 -- ; 
              y2 = strlen(buffer[x])  ; 
             }

            remove_str(argv[1] , x+1 , y - get_digit(x+1) - 5 , 1 , -1) ; 
              y = y2 ; 
              x = x2 ;
         }else{
            char word[2] ; 
            word[0] = ch ; 
            word[1] ='\0' ; 
            insert_str(argv[1]  , word , x+1 , y - get_digit(x+1) - 5) ;
            y ++ ; 
            }
          erase() ; 
          number_of_line = 0 ;
          refresh() ; 
          show(argv[1]) ;
          mvprintw(number_of_line + 5   , 0 , "%s   %s %c" , "INSERT" , filename , save) ;
        	mvchgat(number_of_line + 5 , 0, -1, A_BLINK, 1, NULL); 
          refresh() ; 
          ch = getch();
      }
        if(ch == ctl('h')) ch = 'h' ; 
        if(ch == ctl('j')) ch = 'j' ; 
        if(ch == ctl('k')) ch = 'k' ;         
        if(ch == ctl('l')) ch = 'l' ; 
        if(ch == ctl('i')) ch = 'i' ; 
   }
  if(ch == ':' || ch == '\\'){
      char string[MAX_LENGTH] ;
      char string_use[MAX_LENGTH] ; 
      mvprintw(number_of_line + 6 , 0 , ":") ; 
      echo() ; 
      save = '+' ; 
      getstr(string) ; 
      char tok[2] = " " ; 
      strcpy(string_use , string) ; 
      char * command = strtok(string , tok) ; 
      if(strcmp(command, "save") == 0 ) {
        save = ' ' ; 
        make_copy(temp_file , argv[1]) ; 
      }
      if(strcmp(command , "wq")  == 0 ){
        if(save == '+'){
          make_copy(argv[1] , temp_file) ; 
        }
        remove(temp_file) ; 
        break ; 
      } 
      if(strcmp(command , "saveas") == 0 ){
        command = strtok(NULL  , tok) ; 
        save = ' ' ; 
        char temp_add[MAX_LENGTH] = {0}  ;
        strcpy(temp_add , dirname) ; 
        strcpy(filename , command) ; 
        strcat(temp_add , filename) ;
        rename(argv[1] ,temp_add ) ; 
        strcpy(argv[1] , temp_add) ;
        make_copy(temp_file , argv[1]) ;
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
            strcpy(temp_file , command) ; 
            strcat(temp_file , "tempVIM.1.0.0") ; 
            make_copy(temp_file , command) ;              
        }else{
          mvprintw(number_of_line+6 , 0  , "INVALID FILE NAME ") ; 
          getch() ; 
        }
      }

      if(strcmp(command ,  "delete") == 0  || strcmp(command , "cut") == 0 || strcmp(command , "d") == 0  || strcmp(command , "y") == 0 || strcmp(command , "copy") == 0){
          mode_counter = 0 ; 
          if(line_start >= 0 && byte_start >= 0){
          int size = 0 ; 
          if(line_end > number_of_line - 1 || (line_end == number_of_line - 1  && byte_end > strlen(buffer[number_of_line - 1]))){
            line_end = number_of_line - 1 ; 
            byte_end = strlen(buffer[number_of_line]) ; 
          }
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
                  if(byte_end < strlen(buffer[i-1])) size += byte_end ; else size += strlen(buffer[i - 1 ]) ;
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
      if(strcmp(command , "=") == 0){      
        closing_pair(argv[1]) ; 
        erase() ; 
        number_of_line = 0 ;
        show(argv[1]) ;
      }
      if(Search_Commands(command)){
       if(strcmp(command , "auto-indent") == 0){
          command = strtok(NULL , tok) ;
          if(command == NULL){
            closing_pair(argv[1]) ;
            erase() ; 
            number_of_line = 0 ; 
            show(argv[1]) ; 
          }else{
            run(string_use) ; 
            number_of_line = 0 ; 
            erase() ; 
            show(argv[1]) ;
          }
       }else{
        run(string_use) ; 
        number_of_line = 0 ; 
        erase() ; 
        show(argv[1]) ;
        }
      }

      if(string_use[0] == '\\' && strcmp(mode[mode_counter] , "NORMAL\0") == 0 ){
        for(int i = 0 ; i < strlen(string_use) ; i ++){
          string_use[i] = string_use[i+1] ; 
        }
        noecho(); 
        find_vim(argv[1]  , string_use) ; 
        echo() ;
      }

      mvprintw(number_of_line + 6  , 0, "%s" , blank) ; 
      move(number_of_line + 6 , 0 ) ; 
      mvprintw(number_of_line + 5  , 0 , "%s   %s %c" , mode[mode_counter] , filename , save) ;  
      mvchgat(number_of_line  + 5 , 0, -1, A_BLINK, 1, NULL);
      
    noecho() ;  
    }

  if((ch == 'h' || ch == 'l' || ch == 'j' || ch == 'k' || ch == 'g' || ch == ctl('g')) ){
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
      if(ch == 'g'){
        move(0 ,  5) ; 
        x = 0 ; 
        y = 5 ;
      }else{
        if(ch == ctl('g')){
          x = number_of_line - 1 ;
          y = strlen(buffer[x]) + 5 + get_digit(x+1) ;
          move(x , y) ; 
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


void find_vim( char * address , char * string ){
    int findcase[MAX_FINDCASE][2]  ;
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
            int pointer = 0  ;  
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
                findcase[cnt ][0] = line ; 
                findcase[cnt ][1] = pointer ;  
                cnt ++ ; 
                current_letter = 0 ;
                pointer += size ;  

                }
            }
            if(feof(my_file)) break ;  
        }
        fclose(my_file) ; 
    //scetion 2 : show
    if(cnt == 0 ){
      mvprintw(number_of_line + 6  , 0  , "NO MATCH\t\t\t\t\t" ) ; 
      move(number_of_line + 6 , 0  ) ;
      getch() ;
    }else{
      attron(COLOR_PAIR(2)) ; 
       for(int i = 0 ; i < cnt ; i ++ ){
          mvprintw(findcase[i][0] - 1 , findcase[i][1] + 5 + get_digit(findcase[i][0]) , "%s" , string) ;
        }
      attroff(COLOR_PAIR(2)) ; 
    }
    char c ; 
    int where = 0 ;
    while((c = getch()) == 'n'){
        if(where != cnt )move(findcase[where][0] - 1 , findcase[where][1] + 5 + get_digit(findcase[where][0])) ; 
        if(where == cnt) {
          where = -1 ; 
          move(findcase[0][0] - 1  , findcase[0][1] + 5 + get_digit(findcase[0][0])) ;   
        }
        getyx(stdscr , y  ,x ) ;
        where ++ ; 
    }
    erase() ; 
    number_of_line = 0 ;
    show(address) ; 
 }  

void make_copy(char * address , char * address2) {
  FILE * my_file = fopen(address2 , "r") ; 
  FILE * temppp  = fopen(address , "w") ; 
  char buffer_free[MAX_LENGTH ] ; 
  while(fgets(buffer_free  , MAX_LENGTH , my_file) != NULL){
    fputs(buffer_free , temppp) ; 
  }
  fclose(my_file) ; 
  fclose(temppp) ;
  }