#include<stdio.h>
#include<string.h>

int main(void){
  char str[] = "kdl | lk cd |op";
  char * ptr =NULL,* pptr =NULL;
  //char * argv[10];
  char * a[10] = { 0 };
  int j = 0;
  char * out = NULL,* inner = NULL;
  printf("begain:str:%s\n", str);
  ptr = strtok_r(str, "|", &out);
  while(ptr != NULL){
    pptr = strtok_r(ptr, " ", &inner);
    while(pptr != NULL){
      a[j++] = pptr;
      pptr = strtok_r(NULL," ", &inner);
    }
    ptr = strtok_r(NULL, "|", &out);
  }
  printf("before:str:%s\n", str);
  for (int  i = 0; i < j; i++) {
    printf("%s\t", a[i]);
  }
  return 0;
}
