all: astring_lib main

astring_lib:
	$(MAKE) -C AString/src archive

main:
	gcc -c dir_utility.c -g
	gcc -c env_linked_list.c -g 
	gcc -c td_array.c -g 
	gcc -c var_linked_list.c -g 
	gcc -c func_linked_list.c -g
	gcc main.c dir_utility.o env_linked_list.o td_array.o var_linked_list.o func_linked_list.o -IAString/src/ AString/src/astring.a -o main -g3

run:
	./main example_template.a

run_debug:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./main example_template.a -d

clean:
	rm -f *.o main
	$(MAKE) -C AString/src clean
