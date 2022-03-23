#include <assert.h>
#include "Practice_thread_safe_Queue/my_queue.h"


int main(void)
{
    my_queue_t q1;

    if (my_queue_init(-1, &q1) == 0) assert(!"Queue size negative");
    if (my_queue_init(0, &q1) == 0) assert(!"Queue size 0");

    if (my_queue_init(1, &q1) != 0) assert(!"Queue size 1");
    if (my_queue_init(10, &q1) != 0) assert(!"Queue size 10");

    if (my_queue_uninit((my_queue_t)NULL) == 0) assert(!"Uninit NULL");
    if (my_queue_uninit((my_queue_t)1) == 0) assert(!"Uninit invalid");

    // ...

    return 0;
}
