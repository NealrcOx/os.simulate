/*
pc2.c: 使用信号量解决生产者、计算者、消费者问题

    功能和前面的实验相同，使用信号量解决产者、计算者、消费者问题

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
    return buffer1_in == buffer2_out;
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

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sema, int value)
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex_sema1,mutex_sema2;
sema_t empty_buffer1_sema,empty_buffer2_sema;
sema_t full_buffer1_sema,full_buffer2_sema;

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
      sema_wait(&full_buffer2_sema);
        sema_wait(&mutex_sema2);
        item = get_item();
        printf("\t\tconsume item: %c\n", item);
        sema_signal(&mutex_sema2);
        sema_signal(&empty_buffer2_sema);
    }

    return NULL;
}

void *produce()
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        sema_wait(&empty_buffer1_sema);
        sema_wait(&mutex_sema1);

        item = i + 'a';
        calcu_put_item(item);
        printf("produce item: %c\n", item);

        sema_signal(&mutex_sema1);
        sema_signal(&full_buffer1_sema);
    }

    return NULL;
}

void *calculate(void * arg){
  //int i;
  int item;
  for(int i = 0 ; i < ITEM_COUNT ; i++){
    sema_wait(&full_buffer1_sema);
    //sema_wait(&empty_buffer1_sema);
      //sema_wait(&mutex_sema2);
      sema_wait(&mutex_sema1);
      item = calcu_get_item();
      sema_signal(&mutex_sema1);
      sema_signal(&empty_buffer1_sema);

      item = item - ('a' - 'A');
      printf("\tcalculate item:%c\n", item);

      sema_wait(&empty_buffer2_sema);
      sema_wait(&mutex_sema2);
      put_item(item);
      sema_signal(&mutex_sema2);
      sema_signal(&full_buffer2_sema);
  }
  return NULL;
}

int main(void){
  pthread_t consumer_tid,calculate_tid;

    sema_init(&mutex_sema1, 1);
    sema_init(&empty_buffer1_sema, CAPACITY - 1);
    sema_init(&full_buffer1_sema, 0);

    sema_init(&mutex_sema2, 1);
    sema_init(&empty_buffer2_sema, CAPACITY - 1);
    sema_init(&full_buffer2_sema, 0);

    pthread_create(&calculate_tid, NULL, calculate, NULL);
    pthread_create(&consumer_tid, NULL, consume, NULL);
    produce();
    pthread_join(calculate_tid, NULL);
    pthread_join(consumer_tid, NULL);
    //pthread_join(calculate_tid, NULL);
    return 0;
}
