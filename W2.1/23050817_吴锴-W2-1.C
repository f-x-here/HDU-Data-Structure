#include <stdio.h>
#include <stdlib.h>
typedef struct LinkNode
{
    int value;
    struct LinkNode *next;
} Node;
Node *CreateNode()
{
    Node *head;
    head = (Node *)malloc(sizeof(Node));
    head->next = NULL;
    head->value = 0;
    return head;
}
void DeleteNode(Node *listHead, int key)
{
    Node *curNode, *preNode;
    preNode = listHead;
    if (preNode->next != NULL)
    {
        curNode = preNode->next;
    }
    else
    {
        return;
    }
    while (curNode != NULL)
    {
        if (curNode->value == key)
        {
            preNode->next = curNode->next;
            free(curNode);
            curNode = preNode->next;
            continue;
        }
        preNode = curNode;
        curNode = curNode->next;
    }
}
void AddNode(Node *curNode, int value2Add)
{
    Node *newNode;
    newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value2Add;
    newNode->next = curNode->next;
    curNode->next = newNode;
}
Node *CopyList(Node *list)
{
    Node *head;
    head = CreateNode();
    Node *node2Copy, *curNode;
    node2Copy = list->next;
    curNode = head;
    while (node2Copy != NULL)
    {
        AddNode(curNode, node2Copy->value);
        curNode = curNode->next;
        node2Copy = node2Copy->next;
    }
    return head;
}
Node *Turn2Unique(Node *list)
{
    Node *head = CopyList(list);
    Node *curNode;
    if (head->next != NULL)
    {
        curNode = head->next;
    }
    else
    {
        printf("对空链表操作\n");
        return NULL;
    }
    while (curNode != NULL)
    {
        DeleteNode(curNode, curNode->value);
        curNode = curNode->next;
    }
    return head;
}
Node *PrintList(Node *list)
{
    list = list->next;
    while (list != NULL)
    {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}
int main()
{

    Node *head = CreateNode();
    Node *curNode = head;
    Node *errorList = CreateNode();
    Node *curError = errorList;
    int errorTimes = 0;
    printf("请输入要输入的链表的元素个数\n");
    int num2Add;
    while (scanf("%d", &num2Add) != 1 || num2Add < 0)
    {
        char c;
        while ((c = getchar()) != '\n')
            ;
        printf("非法输入!请输入非负整数\n");
    }
    if (num2Add == 0)
    {
        printf("空链表！\n");
        return 0;
    }
    else
    {
        printf("请依次输入链表元素(整数),以空格/回车划分\n");
        int number = 0;
        for (int i = 1; i <= num2Add; i++)
        {
            if (scanf("%d", &number) != 1)
            {
                errorTimes += 1;
                char c;
                while ((c = getchar()) != '\n' && (c = getchar()) != ' ')
                    ;
                AddNode(curError, i);
                curError = curError->next;
            }
            AddNode(curNode, number);
            curNode = curNode->next;
        }
        if (errorList->next != NULL)
        {
            printf("读入%d个数据,其中%d个读取错误,请根据提示重新输入,每次输入以换行结尾\n", num2Add, errorTimes);
        }
        curError = errorList; 
        int curNodeId = 0;
        curNode = head;
        while (curError->next != NULL)
        {
            curError = curError->next;
            for (; curNodeId < curError->value; curNodeId++)
            {
                curNode = curNode->next;
            }
            printf("请输入第%d次的正确整数数据\n", curError->value);
            while (scanf("%d", &number) != 1)
            {
                char c;
                while ((c = getchar()) != '\n')
                    ;
                printf("非法输入,请重新输入\n");
            }
            curNode->value = number;
        }
    }
    Node *uniqueList = Turn2Unique(head);
    printf("原链表:\n");
    PrintList(head);
    printf("去除重复元素后的链表:\n");
    PrintList(uniqueList);
    system("pause");
}