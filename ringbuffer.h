//
// Created by Marcel Léon Mrgic on 07.08.24.
//

#ifndef RINGBUFFER_H
#include <stdlib.h>
#include <pthread.h>
#define RINGBUFFER_H
#define SUCCESS 0
#define FAILED_INIT -1
#define FAILED_WRITE -2
#define FAILED_READ -2
#define BUFFER_IS_FULL 0


typedef struct ringbuffer
{
    size_t ringbuffer_size;
    uint8_t* begin;
    uint8_t* end;
    uint8_t* write;
    uint8_t* read;
    pthread_mutex_t mutex_read;
    pthread_mutex_t mutex_write;


}ringbuffer;
size_t availableSpace(ringbuffer* ringbuffer);
int init_ringbuffer(ringbuffer* ringbuffer, size_t ringbuffer_size);
int destroy_ringbuffer(ringbuffer* ringbuffer);
int write_ringbuffer(ringbuffer* ringbuffer, uint8_t* message, size_t messagelength);
uint8_t* read_ringbuffer(ringbuffer* ringbuffer);

#endif //RINGBUFFER_H
