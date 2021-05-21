/*mysys.c--to be like system command*/
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<errno.h>

int mysys(char *command);
void split(char * src, const char * separator, char ** dest, int * num);   //字符串分割函数；

int main(void)
{
  printf("---------------------------------------------\n");
  mysys("echo HELLO WORLD");
  printf("---------------------------------------------\n");
  mysys("ls /");
  printf("---------------------------------------------\n");
  return 0;
}

int mysys(char *command)
{
  char str[256] = { 0 };
  for(int i = 0 ; i < (int)strlen(command) ; i++)
      str[i] = *(command + i);      //char *str为字符串常量，保存在静态存储区，无法修改，复制副本到字符数组中，位于堆，可以操作；
  pid_t pid;
  int status;     //mysys(char *command) function's status;
  int num;
    num = 0;
  char * argv[255] = { 0 };
 split(str, " ", argv, &num);   //调用split()分割命令行字符串;

 pid = fork();
  if(command == NULL)
    status = 1;
   else if(pid < 0)
    status = -1;      //对于调用fork()失败（fork()返回值：在父进程中，返回新创建子进程ID，在子进程中返回0，错误返回负值）
     else if(pid == 0)
      {
        //int error = execvp("echo", argv);
        int error = execvp(argv[0],argv);
          if(error < 0)
            {
              perror("execvp");    //perror()打印上一个函数错误原因；
            }
            _exit(127);        //执行execvp()成功，则正常退出子进程,且execvp()正常执行，不会执行此语句；
      }
      else
      {
        while(waitpid(pid, &status, 0) < 0)
        {
          if(errno != EINTR)
          {
            status = -1;
            break;
          }
        }
      }
      return status;  //如果waitpid()执行成功，返回状态；
}
void split(char * src, const char * separator, char ** dest, int * num)
{
  /*
		src 源字符串的首地址(buf的地址)；
		separator 指定的分割字符；
		dest 接收子字符串的数组；
		num 分割后子字符串的个数,如果主函数要调用可以直接使用；
	*/
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止；
        return; //无返回值，直接return;
     if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止;
        return;
     pNext = (char *)strtok(src,separator);   /*必须使用(char *)进行强制类型转换
     (虽然不写有的编译器中不会出现指针错误)*/
     while(pNext != NULL)
     {
          *dest++ = pNext;
          count++;
         pNext = (char *)strtok(NULL,separator);  //必须使用(char *)进行强制类型转换
      }
    *num = count;
}
