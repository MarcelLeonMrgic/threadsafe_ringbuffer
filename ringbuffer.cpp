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
    ringbuffer->end = ringbuffer->begin + ringbuffer_size-sizeof(u_int8_t);
    if(pthread_mutex_init(&ringbuffer->mutex_read,NULL) != 0)
    {
        free(ringbuffer->begin);
        return FAILED_INIT;
    }


    return SUCCESS;

}


void visualizeRingbuffer(ringbuffer ringbuffer,size_t ringbuffer_size)
{
    uint8_t* ptr = ringbuffer.begin;
    while(ptr != ringbuffer.end+1)
    {
        if(ptr != ringbuffer.begin && ptr != ringbuffer.write && ptr != ringbuffer.read && ptr != ringbuffer.end)
        {
            printf("%p %c \n",ptr,*ptr);
        }
        else if(ptr == ringbuffer.begin)
        {
            if(ptr == ringbuffer.write && ptr==ringbuffer.read)
            {
                printf("%p %c <-- begin write read\n",ptr,*ptr);
            }
            else if(ptr == ringbuffer.write)
            {
                printf("%p %c <-- begin write\n",ptr,*ptr);
            }
            else if(ptr == ringbuffer.read)
            {
                printf("%p %c <-- begin read\n",ptr,*ptr);
            }
            else
            {
                printf("%p %c <-- begin\n",ptr,*ptr);
            }
        }
        else if(ptr == ringbuffer.end)
        {
            if(ptr == ringbuffer.write && ptr==ringbuffer.read)
            {
                printf("%p %c <-- end write read\n",ptr,*ptr);
            }
            else if(ptr == ringbuffer.write)
            {
                printf("%p %c <-- end write\n",ptr,*ptr);
            }
            else if(ptr == ringbuffer.read)
            {
                printf("%p %c <-- end read\n",ptr,*ptr);
            }
            else
            {
                printf("%p %c <-- end\n",ptr,*ptr);
            }
        }
        else if(ptr == ringbuffer.write)
        {
            if(ptr == ringbuffer.read)
            {
                printf("%p %c <-- write read\n",ptr,*ptr);
            }
            else
            {
                printf("%p %c <-- write\n",ptr,*ptr);
            }
        }
        else if(ptr == ringbuffer.read)
        {
            printf("%p %c <-- read \n",ptr,*ptr);
        }
        ptr++;
    }




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
size_t string_length_from_ringbuffer(ringbuffer ringbuffer)
{
    int length = 0;
    uint8_t* ptr = ringbuffer.read;
    while (*ptr != '\0')
    {

        if(ptr == ringbuffer.end)
        {
            ptr = ringbuffer.begin;
        }
        else
        {
            ptr++;
        }

        length++;
    }
    return length+1;//plus one because of '\0'
}

uint8_t* read_ringbuffer(ringbuffer* ringbuffer)
{

    size_t messagelength =string_length_from_ringbuffer(*ringbuffer);
    uint8_t* string = (uint8_t*)malloc(messagelength);
    if(ringbuffer->read+messagelength>ringbuffer->end)
    {
        size_t messageToEnd = ringbuffer->end-ringbuffer->read;
        memcpy(string,ringbuffer->read,messageToEnd);
        ringbuffer->read = ringbuffer->begin;

        memcpy(string+messageToEnd,ringbuffer->read,messagelength-messageToEnd);
        ringbuffer->read +=messagelength-messageToEnd;

        return string;

    }

        memcpy(string,ringbuffer->read,messagelength);
        ringbuffer->read += messagelength;
        return string;





}

int destroy_ringbuffer(ringbuffer* ringbuffer)
{
    free(ringbuffer->begin);

    pthread_mutex_destroy(&ringbuffer->mutex_read);

    return SUCCESS;
}
