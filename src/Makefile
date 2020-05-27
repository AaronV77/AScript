all:
	gcc -c astring.c
	gcc -c dir_utility.c
	gcc -c linked_list.c
	gcc -c td_array.c
	gcc -c func_vars_list.c	
	gcc main.c astring.o dir_utility.o linked_list.o td_array.o func_vars_list.o -o main

debug:
	gcc -c astring.c -g
	gcc -c dir_utility.c -g
	gcc -c linked_list.c -g
	gcc -c func_vars_list.c -g
	gcc main.c string.o dir_utility.o linked_list.o func_vars_list.o -o main -g

run:
	./main example_template.a

run_debug:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./main template_example.a -d

clean:
	rm *.o main
