/*sh.c--to be like system command*/
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

#define CD "cd"
#define EXIT "exit"

int mysys(char *command);     //模拟system();
void split(char * src, const char * separator, char ** dest, int * num);   //字符串分割函数；
char prePath[255] = { 0 };    //存放每次发生改变的工作路径；
bool chdirtoprePath = false;    //工作路径改变标志，未改变就是false;
int main(void)
{
  char str_com[256] = { 0 };
  int n;
  printf(">");
 while(fgets(str_com, 255, stdin) != NULL )
  {
    n = 0;
    while(str_com[n] != '\n' && str_com[n] != '\0')
      {
        n++;
      }
      if(str_com[n] == '\n' && n > 0)
        str_com[n] = '\0';
          if(str_com[0] != '\n')   //当第一个参数就是 回车 时，直接处理，重新输入；
            {
              mysys(str_com);
               printf(">");
            }
            else
              printf(">");
  }
  return 0;
}

int mysys(char *command)
{
  struct passwd *my_pwd;      //指向strcut passwd结构体；
  char str[256] = { 0 };      //这是三个用来处理命令中有~的字符数组；
  char str2[256] = { 0 };
  char str3[256] = { 0 };
  for(int i = 0 ; i < (int)strlen(command) ; i++)
      str2[i] = *(command + i);      //char *str为字符串常量，保存在静态存储区，无法修改，复制副本到字符数组中，位于堆，可以操作；
  pid_t pid;
  int status;     //mysys(char *command) function's status;
  int num;        //命令行输入总参数；
    num = 0;
  char * argv[255] = { 0 };       //存放命令行直接参数
  char * argv2[255] = { 0 };      //额外存放需要更替的命令行参数；
      for(int j = 0 ,k = 0; j < (int)strlen(str2) ; j++)    //这个循环是为了用home路径替换命令中的～
        {
          if(str2[j] != '~')
            {
              str[k] = str2[j];
              k++;
            }
            else if(str2[j] == '~')
              {
                my_pwd = getpwuid(getuid());
                strcpy(str3, my_pwd->pw_dir);
                for(int i = 0 ; i < (int)strlen(str3) ; i++)
                  {
                    str[k] = str3[i];
                    k++;
                  }
              }
        }

 split(str, " ", argv, &num);   //调用split()分割命令行字符串;
  if(!strcmp(*(argv), CD))
    {
      char *tempPath = (char *)malloc(strlen(prePath) + 1);     //临时变量，用来存现在的工作路径；
      memset(tempPath, 0 , (int)sizeof(tempPath));
      strcat(tempPath, prePath);

      getcwd(prePath, sizeof(prePath));
            if(!strcmp(argv[1], "-"))   //cd: -;   没有实现cd -命令；
                {
                  if(chdirtoprePath)
                    {
                      argv2[0] = tempPath;
                      chdir(argv2[0]);
                    }
                    else
                      {
                        chdir(tempPath);
                        perror("cd");   //当没有切换国工作目录就cd -时，错误提示；
                      }
                }
              else
                {
                    int error = chdir(argv[1]);
                    chdirtoprePath = true;    //当有一次更换工作路径时，就把这个标志设置为true;
                    if(error < 0)
                      {
                        perror("chdir");
                      }
                  }
                  free(tempPath);
      }
        else if(!strcmp(argv[0], EXIT))
              {
                  exit(0);
              }
            else
            {
              pid = fork();
               if(command == NULL)
                 return (1);
                else if(pid < 0)
                    return (status = -1);      //对于调用fork()失败（fork()返回值：在父进程中，返回新创建子进程ID，在子进程中返回0，错误返回负值）
                  else if(pid == 0)
                   {
                              int error = execvp(argv[0],argv);
                              if(error < 0)
                                {
                                   perror("execvp");    //perror()打印上一个函数错误原因；
                                }
                                _exit(127);        //执行execvp()成功，则正常退出子进程,且execvp()正常执行，不会执行此语句；
                          //  }
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
      return 0;
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
