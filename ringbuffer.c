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


void visualizeRingbuffer(ringbuffer* ringbuffer,size_t ringbuffer_size)
{
    u_int8_t* ptr = ringbuffer->begin;
    for(int i = 0;i<ringbuffer_size/sizeof(u_int8_t)*2;i++)
    {

            if(ptr == ringbuffer->begin && ptr==ringbuffer->read && ptr==ringbuffer->write)
            {
                printf("%p %c <---- begin   read   write \n",ptr ,*ptr);

            }
            else if(ptr == ringbuffer->begin && ptr == ringbuffer->write){
                printf("%p %c <---- begin   write \n",ptr ,*ptr);
            }
            else if(ptr == ringbuffer->begin && ptr == ringbuffer->read){
                printf("%p %c <---- begin   read \n",ptr ,*ptr);
            }
            else if(ptr == ringbuffer->begin){
                printf("%p %c <---- begin \n",ptr ,*ptr);
            }
            else if(ptr+1 == ringbuffer->end && ptr+1==ringbuffer->read && ptr+1==ringbuffer->write)
            {
                printf("%p %c <---- end   read   write \n",ptr ,*ptr);

            }
            else if(ptr+1 == ringbuffer->end && ptr+1 == ringbuffer->write){
                printf("%p %c <---- end   write \n",ptr ,*ptr);
            }
            else if(ptr+1 == ringbuffer->end && ptr+1 == ringbuffer->read){
                printf("%p %c <---- end   read \n",ptr ,*ptr);
            }
            else if(ptr+1 == ringbuffer->end){
                printf("%p %c <---- end \n",ptr ,*ptr);
            }
            else if(ptr == ringbuffer->read && ptr == ringbuffer->write){
                printf("%p %c <---- read   write \n",ptr ,*ptr);
            }
            else if(ptr == ringbuffer->read){
                printf("%p %c <---- read \n",ptr ,*ptr);
            }
            else if(ptr == ringbuffer->write){
                printf("%p %c <---- write \n",ptr ,*ptr);
            }
        else
        {
            printf("%p %c \n",ptr ,*ptr);
        }


        ptr++;
        if(ptr == ringbuffer->end)
        {
            ptr = ringbuffer->begin;
            printf("\n");
        }
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

    // Check if the ring buffer is full
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
        return BUFFER_IS_FULL;
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
    size_t ringbuffer_size = sizeof(uint8_t)*10;
    if (init_ringbuffer(&ringbuffer,ringbuffer_size) != SUCCESS) {
        printf("Failed to initialize ring buffer\n");
        return 1;
    }
    uint8_t* messagering = "hell";
    uint8_t* messagering2 = "aaaaa";
    uint8_t* messagering3 = "aa";






    write_ringbuffer(&ringbuffer,messagering,strlen(messagering));
    write_ringbuffer(&ringbuffer,messagering,strlen(messagering));
    write_ringbuffer(&ringbuffer,messagering2,strlen(messagering2));
    ringbuffer.read++;
    ringbuffer.read++;
    ringbuffer.read++;
    ringbuffer.read++;
    write_ringbuffer(&ringbuffer,messagering3,strlen(messagering3));

    visualizeRingbuffer(&ringbuffer,ringbuffer.ringbuffer_size);

    printf("%zu",availableSpace(&ringbuffer));








    destroy_ringbuffer(&ringbuffer);

    return 0;


}