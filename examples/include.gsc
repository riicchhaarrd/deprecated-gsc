// Usage: build/gsc -d examples/ -s include
// Because you can't specify ../ in function calls and includes.

#include util;

main()
{
	// Call functions from other files.
	hello_world::main();
	
	print("result = ", util_add(1, 2));
}