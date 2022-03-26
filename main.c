#include <unistd.h>
#include <assert.h>
#include "Practice_thread_safe_Queue/my_queue.h"

//int my_queue_init(int size, my_queue_t *q);
//int my_queue_uninit(my_queue_t q);
//int my_queue_write(my_queue_t q, int val);
//int my_queue_read(my_queue_t q, int *val);

static int NNN = 10;

void *write_thread(void *args)
{
    int i;
    int x = 1;
    int s1 = rand() % 4;
    int s2 = rand() % 2;
    int s3 = rand() % 4;
    int s4 = rand() % 2;

    my_queue_t *q = (my_queue_t *)args;
    printf("Write: %d %d %d %d\n", s1, s2, s3, s4);

    sleep(s1);

    for(i = 0; i < NNN; i++) {
        printf("Write %d\n", x);
        sleep(s2);
        if (my_queue_write(*q, x++) != 0) assert("Failed to write");
    }
    sleep(s3);
    for(i = 0; i < NNN; i++) {
        printf("Write %d\n", x);
        sleep(s4);
        if (my_queue_write(*q, x++) != 0) assert("Failed to write");
    }

    return NULL;
}

void *read_thread(void *args)
{
    int i;
    int e,s = 0;
    int s1 = rand() % 4;
    int s2 = rand() % 2;

    my_queue_t *q = (my_queue_t *)args;

    printf("Read: %d %d\n", s1, s2);

    sleep(s1);

    for(i = 0; i < 2*NNN; i++) {
        if (my_queue_read(*q, &e) != 0) assert("Failed to read");
        sleep(s2);
        printf("Read %d\n", e);
        s += e;
    }

    assert(s == 2*NNN*NNN+NNN);

    return NULL;
}

int main(void)
{
    pthread_t wthread, rthread;
    my_queue_t q1, q2;
    int i,j,sw,sr,e;

    if (my_queue_init(-1, &q1) == 0) assert(!"Queue size negative");
    if (my_queue_init(0, &q1) == 0) assert(!"Queue size 0");

    if (my_queue_init(1, &q1) != 0) assert(!"Queue size 1");
    if (my_queue_write(q1, 1) != 0) assert("Failed to write");
    if (my_queue_read(q1, &e) != 0) assert("Failed to read");
    if (my_queue_write(q1, 1) != 0) assert("Failed to write");
    if (my_queue_read(q1, &e) != 0) assert("Failed to read");
    if (my_queue_uninit(q1) != 0) assert(!"Uninit failed");

    if (my_queue_uninit((my_queue_t)NULL) == 0) assert(!"Uninit NULL");
    if (my_queue_uninit((my_queue_t)1) == 0) assert(!"Uninit invalid");

    for (i = 1; i < NNN; i++) {
        if (my_queue_init(i, &q1) != 0) assert(!"Init failed");
        for (j = 0; j < i; j++) {
            if (my_queue_write(q1, j) != 0) assert("Failed to write");
        }
        for (j = 0; j < i; j++) {
            if (my_queue_read(q1, &e) != 0) assert("Failed to read");
            assert(j == e);
        }
        if (my_queue_uninit(q1) != 0) assert(!"Uninit failed");
    }


    for (i = 0; i < 100; i++) {
        NNN = rand() % 11 + 7;
        int qsize = rand() % (NNN/2) + 1;
        printf("Run %d, queue size: %d, items: %d\n", i, qsize, NNN);

        if (my_queue_init(qsize, &q2) != 0) assert(!"Init failed");
        pthread_create(&wthread, NULL, write_thread, (void*)&q2);
        pthread_create(&rthread, NULL, read_thread, (void*)&q2);

        pthread_join(wthread, NULL);
        pthread_join(rthread, NULL);

        if (my_queue_uninit(q2) != 0) assert(!"Uninit failed");
    }

    return 0;
}
