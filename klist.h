/**
 * File              : klist.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 13.02.2022
 * Last Modified Date: 14.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum k_list_err {
	KLIST_ERR_NONE = 0,
	KLIST_ERR_NO_ITEM_AT_INDEX,
	KLIST_ERR_INDEX_OUT_OF_RANGE,
	KLIST_ERR_LIST_IS_EMPTY
} KLIST_ERR;


typedef struct k_list_t {
	void *data;
	struct k_list_t *prev;
	struct k_list_t *next;
} KLIST;

char *k_list_err_str(KLIST_ERR err);

KLIST *k_list_new();

void k_list_free(KLIST *list);

void k_list_copy_list(KLIST *list, KLIST **_new_list);

bool k_list_is_empty(KLIST *list);

void k_list_append(KLIST *list, void *item);

//append NULL-terminated list of items (item0, item1, NULL)
void k_list_append_items(KLIST *list, ...);

void k_list_prepend(KLIST *list, void *item);

void *k_list_item_at(KLIST *list, int index);

void *k_list_first_item(KLIST *list);

void *k_list_last_item(KLIST *list);

int k_list_size(KLIST *list);

KLIST_ERR k_list_remove_item_at_index(KLIST *list, int index);

KLIST_ERR k_list_insert_item_at_index(KLIST *list, void *item, int index);

KLIST_ERR k_list_move_item(KLIST *list, int from_index, int to_index);

int k_list_index_of_item(KLIST *list, void *item);

void k_list_foreach_item(KLIST *list, void *user_data, int (*callback)(void *item, void *user_data));
