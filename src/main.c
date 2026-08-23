#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "read.h"
#include "en_dc.h"
#include "read_file.h"
#include "drive.h"
#define NUM_PRODUCERS 1 
#define NUM_CONSUMERS 3 

Message_Queue queue;
Shared_Buffer shared_buffer;
ReadWrite_Lock lock;
pthread_mutex_t message_mutex;
pthread_cond_t message_available;
unsigned long message_generation=0;
int producer_finished=0;
// uint8_t input_msg[max_size]={10,20,30,40,50};
void *producer(void *arg){
  InputFile input;
  FileArgs *args=(FileArgs *) arg;
  int id=args->id;

  if (input_file_open(&input, args->filename) != 0) {
        printf("Failed to open %s\n", args->filename);
        return NULL;
  }

  float x_coord;
  float y_coord;
  while(input_file_read(&input,&x_coord,&y_coord)){
    uint8_t input_msg[sizeof(float)*2];
    memcpy(input_msg,&x_coord,sizeof(float));
    memcpy(input_msg+sizeof(float),&y_coord,sizeof(float));
    Message msg={0};
    cobs_encode_result result =cobs_encode(msg.data,sizeof(msg.data),input_msg,sizeof(input_msg));
    if(result.status!=COBS_ENCODE_OK){
      printf("COBS Encoding failed \n");
      return NULL;
    }
    msg.length=result.out_len;
    printf("Encoded message length : %zu\n",msg.length);
    // msg.length=4;
    // msg.data[0]=i;
    // msg.data[1]=i+10;
    // msg.data[2]=40;
    // msg.data[3]=id;
    // printf("Writing values : %u %u %u %u %u \n",msg.data[0],msg.data[1],msg.data[2],msg.data[3],msg.data[4]);
    printf("Encoded: ");

    for (size_t j = 0; j < msg.length; j++) {
      printf("%u ", msg.data[j]);
    };
    printf("\n");
    writer_enter(&lock);
    memcpy(shared_buffer.data,msg.data,msg.length);
    shared_buffer.length=msg.length;
    writer_exit(&lock);
    pthread_mutex_lock(&message_mutex);
    message_generation++;
    pthread_cond_broadcast(&message_available);
    pthread_mutex_unlock(&message_mutex);
    usleep(1000);
  }
  pthread_mutex_lock(&message_mutex);
  producer_finished = 1;
  pthread_cond_broadcast(&message_available);
  pthread_mutex_unlock(&message_mutex);
  return NULL;
}

void *consumer(void *arg){
  int id=*(int *)arg;
  Message decode_msg;
  unsigned long last_generation=0;
  for(int i=0;i<10;i++){
    Message msg={0};
    pthread_mutex_lock(&message_mutex);
    while(message_generation==last_generation && !producer_finished){
      pthread_cond_wait(&message_available,&message_mutex);
    };

    if (message_generation == last_generation && producer_finished) {
      pthread_mutex_unlock(&message_mutex);
      break;
    }
    last_generation=message_generation;
    pthread_mutex_unlock(&message_mutex);
    reader_enter(&lock);
    msg.length=shared_buffer.length;
    memcpy(msg.data,shared_buffer.data,shared_buffer.length);
    reader_exit(&lock);
    cobs_decode_result result = cobs_decode(decode_msg.data,sizeof(decode_msg.data),msg.data,msg.length);
    if(result.status !=COBS_DECODE_OK){
        printf("COBS decoding failed \n");
        continue;
    };
    decode_msg.length=result.out_len;
    float x_coord;
    float y_coord;
    memcpy(&x_coord,decode_msg.data,sizeof(float));
    memcpy(&y_coord,decode_msg.data+sizeof(float),sizeof(float));
    printf(
        "Consumer %d: x = %.2f y = %.2f\n",
        id,
        x_coord,
        y_coord
    );
    if(id==1){
      message_queue_push(&queue,&decode_msg);
    }

    // int *data=(int *) msg.data;
    // printf("Consumer %d popped message : message=%u %u %u %u %u  \n",id , decode_msg.data[0],decode_msg.data[1],decode_msg.data[2],decode_msg.data[3],decode_msg.data[4]);
  };
  return NULL;
}

