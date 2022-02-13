/**
 * File              : klist.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 13.02.2022
 * Last Modified Date: 13.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "klist.h"
#include <stdio.h>

char *k_list_err_str(KLIST_ERR err){
	char *str = malloc(BUFSIZ*sizeof(char));
	if (str == NULL) {
		perror("String malloc");
		exit(EXIT_FAILURE);
	}
	switch (err) {
		case KLIST_ERR_NONE: sprintf(str, "KLIST: No error"); break;
		case KLIST_ERR_NO_ITEM_AT_INDEX: sprintf(str, "KLIST: No item at selected index"); break;
		case KLIST_ERR_INDEX_OUT_OF_RANGE: sprintf(str, "KLIST: Index is out of range"); break;
		case KLIST_ERR_LIST_IS_EMPTY: sprintf(str, "KLIST: List is empty"); break;
	}
	
	return str;	
}

KLIST *k_list_allocate() {
	KLIST *list = malloc(sizeof(KLIST));
	if (list == NULL) {
		perror("KLIST malloc");
		exit(EXIT_FAILURE);
	}
	return list;
}

KLIST *k_list_new() {
	KLIST *list = k_list_allocate();
	list->data = NULL;
	list->prev = NULL;
	list->next = NULL;
	return list;
};

_Bool k_list_is_empty(KLIST *list) {
	if (list->prev == NULL && list->next == NULL && list->data == NULL) { //if list is empty	
		return true;
	}
	return false;
}

KLIST *k_list_first(KLIST *list) {
	KLIST *ptr = list;
	while (ptr->prev != NULL) {
		ptr = ptr->prev;
	}
	return ptr;
}

KLIST *k_list_last(KLIST *list) {
	KLIST *ptr = list;
	while (ptr->next != NULL) {
		ptr = ptr->next;
	}
	return ptr;
}

void k_list_copy_list(KLIST *list, KLIST **_new_list) {
	KLIST *new_list = k_list_new();
	new_list->data = list->data;
	new_list->prev = list->prev;
	new_list->next = list->next;
	*_new_list = new_list;
}

void k_list_append(KLIST *list, void *item){
	if (k_list_is_empty(list)) { //if list is empty
		list->data = item; //set data to list
		return;
	}
	KLIST *last = k_list_last(list);
	KLIST *new_list = k_list_allocate(); //create new_list
	new_list->data = item; //set up data
	last->next = new_list; //new_list is next to last
	new_list->prev = last; 
}

void k_list_prepend(KLIST *list, void *item){
	if (k_list_is_empty(list)) {
		list->data = item;
		return;
	}
	KLIST *ptr;
	k_list_copy_list(list, &ptr);
	KLIST *new_list = k_list_allocate(); //create new_list
	new_list->data = item;
	ptr->prev = new_list; //new_list is previous to first
	new_list->next = ptr;
	*list = *new_list;
}

void *k_list_item_at(KLIST *list, int index){
	KLIST *ptr = list;	
	int i;
	for (i = 0; ptr != NULL; ++i) {
		if (i == index) {
			return ptr->data;
		}	
		ptr=ptr->next;
	}
	return NULL;
}

void *k_list_first_item(KLIST *list) {
	return list->data;	
}

void *k_list_last_item(KLIST *list) {
	KLIST *ptr = k_list_last(list);
	return ptr->data;	
}

int k_list_size(KLIST *list){
	KLIST *ptr = list;	
	if (k_list_is_empty(list)) {
		return 0;
	}
	int i;
	for (i = 0; ptr != NULL; ++i) {
		ptr=ptr->next;
	}
	return i;
}

KLIST_ERR 
k_list_remove_item_at_index(KLIST *list, int index){
	if (k_list_is_empty(list)) {
		return KLIST_ERR_LIST_IS_EMPTY;	
	}
	if (index == 0) {
		KLIST *next = list->next;
		if (next == NULL) { //List has only one item
			list->prev = NULL;
			list->data = NULL;
			return KLIST_ERR_NONE;
		}
		next->prev = NULL;
		free(list);
		*list=*next;
		return KLIST_ERR_NONE;
	}
	int i;
	KLIST *ptr=list->next;
	for (i = 1; ptr != NULL; ++i) {
		if (i == index) {
			KLIST *prev = ptr->prev;	
			KLIST *next = ptr->next;
			free(ptr);
			prev->next = next;
			if (next != NULL) {
				next->prev = prev;
			}
			return KLIST_ERR_NONE;
		}	
		ptr=ptr->next;
	}

	return KLIST_ERR_NO_ITEM_AT_INDEX;
}

KLIST_ERR 
k_list_insert_item_at_index(KLIST *list, void *item, int index){
	if (index == 0) {
		k_list_prepend(list, item);
		return KLIST_ERR_NONE;
	}
	KLIST *ptr = list;
	int i;
	for (i = 0; ptr != NULL; ++i) {
		if (i == index) {
			KLIST *new_list = k_list_new();
			new_list->data = item;
			KLIST *next = ptr;
			KLIST *prev = ptr->prev; //pointer will be previous to new_list	
			prev->next = new_list;
			next->prev = new_list;
			new_list->next=next;
			new_list->prev=prev;
			
			return KLIST_ERR_NONE;
		}	
		ptr=ptr->next;
	}

	return KLIST_ERR_INDEX_OUT_OF_RANGE;
}

KLIST_ERR
k_list_move_item(KLIST *list, int from_index, int to_index){
	void *data = k_list_item_at(list, from_index);
	if (from_index < to_index)
		to_index -= 1;	

	KLIST_ERR err = k_list_remove_item_at_index(list, from_index); 
	if (err)
		return err;
	
	return k_list_insert_item_at_index(list, data, to_index);
}

int k_list_index_of_item(KLIST *list, void *item){
	if (item == NULL) {
		return -1;
	}
	KLIST *ptr = list;	
	if (ptr->data == item) {
		return 0;	
	}
	
	int i;
	for (i = 0; ptr != NULL; ++i) {
		if (ptr->data == item) {
			return i;	
		}
		ptr=ptr->next;
	}	
	return -1;
}

void k_list_free(KLIST *list){
	KLIST *ptr = list;	
	while (ptr != NULL) {
		KLIST *_list = ptr;
		ptr=ptr->next;
		free(_list);
	}
}

void k_list_foreach_item(KLIST *list, void *user_data, int (*callback)(void *item, void *user_data)){
	KLIST *ptr = list;
	while (ptr != NULL) {
		int c = callback(ptr->data, user_data);
		if (c != 0) {
			//stop callback if return not 0
			return;
		}
		ptr = ptr->next;
	}
}
