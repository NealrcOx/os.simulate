/*
pi1.c: 使用2个线程根据莱布尼兹级数计算PI

    莱布尼兹级数公式: 1 - 1/3 + 1/5 - 1/7 + 1/9 - ... = PI/4
    主线程创建1个辅助线程
    主线程计算级数的前半部分
    辅助线程计算级数的后半部分
    主线程等待辅助线程运行結束后,将前半部分和后半部分相加
    pi2.c: 使用N个线程根据莱布尼兹级数计算PI

    与上一题类似，但本题更加通用化，能适应N个核心
    主线程创建N个辅助线程
    每个辅助线程计算一部分任务，并将结果返回
    主线程等待N个辅助线程运行结束，将所有辅助线程的结果累加
    本题要求 1: 使用线程参数，消除程序中的代码重复
    本题要求 2: 不能使用全局变量存储线程返回值
*/
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#define MAXCUCLUSTEPS 1000000000
#define CPUCOUNT 4

struct pthreadPara {
  double start;
  double end;
};

struct result{
  double sum;
};

void * pthreadCount(void * arg);


int main(void){
  double total = 0.0;
  pthread_t child_tid[CPUCOUNT];
  struct pthreadPara paras[CPUCOUNT];
  for(int i = 0 ; i < CPUCOUNT ; i++){
  struct pthreadPara * para;
  para = &paras[i];
  para->start = (i / CPUCOUNT) * MAXCUCLUSTEPS;
  para->end = ((i + 1) / CPUCOUNT) * MAXCUCLUSTEPS;

  pthread_create(&child_tid[i], NULL,&pthreadCount,para);
}
for(int j = 0 ; j < CPUCOUNT ; j++){
  struct result * result;
  pthread_join(child_tid[j],(void **)&result);
  total += result->sum;
}
  printf("Pi:%.9lf\n", 4 * total );
  return 0;
}

void * pthreadCount(void * arg){
  struct pthreadPara *para;
  struct result * result;
    double flag = 1.0;
    double temp = 0.0;

    para = (struct pthreadPara *)arg;
  for(double j =  para->start; j < para->end ; j++){
    if( ((int)j) % 2 == 1){
      temp += (flag * ( 1 / j));
      flag *= -1;
    }
  }
  result = malloc(sizeof(struct result));
  result->sum = temp;
  return result;
}
