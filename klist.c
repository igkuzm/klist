/**
 * File              : klist.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 13.02.2022
 * Last Modified Date: 23.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "klist.h"
#include <stdarg.h>
#include <stdio.h>

char *k_list_err_str(KList_Err err){
	char *str = malloc(BUFSIZ*sizeof(char));
	if (str == NULL) {
		perror("String malloc");
		exit(EXIT_FAILURE);
	}
	switch (err) {
		case KLIST_ERR_NONE: sprintf(str, "KList: No error"); break;
		case KLIST_ERR_NO_ITEM_AT_INDEX: sprintf(str, "KList: No item at requested index"); break;
		case KLIST_ERR_INDEX_OUT_OF_RANGE: sprintf(str, "KList: Index is out of range"); break;
		case KLIST_ERR_LIST_IS_EMPTY: sprintf(str, "KList: List is empty"); break;
		case KLIST_ERR_NO_VALUE_IN_LIST: sprintf(str, "KList: List has no requested value"); break;
	}
	
	return str;	
}

KList *k_list_allocate() {
	KList *list = malloc(sizeof(KList));
	if (list == NULL) {
		perror("KList malloc");
		exit(EXIT_FAILURE);
	}
	return list;
}

KList *k_list_new() {
	KList *list = k_list_allocate();
	list->data = NULL;
	list->prev = NULL;
	list->next = NULL;
	return list;
};

_Bool k_list_is_empty(KList *list) {
	if (list->prev == NULL && list->next == NULL && list->data == NULL) { //if list is empty	
		return true;
	}
	return false;
}

KList *k_list_first(KList *list) {
	KList *ptr = list;
	while (ptr->prev != NULL) {
		ptr = ptr->prev;
	}
	return ptr;
}

KList *k_list_last(KList *list) {
	KList *ptr = list;
	while (ptr->next != NULL) {
		ptr = ptr->next;
	}
	return ptr;
}

void k_list_copy_list(KList *list, KList **_new_list) {
	KList *new_list = k_list_new();
	new_list->data = list->data;
	new_list->prev = list->prev;
	new_list->next = list->next;
	*_new_list = new_list;
}

void k_list_append(KList *list, void *item){
	if (k_list_is_empty(list)) { //if list is empty
		list->data = item; //set data to list
		return;
	}
	KList *last = k_list_last(list);
	KList *new_list = k_list_allocate(); //create new_list
	new_list->data = item; //set up data
	last->next = new_list; //new_list is next to last
	new_list->prev = last; 
}

void k_list_prepend(KList *list, void *item){
	if (k_list_is_empty(list)) {
		list->data = item;
		return;
	}
	KList *ptr;
	k_list_copy_list(list, &ptr);
	KList *new_list = k_list_allocate(); //create new_list
	new_list->data = item;
	ptr->prev = new_list; //new_list is previous to first
	new_list->next = ptr;
	*list = *new_list;
}

void *k_list_item_at(KList *list, int index){
	KList *ptr = list;	
	int i;
	for (i = 0; ptr != NULL; ++i) {
		if (i == index) {
			return ptr->data;
		}	
		ptr=ptr->next;
	}
	return NULL;
}

void *k_list_first_item(KList *list) {
	return list->data;	
}

void *k_list_last_item(KList *list) {
	KList *ptr = k_list_last(list);
	return ptr->data;	
}

int k_list_size(KList *list){
	KList *ptr = list;	
	if (k_list_is_empty(list)) {
		return 0;
	}
	int i;
	for (i = 0; ptr != NULL; ++i) {
		ptr=ptr->next;
	}
	return i;
}

KList_Err 
k_list_remove_item_at_index(KList *list, int index){
	if (k_list_is_empty(list)) {
		return KLIST_ERR_LIST_IS_EMPTY;	
	}
	if (index == 0) {
		KList *next = list->next;
		if (next == NULL) { //List has only one item
			list->prev = NULL;
			list->data = NULL;
			return KLIST_ERR_NONE;
		}
		next->prev = NULL;
		free(list);
		list=NULL;
		*list=*next;
		return KLIST_ERR_NONE;
	}
	int i;
	KList *ptr=list->next;
	for (i = 1; ptr != NULL; ++i) {
		if (i == index) {
			KList *prev = ptr->prev;	
			KList *next = ptr->next;
			free(ptr);
			ptr=NULL;
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

KList_Err 
k_list_remove_item(KList *list, void *item){
	if (k_list_is_empty(list)) {
		return KLIST_ERR_LIST_IS_EMPTY;	
	}
	KList *ptr = list; int i = 0;
	while (ptr != NULL) {
		if (ptr->data == item) {
			if (i == 0) {
				k_list_remove_item_at_index(list, 0);
				return KLIST_ERR_NONE;
			}
			KList *prev = ptr->prev;	
			KList *next = ptr->next;
			free(ptr);
			ptr=NULL;
			prev->next = next;
			if (next != NULL) {
				next->prev = prev;
			}
			return KLIST_ERR_NONE;
		}	
		ptr=ptr->next;
		i++;
	}

	return KLIST_ERR_NO_VALUE_IN_LIST;
}

void k_list_remove_all_items(KList *list){
	KList *ptr = k_list_last(list);
	while (ptr->prev != NULL) {
		KList *prev = ptr->prev;
		free(ptr);
		ptr=NULL;
		ptr = prev;
	}
	ptr->prev = NULL;
	ptr->next = NULL;
	ptr->data = NULL;
}

KList_Err 
k_list_insert_item_at_index(KList *list, void *item, int index){
	if (index <0) {
		k_list_append(list, item);
		return KLIST_ERR_NONE;
	}
	if (index == 0) {
		k_list_prepend(list, item);
		return KLIST_ERR_NONE;
	}
	KList *ptr = list;
	int i;
	for (i = 0; ptr != NULL; ++i) {
		if (i == index) {
			KList *new_list = k_list_new();
			new_list->data = item;
			KList *next = ptr;
			KList *prev = ptr->prev; //pointer will be previous to new_list	
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

KList_Err
k_list_move_item(KList *list, int from_index, int to_index){
	void *data = k_list_item_at(list, from_index);
	if (from_index < to_index)
		to_index -= 1;	

	KList_Err err = k_list_remove_item_at_index(list, from_index); 
	if (err)
		return err;
	
	return k_list_insert_item_at_index(list, data, to_index);
}

int k_list_index_of_item(KList *list, void *item){
	if (item == NULL) {
		return -1;
	}
	KList *ptr = list;	
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

void k_list_free(KList *list){
	k_list_remove_all_items(list);
	free(list);
	list=NULL;
}

int k_list_foreach_item(KList *list, void *user_data, int (*callback)(void *item, void *user_data)){
	KList *ptr = list;
	while (ptr != NULL) {
		int c = callback(ptr->data, user_data);
		if (c != 0) {
			//stop callback if return not 0
			return c;
		}
		ptr = ptr->next;
	}
	return 0;
}

void k_list_append_items(KList *list, ...){
	va_list valist;
	va_start(valist, list);
	void *item = va_arg(valist, void *);
	while (item != NULL) {
		k_list_append(list, item);
		item = va_arg(valist, void *);
	}
	va_end(valist);
}

