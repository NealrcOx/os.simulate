/*
sh3.c: 实现shell程序，要求在第2版的基础上，添加如下功能

    实现管道
    只要求连接两个命令，不要求连接多个命令
    不要求同时处理管道和重定向

    # 执行sh3
    $ ./sh3

    # 执行命令cat和wc，使用管道连接cat和wc
    > cat /etc/passwd | wc -l

    考虑如何实现管道和文件重定向，暂不做强制要求

    $ cat input.txt
    3
    2
    1
    3
    2
    1
    $ cat <input.txt | sort | uniq | cat >output.txt
    $ cat output.txt
    1
    2
    3
*/
	#include<stdio.h>
	#include<unistd.h>
	#include<sys/types.h>
	#include<string.h>
	#include<sys/wait.h>
	#include<errno.h>
	#include<sysexits.h>
	#include<stdlib.h>
	#include<pwd.h>
	#include<stdbool.h>
	#include<fcntl.h>

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
	int commandCount = 0;


int mysys(char *command);     //模拟system();

//void execSimple(char * command); //执行非管道命令

//void execPipe(int childCount);  //执行管道命令

void paresCommands(char * commandLine);  //最外层命令分割，分割符号为" | "

void paresCommand(char * ptr, int count); //内层分割，分割符号为“  ”

//void commandDump(command);  //打印分割命令字符串


//char prePath[255] = { 0 };    //存放每次发生改变的工作路径；
//bool chdirtoprePath = false;    //工作路径改变标志，未改变就是false;


int main(void)
{
  char strCom[256] = { 0 };
  int n;
  printf(">");
 while(fgets(strCom, 255, stdin) != NULL )
  {
    n = 0;
    while(strCom[n] != '\n' && strCom[n] != '\0')
      {
        n++;
      }
      if(strCom[n] == '\n' && n > 0)
        strCom[n] = '\0';
          if(strCom[0] != '\n')   //当第一个参数就是 回车 时，直接处理，重新输入；
            {
              mysys(strCom);
               printf(">");
            }
            else
              printf(">");
  }
  return 0;
}

int mysys(char * line)
{
      paresCommands(line);
      return 0;
}

void paresCommands(char * commandLine){
  char * ptr;
  ptr = (char * )strtok(commandLine, "|");
  while(ptr != NULL){
    paresCommand(ptr,commandCount);
    commandCount++;
    ptr = (char *)strtok(NULL, "|");
  }
}

void paresCommand(char * ptr, int count){
  char * ptrNext;
  ptrNext = strtok(ptr, " ");
  while(ptrNext != NULL){
    commands[count].argc++;
  }
}
