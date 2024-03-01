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