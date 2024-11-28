#include<stdio.h>
#include<stdlib.h>
typedef struct Node {
	char data;
	int index;
	struct Node* firstChild;
	struct Node* next;
}Node;
void buildTree(Node* root, char curNodeData)
{
	char ch;
	static int curIndex = 0;
	root->next = NULL;
	root->firstChild = NULL;
	root->data = curNodeData;
	root->index = curIndex;
	ch = getchar();
	int hasChild = 0;
	Node* p;
	Node* q;
	while (ch != '#')
	{
		Node* p = (Node*)malloc(sizeof(Node));
		p->next = NULL;
		p->firstChild = NULL;
		if (!hasChild)
		{
			root->firstChild = p;
			hasChild = 1;
		}
		else
		{
			q->next = p;
		}
		q = p;
		curIndex++;
		buildTree(p, ch);
		ch = getchar();
	}
}
void show1(Node* root)
{
	static int list = 0;
	printf("%d ", list);
	list++;
	printf("%c :", root->data);
	Node* cur;
	cur = root->firstChild;
	while (cur != NULL)
	{
		printf("->%d", cur->index);
		cur = cur->next;
	}
	printf("->NULL\n");
	cur = root->firstChild;
	if (cur != NULL)
	{
		show1(cur);
	}
	cur = root->next;
	if (cur != NULL)
	{
		show1(cur);
	}
}
void show2(Node *root)
{
	printf("%c", root->data);
	Node* cur;
	cur = root->firstChild;
	if (cur != NULL)
	{
		show2(cur);
	}
	cur = root->next;
	if (cur != NULL)
	{
		show2(cur);
	}
}
int main()
{
	char ch;
	ch = getchar();
	Node* root = (Node*)malloc(sizeof(Node));
	buildTree(root, ch);
	show1(root);
	printf("PreOrder:");
	show2(root);
    printf("\n");
    system("pause");
}