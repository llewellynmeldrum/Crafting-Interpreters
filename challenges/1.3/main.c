#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
// define doubly linked list, data= heap alloc strings

#define println() printf("\n");
#define STRCMP_MATCH 0

typedef struct node{
	struct node* next;
	struct node* prev;
	char* data; 
}Node;


void printList(Node* head);
Node* createNode(Node* prev, Node* next, const char* input);
void deleteNode(Node* target);
Node* findNode(Node* head, const char* target_data);
void printNode(Node* node);
Node* makeList(int length); // returns head

int main(){
	Node* head = makeList(10);
	printList(head);

	printf("deleting node7...\n");
	deleteNode(findNode(head, "node7"));

	printList(head);


	printf("attempting delete with bogus name...");
	deleteNode(findNode(head, "bogus"));
	printList(head);

	printf("deleting node1...\n");
	deleteNode(findNode(head, "node1"));
	printList(head);

	printf("deleting head...\n");
	deleteNode(findNode(head, "head"));
	printList(head);
	// doesnt work, because head and tail deletion are edge cases. 

}

void printList(Node* head){
	printf("Printing list:\n");
	Node* curr = head;
	while (curr){
		if (curr==head) {
			printNode(curr);
		} else {
			printf("<->");
			printNode(curr);
		}

		curr = curr->next;
	}
	println();
}

Node* createNode(Node* prev, Node* next, const char* input){
	Node* node = malloc(sizeof(Node));
	node->prev = prev;
	node->next = next;
	node->data = malloc(sizeof(char) * (strlen(input) + 1));
	strcat(node->data, input);
	return node;
}

void deleteNode(Node* target){
	if (!target){
		printf("Error! deleteNode called on NULL target.\n");
		return;
	}
	// [prev]<->[target]<->[next]
	// [prev]<->[next]
	Node* n1 = target->prev;
	Node* n2 = target->next;
	n1->next = n2;
	n2->prev = n1;
	free(target->data);
	free(target);
}

Node* findNode(Node* head, const char* target_data){
	Node* curr = head;
	while (curr){
		if( strcmp(target_data,curr->data) == STRCMP_MATCH ){
			return curr;
		}
		curr = curr->next;
	}
	printf("Unable to find node with data=\"%s\"\n",target_data);
	return NULL;
}

void printNode(Node* node){
	printf("[\"%s\"]", node->data);
}
Node* makeList(int length){
	Node* head = createNode(NULL, NULL, "head");	
	Node* curr = head;
	Node* prev = NULL;
	for (int i = 0; i<length-1; i++){
		char name[8];
		sprintf(name, "node%d",i);
		// add to head: 
		// make a node, prev=old, next = NULL
		// new->next = newNode.
		// so head->prev=null, head->next=new, new->prev=head
		curr->next = createNode(curr, NULL, name);

		prev = curr;
		curr = curr->next;
	} 

	curr->next = createNode(curr, NULL, "tail");
	prev = curr;
	return head;
}
