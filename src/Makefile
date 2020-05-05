all:
	gcc -c string.c
	gcc -c dir_utility.c
	gcc -c linked_list.c
	gcc main.c string.o dir_utility.o linked_list.o -o main

debug:
	gcc -c string.c -g
	gcc -c dir_utility.c -g
	gcc -c linked_list.c -g
	gcc main.c string.o dir_utility.o linked_list.o -o main -g

run:
	./main template_example.a

run_debug:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./main template_example.a -d

clean:
	rm *.o main
