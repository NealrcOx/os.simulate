/*
sort.c: 多线程排序

    主线程创建两个辅助线程
    辅助线程1使用选择排序算法对数组的前半部分排序
    辅助线程2使用选择排序算法对数组的后半部分排序
    主线程等待辅助线程运行結束后,使用归并排序算法归并子线程的计算结果
    本题要求 1: 使用线程参数，消除程序中的代码重复
*/
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#define CHILDPTHCNT 2     //创建的子线程个数
#define SORTARRAY 20    //排序的数组长度

struct pthreadPara{
  int * start_address;
  int  start;
  int  end;
};

struct result{
  struct pthreadPara resultc;
};

void print_deal_array(int * arr);   //输出数组

void * pthread_sort(void * arg);

void merge_sort(struct result * one, long long len);


int main(void){

  srand((unsigned int)time(0));  //初始化种子为随机直

  int deal_array[SORTARRAY];
  for(long long j = 0 ; j < SORTARRAY ; j++){
    deal_array[j]  = rand() % SORTARRAY + 1; //1～SORTARRAY
  }

  print_deal_array(deal_array);

  pthread_t child_tid[CHILDPTHCNT];
  struct pthreadPara paras[CHILDPTHCNT];
  struct result results[CHILDPTHCNT];
  for(int j = 0 ; j < CHILDPTHCNT ; j++){
    struct pthreadPara * para;
    para = & paras[j];
    para -> start_address = deal_array;
    para -> start = (j / CHILDPTHCNT) * SORTARRAY;
    para -> end = ((j + 1) / 2) * SORTARRAY;

    pthread_create(&child_tid[j], NULL, &pthread_sort, para);
  }

  for(int j = 0 ; j < CHILDPTHCNT ; j++){
    struct result * result;
    pthread_join(child_tid[j],(void **)&result);
    results[j] = * result;
    merge_sort(&results[j], SORTARRAY);
    free(result);
  }

  print_deal_array(deal_array);
  return 0;
}

void print_deal_array(void){
  for(long long j = 1 ; j <= SORTARRAY ; j++){
    printf("-%10d", deal_array[j - 1]);
    if(j % 10 == 0){
      printf("\n");
    }
  }
}

void * pthread_sort( void * arg){
  struct pthreadPara * para;
  struct result * result;
  int temp;
    temp = 0;

  para = (struct pthreadPara *)arg;
  for(long long j = para->start ; j < para->end ; j++){
    temp = *(para->start_address + j * sizeof(int));
    for(long long i = j + 1 ; i < para->end ; i++){
      if(*(para->start_address + i * sizeof(int)) <= temp){
        temp = *(para->start_address + j * sizeof(int));
      }
    }
    *(para->start_address + (j + 1) * sizeof(int)) = *(para->start_address + j * sizeof(int));
    *(para->start_address + j * sizeof(int)) = temp;
  }
  result = (struct result *)malloc(sizeof(struct result));
  result->resultc = * para;
  return result;
}

void merge_sort(struct result * one, long long len){
  if(i != CHILDPTHCNT){
    merge_sort(struct result * one, long long len);
  }
  else{
    int * pre_sort_addr = NULL;
    int * late_sort_addr = NULL;
    
  }
}
