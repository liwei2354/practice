#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define THREAD_NUMBER 4

typedef void * (*threadFunc_t)(void *);

pthread_t tidArr[THREAD_NUMBER];

int flag = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * threadFunc1(void * arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(1 == flag)
		{
			fprintf(stdout,"%c",'A');
			fflush(NULL);
			flag = 2;
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	
	pthread_exit(NULL);
}

void * threadFunc2(void * arg)
{
	while(1)
	{ 
		pthread_mutex_lock(&mutex);
		if(2 == flag)
		{
			fprintf(stdout,"%c",'B');
			fflush(NULL);
			flag = 3;
		}
		
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	
	pthread_exit(NULL);
}

void * threadFunc3(void * arg)
{
	while(1)
	{ 
		pthread_mutex_lock(&mutex);
		if(3 == flag)
		{
			fprintf(stdout,"%c",'C');
			fflush(NULL);
			flag = 4;
		}
		
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	
	pthread_exit(NULL);
}

void * threadFunc4(void * arg)
{
	while(1)
	{ 
		pthread_mutex_lock(&mutex);
		if(4 == flag)
		{
			fprintf(stdout,"%c",'D');
			fflush(NULL);
			flag = 1;
		}
		
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	
	pthread_exit(NULL);
}

void sighandler(int signum)
{
	fprintf(stdout,"SIGINT received!\n");
	int i;
    for(i = 0;i < THREAD_NUMBER; i++)
	{
		pthread_cancel(tidArr[i]);
	}
}

int main(int argc,char **argv)
{
    pthread_t *ret;
	threadFunc_t threadFuncArr[] = {threadFunc1,threadFunc2,threadFunc3,threadFunc4};
	
	int i;
 
	signal(SIGINT,sighandler);

	for(i = 0;i < THREAD_NUMBER; i++)
	{
		//pthread_create(tidArr + i,NULL,threadFunc,(void *)('A'+i));
		pthread_create(tidArr + i,NULL,threadFuncArr[i],NULL);
	}

     
	for(i = 0;i < THREAD_NUMBER; i++)
	{
		pthread_join(tidArr[i],(void **)&ret);
		//fprintf(stdout,"thread id %d exit!",*ret);
		//free(ret);
	}
	
	while(1)
	{
		pause();
	}
	pthread_mutex_destroy(&mutex);
}



