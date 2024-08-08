#include "ringbuffer.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>

int init_ringbuffer(ringbuffer* ringbuffer, size_t ringbuffer_size)
{

    ringbuffer->begin = (uint8_t*)malloc(ringbuffer_size);
    ringbuffer->ringbuffer_size = ringbuffer_size;
    if(ringbuffer->begin == NULL)
    {
        printf("failed init ringbuffer memory allocation");
        return FAILED_INIT;
    }
    ringbuffer->write = ringbuffer->begin;
    ringbuffer->read = ringbuffer->begin;
    ringbuffer->end = ringbuffer->begin + ringbuffer_size;
    if(pthread_mutex_init(&ringbuffer->mutex,NULL) != 0)
    {
        free(ringbuffer->begin);
        return FAILED_INIT;
    }
    if(    pthread_cond_init(&ringbuffer->cond,NULL) != 0)
    {
        free(ringbuffer->begin);
        return FAILED_INIT;
    }

    return SUCCESS;

}


void visualizeRingbuffer(ringbuffer ringbuffer,size_t ringbuffer_size)
{
    uint8_t* ptr = ringbuffer.begin;
    printf("%p %c",&ptr,*ptr);



}
size_t availableSpace(ringbuffer* ringbuffer)
{
    size_t memorySpace;

    if (ringbuffer->read <= ringbuffer->write)
    {
        memorySpace = (ringbuffer->end - ringbuffer->write) + (ringbuffer->read - ringbuffer->begin);
    }
    else
    {
        memorySpace = (ringbuffer->read - ringbuffer->write);
    }


    if (memorySpace > ringbuffer->ringbuffer_size)
    {
        return BUFFER_IS_FULL;
    }

    return memorySpace;
}

int write_ringbuffer(ringbuffer* ringbuffer, uint8_t* message, size_t messagelength)
{
    messagelength+=1;
    size_t availableSpaceAtTheMoment = availableSpace(ringbuffer);

    if (availableSpaceAtTheMoment<messagelength)
    {
        return FAILED_WRITE;
    }
    if(ringbuffer->write == ringbuffer->end)
    {
        ringbuffer->write = ringbuffer->begin;
    }
    if(ringbuffer->write == ringbuffer->end)
    {
        ringbuffer->write = ringbuffer->begin;
    }
    //with wrapper
    if(ringbuffer->write+messagelength>ringbuffer->end)
    {
        size_t messageToEnd = ringbuffer->end-ringbuffer->write;

        memcpy(ringbuffer->write,message,messageToEnd);
        ringbuffer->write = ringbuffer->begin;

        memcpy(ringbuffer->write,message+(messageToEnd),messagelength-messageToEnd);
        ringbuffer->write +=messagelength-messageToEnd;
    }

    else{
        memcpy(ringbuffer->write,message,messagelength);
        ringbuffer->write += messagelength;

    }
    return SUCCESS;

}
int string_length_from_ringbuffer(ringbuffer ringbuffer)
{
    int length = 0;
    uint8_t* ptr = &ringbuffer.read;
    while (*ptr != '\0')
    {
        length++;
        ptr++;
    }
    return length;
}
int read_ringbuffer(ringbuffer* ringbuffer)
{
    uint8_t* string;

    if(ringbuffer->read+1 == ringbuffer->write)
    {
        return FAILED_READ;
    }
    int stringlength = string_length_from_ringbuffer(*ringbuffer);
    for(int i = 0;i<stringlength;i++)
    {
        string[i] = ringbuffer->read;
        if(ringbuffer->read == ringbuffer->end)
        {
            ringbuffer->read = ringbuffer->begin;
        }
        else
        {
            ringbuffer->read++;
        }
    }
    printf("%s",string);
    /*
     * Der read pointer liest die nachricht es sei denn er ist hinter dem write pointer
     *
     */
    while(*ringbuffer->read != '\0')
    {

    }
    return SUCCESS;
}

int destroy_ringbuffer(ringbuffer* ringbuffer)
{
    free(ringbuffer->begin);

    pthread_mutex_destroy(&ringbuffer->mutex);
    pthread_cond_destroy(&ringbuffer->cond);
    return SUCCESS;
}
int main()
{
    ringbuffer ringbuffer;
    size_t ringbuffer_size = sizeof(uint8_t)*11;
    if (init_ringbuffer(&ringbuffer,ringbuffer_size) != SUCCESS) {
        printf("Failed to initialize ring buffer\n");
        return 1;
    }
    uint8_t* message1= 'fdsf';
    write_ringbuffer(&ringbuffer,message1,strlen(message1));






    visualizeRingbuffer(ringbuffer,ringbuffer.ringbuffer_size);

    printf("%zu",availableSpace(&ringbuffer));








    destroy_ringbuffer(&ringbuffer);

    return 0;


}