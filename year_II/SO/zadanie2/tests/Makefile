all:
	nasm -f elf64 ../producer_consumer.asm -o producer_consumer.o
	nasm -f elf64 ../dijkstra_semaphore.asm -o dijikstra_semaphore.o
	gcc -c -std=gnu11 -lpthread factory.c -o factory.o
	gcc -std=gnu11 -lpthread *.o tests.c -o tests
	rm *.o
