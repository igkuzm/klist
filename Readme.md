**Array container for C**

List array (std::vector like) container written in pure C.
Simpe example:

```
#include "klist.h"

int callback(void *item, void *user_data) {
	printf("%s ", (char *)item);
	return 0;
}

int main(int argc, char *argv[])
{
	KLIST *list = k_list_new(); //create new container
	k_list_append_items(list, "Hello", "World", "!", NULL); //add items to container
	k_list_prepend(list, "####"); //add item to the start of list

	k_list_foreach_item(list, NULL, callback);//run callback for each item in list
	printf("\n");

	k_list_free(list); //free memory

	
return 0;
}
```

