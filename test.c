#include<stdio.h>
#include<string.h>

int main(void){
  char str[] = " kdl|klk | kl";
  char * ptr;
  char * a[10] = { 0 };
  int j = 0;
  printf("begain:str:%s\n", str);
  ptr = strtok(str, "|");
  while(ptr != NULL){
    a[j++] = ptr;
    ptr = strtok(NULL, "|");
  }
  printf("before:str:%s\n", str);
  for (int  i = 0; i < j; i++) {
    printf("%s\t", *(a + i));
  }
  return 0;
}
