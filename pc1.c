/*
pc1.c: 使用条件变量解决生产者、计算者、消费者问题

    系统中有3个线程：生产者、计算者、消费者
    系统中有2个容量为4的缓冲区：buffer1、buffer2
    生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1
    计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2
    消费者从buffer2取出字符，将其打印到屏幕上
*/
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

#define CAPACITY 4

int buffer1[CAPACITY];
int buffer2[CAPACITY];
int buffer1_in;
int buffer1_out;
int buffer2_in;
int buffer2_out;

int buffer1_is_empty()
{
    return buffer1_in == buffer1_out;
}

int buffer1_is_full()
{
    return (buffer1_in + 1) % CAPACITY == buffer1_out;
}

int buffer2_is_empty()
{
    return buffer2_in == buffer2_out;
}

int buffer2_is_full()
{
    return (buffer2_in + 1) % CAPACITY == buffer2_out;
}

int calcu_get_item()
{
    int item;

    item = buffer1[buffer1_out];
    buffer1_out = (buffer1_out + 1) % CAPACITY;
    return item;
}

void calcu_put_item(int item)
{
    buffer1[buffer1_in] = item;
    buffer1_in = (buffer1_in + 1) % CAPACITY;
}

int get_item()
{
    int item;

    item = buffer2[buffer2_out];
    buffer2_out = (buffer2_out + 1) % CAPACITY;
    return item;
}

void put_item(int item)
{
    buffer2[buffer2_in] = item;
    buffer2_in = (buffer2_in + 1) % CAPACITY;
}

pthread_mutex_t mutex_sema1,mutex_sema2;
pthread_cond_t wait_empty_buffer1,wait_empty_buffer2;
pthread_cond_t wait_full_buffer1,wait_full_buffer2;

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
      pthread_mutex_lock(&mutex_sema2);
        while (buffer2_is_empty())
            pthread_cond_wait(&wait_full_buffer2, &mutex_sema2);
        item = get_item();
        printf("\t\tconsume item: %c\n", item);
        pthread_cond_signal(&wait_empty_buffer2);
        pthread_mutex_unlock(&mutex_sema2);
    }

    return NULL;
}

void *produce()
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
      pthread_mutex_lock(&mutex_sema1);
        while (buffer1_is_full())
            pthread_cond_wait(&wait_empty_buffer1, &mutex_sema1);

        item = i + 'a';
        calcu_put_item(item);
        printf("produce item: %c\n", item);

        pthread_cond_signal(&wait_full_buffer1);
        pthread_mutex_unlock(&mutex_sema1);
    }

    return NULL;
}

void *calculate(void * arg){
  //int i;
  int item;
  for(int i = 0 ; i < ITEM_COUNT ; i++){
    pthread_mutex_lock(&mutex_sema1);
        while (buffer1_is_empty())
            pthread_cond_wait(&wait_full_buffer1, &mutex_sema1);
      item = calcu_get_item();
        pthread_cond_signal(&wait_empty_buffer1);
        pthread_mutex_unlock(&mutex_sema1);

      item = item - ('a' - 'A');
      printf("\tcalculate item:%c\n", item);

      pthread_mutex_lock(&mutex_sema2);
          while (buffer2_is_full())
              pthread_cond_wait(&wait_empty_buffer2, &mutex_sema2);
      put_item(item);

      //pthread_cond_signal(&wait_empty_buffer1);
      //pthread_mutex_unlock(&mutex_sema1);

      pthread_cond_signal(&wait_full_buffer2);
        pthread_mutex_unlock(&mutex_sema2);
  }
  return NULL;
}

int main(void){
  pthread_t consumer_tid,calculate_tid;

  pthread_mutex_init(&mutex_sema1, NULL);
  pthread_cond_init(&wait_empty_buffer1, NULL);
  pthread_cond_init(&wait_full_buffer1, NULL);

  pthread_mutex_init(&mutex_sema2, NULL);
  pthread_cond_init(&wait_empty_buffer2, NULL);
  pthread_cond_init(&wait_full_buffer2, NULL);

    pthread_create(&calculate_tid, NULL, calculate, NULL);
    pthread_create(&consumer_tid, NULL, consume, NULL);
    produce();
    pthread_join(calculate_tid, NULL);
    pthread_join(consumer_tid, NULL);
    //pthread_join(calculate_tid, NULL);
    return 0;
}
