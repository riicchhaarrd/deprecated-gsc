main()
{
	items = [];
	items[items.size] = 3;
	items[items.size] = 7;
	items[items.size] = 4;
	items["test"] = 123;
	
	print("items: " + items + ", typeof(items): " + typeof(items));
	dump(items);
	
	for(i = 0; i < items.size; i++)
	{
		print("Item " + i + ": " + items[i]);
	}
	
	keys = getarraykeys(items);
	for(i = 0; i < keys.size; i++)
	{
		print("Item " + keys[i] + ": " + items[i]);
	}
}