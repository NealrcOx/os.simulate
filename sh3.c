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

	char commandtemp[256]={ 0 }; //存放分割完成的命令，每个命令带\0
	int commandCount = 0;

	int sumAddress = 0; //记录每次分割后，移动到commandTemp[]中的位置


int mysys(char *command);     //模拟system();

//void execSimple(char * command); //执行非管道命令

//void execPipe(int childCount);  //执行管道命令

void paresCommands( char * commandLine);  //最外层命令分割，分割符号为" | "

void paresCommand( char * commandLine, struct command * command); //内层分割，分割符号为“  ”

void commandDump(struct command * commandTest,int cmandCount);  //打印命令，argv 和 argc

void testCommand(int sumCommandCont); //检查总的命令, 与commndDump()函数结合使用

//char prePath[255] = { 0 };    //存放每次发生改变的工作路径；
//bool chdirtoprePath = false;    //工作路径改变标志，未改变就是false;


int main(void)
{
  char strCom[256] = {0} ; //= "cat -l | wc -l";
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
							printf("strCom:%s\n", strCom);
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
			testCommand(commandCount);
      return 0;
}

void paresCommands( char * commandLine){
  char * ptr = NULL;
	char * outPtr = NULL;
//	commandCount = 0;   //管道命令总个数

	char buffer[64] = { 0 };
	strcpy(buffer, commandLine);

  ptr = (char * )strtok_r(buffer, "|", &outPtr);
  while(ptr != NULL){
    paresCommand(commandLine,&commands[commandCount]);
    commandCount++;
    ptr = (char *)strtok_r(NULL, "|", &outPtr);
  }
}

void paresCommand( char * commandLine,struct command * command){
  char * ptrNext = NULL;  	//内部分割函数返回地址
	char * innerPtr = NULL;		//存储内部分割剩余字符串地址
	char * inptr = NULL;			//外部分割函数返回地址
	char * outPPtr = NULL;		//存储外部分割剩余字符串地址
	int simpleCommandCount = 0; //被管道隔开的每个命令个数
	int m = 0;

	//保护原始命令不被修改
	char buffer2[64] = { 0 };
	strcpy(buffer2, commandLine);

	inptr = (char * )strtok_r(buffer2, "|", &outPPtr);
		while( inptr != NULL){
			if(m == commandCount){
  			ptrNext = strtok_r(inptr, " ", &innerPtr);
  				while(ptrNext != NULL){
							for(int y = 0 ; y < (int)strlen(ptrNext); y++){
								commandtemp[sumAddress + y] = *(ptrNext + y );
							}
    					command->argv[simpleCommandCount] = &commandtemp[sumAddress];
							sumAddress += (int)strlen(ptrNext) + 1;
							simpleCommandCount++;
						ptrNext = (char *)strtok_r(NULL," ", &innerPtr);
						if(ptrNext == NULL){
							break;
						}
  				}
					command->argc = simpleCommandCount;
				}
				m++;
				inptr = (char *)strtok_r(NULL, "|", &outPPtr);
			}
}

void commandDump(struct command * commandTest,int cmandCount){
		printf("commands[%d]\targc:%d  argv:\t", cmandCount, commandTest->argc);
		for(int i = 0 ; i < commandTest->argc ; i++){
			printf("argv[%d]%s\t", i,commandTest->argv[i]);
		}
		printf("\n");
}

void testCommand(int sumCommandCont){
	for(int j = 0 ; j < sumCommandCont ; j++){
		commandDump(&commands[j],j);
	}
	printf("\ncommandCount:%d\n",commandCount);
}
