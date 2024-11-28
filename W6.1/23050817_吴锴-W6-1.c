#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#undef max

int max(int a, int b)
{
	return a > b ? a : b;
}

int width[20];
typedef struct node
{
	char data;
	struct node* lNode;
	struct node* rNode;
} Node;
int buildTree(const char* str, int Index, Node* rt, int maxNode, int depth)
{
	int result = 0;
	if (Index > maxNode)
	{
		rt = NULL;
		return 0;
	}
	rt->data = str[Index - 1];
	if (rt->data == ' ')
	{
		return 0;
	}
	width[depth]++;
	result++;
	//    printf("%c ", rt->data);
	rt->lNode = (Node*)malloc(sizeof(Node));
	rt->rNode = (Node*)malloc(sizeof(Node));
	depth++;
	result += buildTree(str, Index * 2, rt->lNode, maxNode, depth);
	result += buildTree(str, Index * 2 + 1, rt->rNode, maxNode, depth);
#ifdef DEBUG
	printf("%d %d\n", Index, result);
#endif
	return result;
}
void format(int index)
{
	if (index < 2)return;
	format(index>>1);
	if (index % 2 == 0)
	{
		printf("| ");
	}
	else
	{
		printf("  ");
	}
}
void showTree(Node* rt, int depth, int index, int maxNode)
{
	if (rt->data == ' ')
	{
		return;
	}
	if (index > 3)
	{
		format(index>>1);
	}
	if (index != 1)
	{
		printf("|-");
	}
	printf("%c\n", rt->data);
	depth++;
	if (index * 2 <= maxNode)
	{
		showTree(rt->lNode, depth, index * 2, maxNode);
	}
	if (index * 2 + 1 <= maxNode)
	{
		showTree(rt->rNode, depth, index * 2 + 1, maxNode);
	}
}
bool check(int length)
{
	if (length == 1)
		return true;
	return length % 2 == 0 ? check(length / 2) : false;
}
int main()
{
	int nodeNum = 0;
	char* str = (char*)malloc(1024 * sizeof(char));
	printf("请按照层次编号以完全树的形式顺序输入树的各节点内容,空节点输入空格，以换行结束。\n");
	char c;
	int flag;
Input:
	str[0] = '\0';
	flag = 0;
	while ((c = getchar()) != '\n')
	{
		str[flag] = c;
		flag++;
		if (c != ' ')
		{
			nodeNum++;
		}
	}
	if (flag != 0 && check(flag + 1))
	{
		printf("开始建树\n");
		memset(width, 0, sizeof(width));
		Node* rt = (Node*)malloc(sizeof(Node));
		int buildNum = buildTree(str, 1, rt, flag, 0);
		if (buildNum != nodeNum)
		{
#ifdef DEBUG
			printf("%d %d\n", buildNum, nodeNum);
#endif
			printf("不允许空节点存在子节点,请重新输入\n");
			goto Input;
		}
		showTree(rt, 0, 1, flag);
		int maxWidth = 0;
		for (int i = 0; i < log2(flag); i++)
		{
			maxWidth = max(maxWidth, width[i]);
		}
		printf("二叉树宽度为：%d", maxWidth);
	}
	else
	{
		printf("输入数据非完全树，请重新输入\n");
		goto Input;
	}
}