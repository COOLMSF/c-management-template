#ifndef __TREE_H__
#define __TREE_H__

#include <stdbool.h>
typedef void *DataType;

typedef struct TreeNode
{
	DataType				data;
	struct TreeNode*		lchild;
	struct TreeNode*		rchild;
}TreeNode, *TreeNodePtr;


/*初始化操作, 建立一个二叉数 */
void InitTree(TreeNodePtr* T);

/*若二叉树存在, 则销毁它 */
void DestroyTree(TreeNodePtr* T);

/*若二叉树存在, 插入值e */
void InsertTree(TreeNodePtr* T, DataType e);

/*若二叉树存在结点e, 则删除之 */
void DeleteNode(TreeNodePtr* T, DataType e);

/*判断二叉树是否为空, 空返回true, 否则false */
bool TreeEmpty(TreeNodePtr T);

/*若二叉树存在, 输出其结点个数 */
int NodeCount(TreeNodePtr T);

/* 初始条件: 二叉树T存在。操作结果: 返回T的深度 */
DataType TreeDepth(TreeNodePtr T);

/* 初始条件: 二叉树T存在，p指向T中某个结点 */
/* 操作结果: 返回p所指结点的值 */
DataType Value(TreeNodePtr T);

/* 给p所指结点赋值为value */
void Assign(TreeNodePtr T, DataType value);

/* 初始条件: 二叉树T存在 */
/* 操作结果: 前序递归遍历T */
void PreOrderTraverse(TreeNodePtr T, int *p);

/* 初始条件: 二叉树T存在 */
/* 操作结果: 中序递归遍历T */
void InOrderTraverse(TreeNodePtr T);

/* 初始条件: 二叉树T存在 */
/* 操作结果: 后序递归遍历T */
void PostOrderTraverse(TreeNodePtr T);

/* 初始条件: 二叉树T存在 */
/* 操作结果: 前序循环遍历T */
void PreOrderLoop(TreeNodePtr T);

/* 初始条件: 二叉树T存在 */
/* 操作结果: 中序循环遍历T */
void InOrderLoop(TreeNodePtr T);

/* 初始条件: 二叉树T存在 */
/* 操作结果: 后序循环遍历T */
void PostOrderLoop(TreeNodePtr T);
void PostOrderLoop2(TreeNodePtr T);
void PreOrderTraverseBookInfo(TreeNodePtr T, int *p);

/* 初始条件: 二叉树T存在 */
/* 操作结果: 层序遍历 */
void LevelTraversal(TreeNodePtr T);

TreeNodePtr FindElem(TreeNodePtr T, DataType e);

#endif