void *drive_write(void *arg){
  int id=*(int *)arg;
  InputFile input;
  FileArgs *args=(FileArgs *) arg;
  if (input_file_open_write(&input, args->result_filename) != 0) {
    printf("Failed to open %s\n", args->result_filename);
    return NULL;
  }
  for(int i=0;i<10;i++){
    Message msg={0};
    message_queue_pop(&queue,&msg);
    float x;
    float y;
    memcpy(&x,msg.data,sizeof(float));
    memcpy(&y,msg.data+sizeof(float),sizeof(float));

    struct coordinate coordinate_target={
      .latitude=x,
      .longitude=y,
      .altitude=0.0f
    };
    struct rover_state rover ={
      .position={
        .latitude=0.0f,
        .longitude=0.0f,
        .altitude=0.0f
      },
      .heading_rad=0.0f
    };
    enum drive_status result_status = drive_to_target(&rover,&coordinate_target);
    float distance=0;
    float x_aggregate=rover.position.latitude-x;
    float y_aggregate=rover.position.longitude-y;
    distance=sqrt(pow(x_aggregate,2)+pow(y_aggregate,2));
    float dx=coordinate_target.latitude-rover.position.latitude;
    float dy=coordinate_target.longitude-rover.position.longitude;
    float error =hypotf(dx,dy);
    int status=1;
    if (result_status==DRIVE_REACHED_TARGET && error<=0.10){
      status=0;
    }
    input_file_write(&input,&rover.position.latitude,&rover.position.longitude,&error,&status);
    printf("Driver %d : target= {%.2f , %.2f} , rover={%.2f,%.2f} , error : %f \n",id,x,y,rover.position.latitude,rover.position.longitude,error);
    // if(result_status==DRIVE_REACHED_TARGET && error<=0.10){
    //   printf("Success \n");
    // }
  }
  return NULL;
}


int main(){
  pthread_t producers[NUM_PRODUCERS];
  pthread_t consumers[NUM_CONSUMERS];
  pthread_t drive_writers[NUM_PRODUCERS];
  int producers_id[NUM_PRODUCERS] ={1};
  int consumer_id[NUM_CONSUMERS]={1,2,3};
  int writer_id[NUM_PRODUCERS]={1};
  const char *testcases[]={
    "input/testcase1.txt",
    "input/testcase2.txt",
    "input/testcase3.txt",
    "input/testcase4.txt"
  };
  const char *result_tc[]={
    "result/result1.txt",
    "result/result2.txt",
    "result/result3.txt",
    "result/result4.txt"
  };
  if(rwlock_init(&lock) !=0 ){
    printf("Reader writer synchrnization failed \n");
    return 1;
  }
  if(message_queue_init(&queue)!=0){
    printf("Queue Initialization failed \n");
    return 1;
  }
  if(pthread_cond_init(&message_available,NULL)!=0){
    printf("Condition mutex Initialization failed\n");
    return 1;

  }
  for(int i=0;i<4;i++){
    printf("Input : %d \n",i+1);
    printf("\n");
    printf("\n");
    FileArgs file_args={
      .id=1,
      .filename=testcases[i],
      .result_filename=result_tc[i]
    };
    message_generation=0;
    producer_finished=0;
    pthread_create(&producers[0],NULL,producer,&file_args);
    for(int i=0;i<NUM_CONSUMERS;i++){
      pthread_create(&consumers[i],NULL,consumer,&consumer_id[i]);

    };

    for(int i = 0; i < NUM_PRODUCERS; i++) {
      pthread_create(
        &drive_writers[i],
        NULL,
        drive_write,
        &file_args
      );
    };
    for(int i=0;i<NUM_PRODUCERS;i++){
      pthread_join(producers[i],NULL);
    };
    for(int i=0;i<NUM_CONSUMERS;i++){
      pthread_join(consumers[i],NULL);
    };
    for (int i = 0; i < NUM_PRODUCERS; i++) {
      pthread_join(drive_writers[i], NULL);
    };
 

  };
  rwlock_destroy(&lock);
  message_destroy(&queue);


}
