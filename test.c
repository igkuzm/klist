/**
 * File              : test.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 13.02.2022
 * Last Modified Date: 14.02.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "klist.h"
#include <malloc/_malloc.h>
#include <stdio.h>
#include <stdlib.h>

char readChar(){
	char a;
	scanf("%c", &a);
	return a;	
}

char *readString(){
	int i = 0;
	char *a = (char *) calloc(BUFSIZ, sizeof(char));
	if (!a) {
		fprintf(stderr, "ERROR. Cannot allocate memory\n");		
		return NULL;
	}										
	
	while (1) {
		scanf("%c", &a[i]);
		if (a[i] == '\n') {
			break;
		}
		else {
			i++;
		}
	}
	a[i] = '\0';
	return a;
}

void print_header(){
	system("clear");	
	printf("#######################################\n");
	printf("LIST test\n");
	printf("#######################################\n");
	printf("\n");
}

int print_list_callback(void *item, void *user_data) {
	int *i = user_data;
	char *text = item;
	if (text == NULL) {
		text = "NULL";
	}	
	printf("%d.\t%s\n", *i, text);
	i[0]++;

	return 0;
}

void print_list(KLIST *list){
	//int *i = malloc(sizeof(int));
	//if (i==NULL) {
		//perror("i malloc");
		//exit(EXIT_FAILURE);
	//}
	printf("#######################################\n");
	printf("KLIST ITEMS:\n");
	if (k_list_is_empty(list)) {
		printf("NULL\n");
	} else {
		int i = 0;
		k_list_foreach_item(list, &i, print_list_callback);
	}
	printf("#######################################\n");
	printf("\n");
}

void print_navigation(){
	printf("#######################################\n");
	printf("a - append item\n");
	printf("l - append thee items\n");
	printf("p - prepend item\n");
	printf("r - remove item at index\n");
	printf("i - insert item at index\n");
	printf("t - item at index\n");
	printf("s - size of list\n");
	printf("m - move item from index to index\n");
	printf("#######################################\n");
	printf("q - quit\n");
}

void start_test(KLIST *list){
Start:;
	print_header();
	print_list(list);
	print_navigation();
	char c;
	while (1){		
		c = readChar();
		switch (c) {
			case 'q':
				goto Exit;
				break;
			case 'a':
				{
					printf("Enter string to append\n");
					getchar();
					char *str = readString();
					k_list_append(list, str);
					goto Start;
					break;
				}	
			case 'l':
				{
					printf("Enter string to append\n");
					getchar();
					char *str0 = readString();
					printf("Enter string to append\n");
					char *str1 = readString();					
					printf("Enter string to append\n");
					char *str2 = readString();					
					k_list_append_items(list, str0, str1, str2, NULL);
					goto Start;
					break;
				}				
			case 'p':	
				{
					printf("Enter string to prepend\n");
					getchar();
					char *str = readString();
					k_list_prepend(list, str);
					goto Start;
					break;				
				}
			case 't':	
				{
					printf("Enter item index\n");
					getchar();
					char *str = readString();
					int i = atoi(str);
					printf("Index: %d is in item '%s'\n", i, (char *)k_list_item_at(list, i));
					getchar();
					goto Start;
					break;				
				}
			case 's':	
				{
					printf("Size of list: %d\n", k_list_size(list));
					getchar();
					getchar();
					goto Start;
					break;				
				}				
			case 'i':
				{
					printf("Enter string as item\n");
					getchar();
					char *str = readString();
					printf("Enter index\n");
					char *stri = readString();
					int i = atoi(stri);
					printf("Insert item: '%s' at index: %d\n", str, i);
					int err = k_list_insert_item_at_index(list, str, i);
					printf("%s\n", k_list_err_str(err));
					getchar();
					goto Start;
					break;
				}				
			case 'r':
				{
					printf("Enter index\n");
					getchar();
					char *stri = readString();
					int i = atoi(stri);
					printf("Remove item at index: %d\n", i);
					int err = k_list_remove_item_at_index(list, i);
					printf("%s\n", k_list_err_str(err));
					getchar();
					goto Start;
					break;
				}				
			case 'm':
				{
					printf("Enter from index\n");
					getchar();
					char *strfrom = readString();
					int from = atoi(strfrom);
					printf("Enter to index\n");
					char *strto = readString();
					int to = atoi(strto);					
					printf("Move item from index: %d to index: %d\n", from, to);
					int err = k_list_move_item(list, from, to);
					printf("%s\n", k_list_err_str(err));
					getchar();
					goto Start;
					break;
				}				
			default:
				goto Start;
				break;
		}	
	}
	Exit:;
	printf("Exit...\n");
	k_list_free(list);
}

int main(int argc, char *argv[])
{
	printf("Starting klist test...\n");
	KLIST *list = k_list_new();
	start_test(list);
	return 0;
}

