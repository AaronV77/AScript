all: astring_lib main

astring_lib:
	$(MAKE) -C AString/src archive

main:
	gcc -c dir_utility.c -g
	gcc -c env_linked_list.c -g 
	gcc -c td_array.c -g 
	gcc -c var_linked_list.c -g 
	gcc -c func_linked_list.c -g
	gcc -c calc_and_if.c -g -IAString/src/
	gcc -c ascript_core.c -g -IAString/src/
	gcc main2.c dir_utility.o env_linked_list.o td_array.o ascript_core.o var_linked_list.o func_linked_list.o calc_and_if.o -IAString/src/ AString/src/astring.a -o main2 -lm -g3

run:
	./main2 new_template.a

run_debug:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./main2 example_template.a -d

clean:
	rm -f *.o main main2
	$(MAKE) -C AString/src clean
