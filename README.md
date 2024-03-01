# GameScript (GSC)
Compiler and virtual machine implementation in C++

Example script
```c
/*
	Usually these events or callbacks with function pointers get called from C++, but this serves as a example.
*/

my_coroutine1(dmg)
{
	// Coroutine instance will stop when this event happens.
	level endon("game_ended");
	
	// Coroutine will block until this event happens.
	level waittill("game_started");
	
	for(;;)
	{
		if(self.health <= 0) // Entity is dead
		{
			break;
		}
		self.health -= dmg;
		print("Health: " + self.health);
		wait 0.5; // Wait half a second.
	}
	[[ self.callbacks.dead ]]();
	print("Entity " + self.name + " is dead.");
}

callback()
{
	print("callback()");
}

main()
{
	
	e = spawnstruct();
	e.name = "Test Entity";
	e.health = 100;
	e.callbacks = spawnstruct();
	e.callbacks.dead = ::callback;
	
	dmg = 3;
	
	e thread my_coroutine1(dmg);
	
	wait 2; // Wait 2 seconds
	print("[starting game]");
	level notify("game_started"); // You can notify this through C++ aswell.
	
	wait 5;
	print("[ending game]");
	level notify("game_ended");
}
```

# Building
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Usage
```sh
$ ./gsc [-d <base_dir>] [-s <script_file>] [-f <function>] [--verbose]
-d <base_dir>: Specifies the base directory, default value is the current directory.
-s <script_file>: Specifies the script file.
-f <function>: Specifies the function, default function is main.
--verbose: Enable verbose mode.
```
# Example
```sh
$ ./gsc -s ../examples/hello_world --verbose
compiling program ../examples/hello_world
-------------------------------------------------------------------------------
Compile done!
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
