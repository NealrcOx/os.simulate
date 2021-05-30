#include<stdio.h>
#include<string.h>
#define CD "cd"
#define EXIT "exit"
#define MAX_ARGC 16
#define MAX_COMMANDS 20


struct command {
  int argc;
  char * argv[MAX_ARGC];
  char * input;   //用于重定向输入
  char * output;  //用于重定向输出
};

struct command commands[MAX_COMMANDS];

int main(void){
  char str[] = "kdl -l | lk cd";
  char * ptr =NULL,* pptr =NULL;
  //char * argv[10];
  //char * a[10] = { 0 };
  int j = 0,k = 0;
  char * out = NULL,* inner = NULL;
  struct command * command;
  printf("begain:str:%s\n", str);
  ptr = strtok_r(str, "|", &out);
  while(ptr != NULL){
    pptr = strtok_r(ptr, " ", &inner);
    while(pptr != NULL){
      printf("k:%d j:%d\n", k,j);
      command = &commands[k];
       command->argv[j] = pptr;
       j++;
      pptr = strtok_r(NULL," ", &inner);
    }
    k++;
    ptr = strtok_r(NULL, "|", &out);
  }
  printf("before:str:%s k:%d j:%d\n", str,k,j);
  for (int  i = 0; i < k; i++) {
    for(int m = 0 ; m < j;m++){
    if(i == 0){
    printf("%s\n", commands[i].argv[m]);
  }
    if(i == 1){
      printf("%s\n", commands[i].argv[m + 2]);
  }
}
}
  return 0;
}
