// Aliyah Flores
// aff13

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct Node {
	struct Node* next;
	int value;
} Node;

//struct Node* head = NULL;

Node* create_node(int value) {
	//use malloc to allocate enough memory to hold one Node
	//store pointer to allocated memory in a Node* variable
	//set value member to one set in function
	//set next member to Null
	//return Node*
	Node* head = (Node*) malloc(sizeof(Node));
	head->value = value;
	head->next = NULL;

	return head;
}

void print_list(Node* n) {
	//print out list
	if(n == NULL) {
		return;
	}
	print_list(n->next);
	printf("%d\n", n->value);
}

void free_list(Node* n) {
	//iterate through the list and call free on each of the nodes in the list
	//do nothing if null
	while(n != NULL) {
		Node* tmp = n;
		tmp = n->next;
		free(n);
		n = NULL;
		n = tmp;
	}
} 

int main() {
	Node* head = NULL;
	Node* nodelist;
	char node[100];
	int number = 0;
	
	while (number != -1) {
	
		printf("Please type an integer: ");
		fgets(node, sizeof(node), stdin);
		sscanf(node, "%d", &number);
		
		if (number == -1) {
			break;
		}
		
		Node* new = create_node(number);
				
		if (head == NULL) {
			head = new;
			nodelist = new;
		} else {
			nodelist->next = new;
			nodelist = nodelist->next;
		} 
	}
		print_list(head);
		free_list(head);

}