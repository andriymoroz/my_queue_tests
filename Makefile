CC=gcc
TRG=my_queue_test

.PHONY: default
default: my_queue_test

libmy_queue.so:
	$(MAKE) -C Practice_thread_safe_Queue

my_queue_test: main.o libmy_queue.so
	$(CC) main.c -L./Practice_thread_safe_Queue/ -Wl,-rpath=./Practice_thread_safe_Queue/ -lmy_queue -pthread -o $(TRG)

clean:
	$(RM) $(TRG) *.o
	$(MAKE) -C Practice_thread_safe_Queue clean
