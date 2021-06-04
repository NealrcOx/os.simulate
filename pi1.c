/*
pi1.c: 使用2个线程根据莱布尼兹级数计算PI

    莱布尼兹级数公式: 1 - 1/3 + 1/5 - 1/7 + 1/9 - ... = PI/4
    主线程创建1个辅助线程
    主线程计算级数的前半部分
    辅助线程计算级数的后半部分
    主线程等待辅助线程运行結束后,将前半部分和后半部分相加
*/
#include<pthread.h>
#include<stdio.h>
#define MAXCUCLUSTEPS 10000000000

double pthreadResultOfSum = 0.0; //辅助线程求和
double mainResultOfSum = 0.0;     //主线程求和

void * pthreadCount(void * arg);

void mainCount(void);

int main(void){
  double total = 0.0;
  pthread_t child_tid;
  pthread_create(&child_tid, NULL,&pthreadCount,NULL);
  mainCount();
  pthread_join(child_tid,NULL);
  total = pthreadResultOfSum + mainResultOfSum;
  printf("Pi:%.9lf\n", 4 * total );
  return 0;
}

void * pthreadCount(void * arg){
    double flag = 1.0;
  for(double j = MAXCUCLUSTEPS / 2 ; j < MAXCUCLUSTEPS ; j++){
    if( ((int)j) % 2 == 1){
      pthreadResultOfSum += (flag * ( 1 / j));
      flag *= -1;
    }
  }
  return NULL;
}

void mainCount(void){
  double flag = 1.0;
for(double i = 1.0 ; i < MAXCUCLUSTEPS / 2 ; i++){
  if( ((int)i) % 2 == 1){
    mainResultOfSum += (flag * ( 1 / i));
    flag *= -1;
  }
}
}
