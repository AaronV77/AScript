
all: astring_lib main

astring_lib:
	$(MAKE) -C AString/src archive

main:
	gcc -c dir_utility.c
	gcc -c linked_list.c
	gcc -c td_array.c
	gcc -c func_vars_list.c	
	gcc main.c dir_utility.o linked_list.o td_array.o func_vars_list.o -IAString/src/ AString/src/astring.a -o main

run:
	./main example_template.a

run_debug:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./main example_template.a -d

clean:
	rm -f *.o main
	$(MAKE) -C AString/src clean
