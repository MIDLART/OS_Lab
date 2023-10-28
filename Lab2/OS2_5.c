#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
	ok,
	invalid_arguments
} status_code;


typedef enum {
	No,
	Woman,
	Man
} occupied;

pthread_cond_t woman_cond, man_cond;
pthread_mutex_t mutex;

int in_bathroom = 0;
occupied by = No;

int conversion_to_int(char* argv) {
	int num = 0;
	size_t str_len = strlen(argv);
	for(int i = 0; i < str_len; i++) {
	if (isdigit(argv[i]) == 0) {
		return -1;
	}
	
	if (i == 9) {
		if (INT_MAX / 10 < num) {
		return -1;
		} else if (INT_MAX / 10 == num && argv[i] - '0' > 7) {
		return -1;
		}
	}
	num = num * 10 + argv[i] - '0';
	}
    return num;
}

void woman_wants_to_enter (int num) {
	pthread_mutex_lock(&mutex);
	while (in_bathroom == num || by == Man) {
		pthread_cond_wait(&woman_cond, &mutex);
	}
	printf("Woman entered the bathroom\n");
	by = Woman;
	in_bathroom++;
	pthread_mutex_unlock(&mutex);
}

void man_wants_to_enter (int num) {
	pthread_mutex_lock(&mutex);
	while (in_bathroom == num || by == Woman) {
		pthread_cond_wait(&man_cond, &mutex);
	}
	printf("Man entered the bathroom\n");
	by = Man;
	in_bathroom++;
	pthread_mutex_unlock(&mutex);
}

void woman_leaves () {
	pthread_mutex_lock(&mutex);
	in_bathroom--;
	printf("Woman leaves the bathroom\n");
	
	if (in_bathroom == 0) {
		by = No;
		printf("Bathroom is free\n");
		pthread_cond_broadcast(&man_cond);
	}
	
	pthread_mutex_unlock(&mutex);
}

void man_leaves () {
	pthread_mutex_lock(&mutex);
	in_bathroom--;
	printf("Man leaves the bathroom\n");
	
	if (in_bathroom == 0) {
		by = No;
		printf("Bathroom is free\n");
		pthread_cond_broadcast(&woman_cond);
	}
	
	pthread_mutex_unlock(&mutex);
}

void* thread_f(void* n) {
	int N = *((int*)n);
	usleep((rand() % 9 + 1) * 100000);
	woman_wants_to_enter(N);
	usleep(1000000);
	woman_leaves();
	return NULL;
}

void* thread_m(void* n) {
	int N = *((int*)n);
	usleep((rand() % 9 + 1) * 100000);
	man_wants_to_enter(N);
	usleep(1000000);
	man_leaves();
    	return NULL;
}

void solution (int mans, int womans, int N) {
	pthread_t man[mans], woman[womans];
	for (int i = 0; i < mans; i++) {
		pthread_create(&man[i], NULL, thread_m, (void*)&N);
	}
	for (int i = 0; i < womans; i++) {
		pthread_create(&woman[i], NULL, thread_f, (void*)&N);
	}
	for (int i = 0; i < mans; i++) {
		pthread_join(man[i], NULL);
	}
	for (int i = 0; i < womans; i++) {
		pthread_join(woman[i], NULL);
	}
}

int main (int argc, char* argv[]) {
	if (argc != 2) {
		printf("Invalid arguments\n");
		return invalid_arguments;
	}
	
	int N, mans, womans;
	N = conversion_to_int(argv[1]);
	
	printf("Enter number of mans:\n");
	scanf("%d", &mans);

	printf("Enter number of womans:\n");
	scanf("%d", &womans);

	if (N < 1 || mans < 1 || womans < 1) {
		printf("Invalid arguments\n");
		return invalid_arguments;
	}
	
	srand(time(NULL));
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&man_cond, NULL);
    	pthread_cond_init(&woman_cond, NULL);
	
	solution ((int)mans, (int)womans, (int)N);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&woman_cond);
	pthread_cond_destroy(&man_cond);
	
	return ok;
}

