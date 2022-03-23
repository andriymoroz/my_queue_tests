#include <unistd.h>
#include <assert.h>
#include "Practice_thread_safe_Queue/my_queue.h"

//int my_queue_init(int size, my_queue_t *q);
//int my_queue_uninit(my_queue_t q);
//int my_queue_write(my_queue_t q, int val);
//int my_queue_read(my_queue_t q, int *val);

#define NNN 10

void *write_thread(void *args)
{
    int i;
    int x = 1;
    my_queue_t *q = (my_queue_t *)args;

    for(i = 0; i < NNN; i++) {
        printf("Write %d\n", x);
        if (my_queue_write(*q, x++) != 0) assert("Failed to write");
    }
    sleep(5);
    for(i = 0; i < NNN; i++) {
        printf("Write %d\n", x);
        if (my_queue_write(*q, x++) != 0) assert("Failed to write");
    }

    return NULL;
}

void *read_thread(void *args)
{
    int i;
    int e,s = 0;
    my_queue_t *q = (my_queue_t *)args;

    sleep(3);

    for(i = 0; i < 2*NNN; i++) {
        if (my_queue_read(*q, &e) != 0) assert("Failed to read");
        //sleep(1);
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

    if (my_queue_init(NNN/2, &q2) != 0) assert(!"Init failed");
    pthread_create(&wthread, NULL, write_thread, (void*)&q2);
    pthread_create(&rthread, NULL, read_thread, (void*)&q2);

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

    pthread_join(wthread, NULL);
    pthread_join(rthread, NULL);

    if (my_queue_uninit(q2) != 0) assert(!"Uninit failed");

    return 0;
}
