#include "func_vars_list.h"

int main() {

	Func_Var_List * temp = NULL;
	
	func_var_push(&temp, "function", "int");
	func_var_argument_push(&temp, "function", "Aaron");
	func_var_argument_push(&temp, "function", "Anthony");
	func_var_argument_push(&temp, "function", "Valoroso");
	
	printf("Type: %s\n", temp->front->name);
	printf("Datatype: %s\n", temp->front->datatype);

	func_var_argument_push(&temp, "function", "string");
	for (int i = 0; i < temp->front->arguments->current_num_rows; ++i)
		printf("Argument-%d: %s\n", i, ((char**)temp->front->arguments->array)[i]);

	func_var_cleanup(&temp);

	return 0;
}
