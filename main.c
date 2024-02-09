#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"



/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	

	//printf("%ld %s", sizeof("hello" + sizeof("world")), "hello" + sizeof("world"));
	struct stack_t* lsStack;
	if(argc == 2){
		lsStack= constructLsStack(argv[1], "");
	}else if(argc == 3){
		lsStack = constructLsStack(argv[1], argv[2]);
	}else{
		printf("Invalid arguments. Format: ls2 filePath exactPatternMatch(optional)\n");
		return 0;
	}
	
	printstack(lsStack);
	freestack(lsStack);
	// stack stores the lines to print out
	/*stack_t *s = initstack();

	push(s, "Hello1");
	push(s, "Hello2");
	push(s, "Hello3");

	// print stack
	printstack(s);

	// free up stack
	freestack(s);
	*/
	return 0;
}
