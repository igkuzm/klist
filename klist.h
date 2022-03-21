/**
 * File              : klist.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 13.02.2022
 * Last Modified Date: 21.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum k_list_err {
	KLIST_ERR_NONE = 0,
	KLIST_ERR_NO_ITEM_AT_INDEX,
	KLIST_ERR_INDEX_OUT_OF_RANGE,
	KLIST_ERR_LIST_IS_EMPTY,
	KLIST_ERR_NO_VALUE_IN_LIST
} KList_Err;


typedef struct k_list_t {
	void *data;
	struct k_list_t *prev;
	struct k_list_t *next;
} KList;

#define FOR_EACH(item, list) \
    for (KList *(item) = (list); (item); (item) = (item)->next)

//return error description
char *k_list_err_str(KList_Err err);

//crate new list array
KList *k_list_new();

//free memory list
void k_list_free(KList *list);

//check if list is empty
bool k_list_is_empty(KList *list);

//add item to the end of list
void k_list_append(KList *list, void *item);

//add items to the end of list (NULL terminated). like: (item0, item1, NULL)
void k_list_append_items(KList *list, ...);

//add item to the start of list
void k_list_prepend(KList *list, void *item);

//return item at index
void *k_list_item_at(KList *list, int index);

//return first item of list
void *k_list_first_item(KList *list);

//resturn last item of list
void *k_list_last_item(KList *list);

//return number of items in list
int k_list_size(KList *list);

//remove item at index - return error code
KList_Err k_list_remove_item_at_index(KList *list, int index);

//remove item - return error code
KList_Err k_list_remove_item(KList *list, void *item);

//remove all items
void k_list_remove_all_items(KList *list);

//insert item at index to list. If index -1 - append to list. Return error code
KList_Err k_list_insert_item_at_index(KList *list, void *item, int index);

//move item from index to index. Return error code.
KList_Err k_list_move_item(KList *list, int from_index, int to_index);

//return index of item
int k_list_index_of_item(KList *list, void *item);

//run callback for each item in list. Return non-zero in callback function to stop it.
//the main function returns the callback return
int k_list_foreach_item(KList *list, void *user_data, int (*callback)(void *item, void *user_data));
