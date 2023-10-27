#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM 5

pthread_mutex_t forks[NUM];

sem_t semaphore;

void* problem_of_dining_philosophers (void* i) {
	int id = *((int*)i);
	int left_fork = id;
    	int right_fork = (id + 1) % NUM;
    	
    	while (1) {
		printf ("The philosopher %d thinks\n", id);
		sleep (1);
		
		sem_wait(&semaphore);

		pthread_mutex_lock(&forks[left_fork]);
		printf ("The philosopher %d takes left fork: %d\n", id, left_fork);
		sleep (1);

		pthread_mutex_lock(&forks[right_fork]);
		printf ("The philosopher %d takes right fork: %d\n", id, right_fork);

		printf ("The philosopher %d eats\n", id);
		sleep (1);

		pthread_mutex_unlock (&forks[right_fork]);
		pthread_mutex_unlock (&forks[left_fork]);
		printf ("The philosopher %d put down his forks\n", id);
		
		sem_post(&semaphore);
        }
        
        return NULL;
}

int main () {
	pthread_t philosophers[NUM];
	int id[NUM];
	
	sem_init(&semaphore, 0, NUM - 1);

	int i;
	for (i = 0; i < NUM; i++) {
		pthread_mutex_init (&forks[i], NULL);
	}

	for (i = 0; i < NUM; i++) {
		id[i] = i;
		pthread_create (&philosophers[i], NULL, problem_of_dining_philosophers, (void*)&id[i]);
	}
	
	for (i = 0; i < NUM; i++) {
		pthread_join (philosophers[i], NULL);
	} 

	for (i = 0; i < NUM; i++) {
		pthread_mutex_destroy (&forks[i]);
	} 
	
	sem_destroy(&semaphore);

	return 0;
}














