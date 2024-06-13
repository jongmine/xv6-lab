#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common_threads.h"

//
// Your code goes in the structure and functions below
//

typedef struct __rwlock_t {
    sem_t lock;        // 기본 잠금 (binary semaphore)
    sem_t writelock;   // 쓰기 잠금, 하나의 작가만 가능
    sem_t no_writer;   // 작가 대기 중일 때 새로운 독자 차단
    int readers;       // 현재 읽기 중인 독자 수
    int writers_waiting; // 대기 중인 작가 수
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
    rw->readers = 0;
    rw->writers_waiting = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
    sem_init(&rw->no_writer, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    sem_wait(&rw->no_writer); // 작가가 대기 중일 경우 새로운 독자 차단
    sem_wait(&rw->lock);
    rw->readers++;
    if (rw->readers == 1) {
        sem_wait(&rw->writelock); // 첫 번째 독자가 쓰기 잠금 획득
    }
    sem_post(&rw->lock);
    sem_post(&rw->no_writer); // 작가가 대기 중이 아니면 새로운 독자 허용
}

void rwlock_release_readlock(rwlock_t *rw) {
    sem_wait(&rw->lock);
    rw->readers--;
    if (rw->readers == 0) {
        sem_post(&rw->writelock); // 마지막 독자가 쓰기 잠금을 해제
    }
    sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    sem_wait(&rw->lock);
    rw->writers_waiting++; // 대기 중인 작가 수 증가
    if (rw->writers_waiting == 1) {
        sem_wait(&rw->no_writer); // 새로운 독자가 접근하지 못하게 함
    }
    sem_post(&rw->lock);
    sem_wait(&rw->writelock); // 쓰기 잠금 획득
}

void rwlock_release_writelock(rwlock_t *rw) {
    sem_post(&rw->writelock);
    sem_wait(&rw->lock);
    rw->writers_waiting--; // 대기 중인 작가 수 감소
    if (rw->writers_waiting == 0) {
        sem_post(&rw->no_writer); // 더 이상 대기 중인 작가가 없으면 새로운 독자 허용
    }
    sem_post(&rw->lock);
}

//
// Don't change the code below (just use it!)
//

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_readlock(&lock);
	printf("read %d\n", value);
	rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_writelock(&lock);
	value++;
	printf("write %d\n", value);
	rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
	Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
	Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_join(pw[i], NULL);

    printf("end: value %d\n", value);

    return 0;
}

