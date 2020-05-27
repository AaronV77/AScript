#include "func_vars_list.h"

int main() {

	Func_Var_List * temp = NULL;
	//temp = NULL;
	
	func_var_push(&temp, "function", "int", "int");
	
	printf("Type: %s\n", temp->front->name);
	printf("Datatype: %s\n", temp->front->datatype);
	printf("Argument-1: %s\n", ((char**)temp->front->arguments->array)[0]);

	func_var_argument_push(&temp, "function", "string");
	printf("Argument-2: %s\n", ((char**)temp->front->arguments->array)[1]);

	func_var_cleanup(&temp);

	return 0;
}
