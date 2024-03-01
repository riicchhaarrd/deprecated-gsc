# GameScript (GSC)
Compiler and virtual machine implementation in C++

# Building
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Usage
```sh

$ ./gsc ../examples/hello_world
-------------------------------------------------------------------------------
                -->PushString Hello world!\n (0)        ../examples/hello_world::main
                -->CallFunction print (1)       ../examples/hello_world::main
[./../examples/hello_world.gsc:0] Hello world!\n
                -->Pop (1)      ../examples/hello_world::main
                -->PushUndefined (0)    ../examples/hello_world::main
                -->Ret (1)      ../examples/hello_world::main
```

# Adding a new function to GSC
You can add a new map of functions, but the easiest way is to add a new function in ```src/script/stockfunctions.cpp``` by adding a new entry to ```stockfunctions```.

```c
// Function which adds to numbers together
// Usage: result = my_function(1, 2);
// result will be 3
int my_function(script::VMContext& ctx)
{
	int lhs = ctx.get_int(0);
	int rhs = ctx.get_int(1);
	ctx.add_int(lhs + rhs);
	return 1;
}

std::unordered_map<std::string, StockFunction> stockfunctions = {
	...
	{"my_function", my_function},
	...
};
```
