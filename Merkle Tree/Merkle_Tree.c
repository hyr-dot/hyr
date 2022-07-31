#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

// Merkle Tree 节点定义
typedef struct Node {
	struct Node* left;		// 左子节点 
	struct Node* right;		// 右子节点 
	struct Node* parent;		// 父节点 
	uint depth;							//深度，底层为0 
	uint data;							// 当前节点的值
}MerkleTree;

#define New_Node(mt, tree_depth) {	\
	mt = (MerkleTree *)malloc(sizeof(MerkleTree)); \
	mt->left = NULL; \
	mt->right = NULL; \
	mt->parent = NULL; \
	mt->depth = (uint)tree_depth; \
	mt->data = 0;	\
	}

// 打印Merkle Tree
void Print_Merkle_Tree(MerkleTree* mt, int high)
{
	MerkleTree* p = mt;
	int i;

	if (p == NULL)
		return;

	if (p->left == NULL && p->right == NULL) {
		for (i = 0; i < high - p->depth; i++)
			printf("           ");

		//printf("------>%s\n", p->str);
	}
	else {
		Print_Merkle_Tree(mt->left, high);
		printf("\n");
		for (i = 0; i < high - p->depth; i++)
			printf("           ");

		printf("------>%-6d\n", p->data);

		Print_Merkle_Tree(mt->right, high);
	}
}

uint hash_string(char* key) {
	uint cal = 11, hash = 0;
	while (*key != '\0' && *key != 0) {
		hash = hash + *key * 11;
		key++;
	}
	return hash & 0x7FFFFFFF;
}

uint hash_two_node(uint num1, uint num2) {
	uint cal = 9, hash = 0;
	hash = num1 + num2;
	hash *= cal;
	return hash & 0x7FFFFFFF;
}

// 寻找最后一个叶子节点
MerkleTree* Find_Last_Node(MerkleTree* mt) {
	MerkleTree* p = mt, * tmp;

	if (p->left == NULL && p->right == NULL)
		return p;
	else if (p->right == NULL && p->left != NULL)
		return Find_Last_Node(p->left);
	else if (p->right != NULL)
		return Find_Last_Node(p->right);
}

// 根据最后一个节点寻找插入的位置 
MerkleTree* Merkle_Tree_Insertloc(MerkleTree* mt) {
	MerkleTree* p = mt->parent;
	while (p->left != NULL && p->right != NULL && p->parent != NULL)  p = p->parent;
	if (p->parent == NULL && p->left != NULL && p->right != NULL)
		return NULL;
	else
		return p;
}

// Merkle tree 初始化 
MerkleTree* Merkle_Tree_Init(MerkleTree* mt, int* arr, int nums)
{
	MerkleTree* node, * tmp, * p=NULL;
	int i;
	if (nums == 0) {

		printf("Merkle Tree construction complete!\n");
		return mt;
	}
	else {

		// 创建一个叶节点 
		New_Node(node, 0);
		node->data = *arr;
		printf("Create leaf node [%d] tree_depth=%d, depth=%d, data=%-6d, nums=%d, \n", __LINE__, mt == NULL ? 0 : mt->depth, node->depth, node->data, nums);

		// 如果 mt 为空，说明当前没有树，需要新建一个头结点 
		if (mt == NULL) {

			New_Node(mt, 1);
			mt->left = node;
			node->parent = mt;
			printf("Create head node [%d] tree_depth=%d, depth=%d, data=%-6d \n", __LINE__, mt->depth, mt->depth, mt->data);
			mt->data = hash_two_node(mt->left->data, mt->right == NULL ? 0 : p->right->data);
			mt = Merkle_Tree_Init(mt, arr + 1, nums - 1);
		}
		//如果 mt 不为空，说明当前已经有树了 
		else
		{
			// 遍历当前树，找到一个空的叶子节点，满二叉树时返回NULL 
			p = Merkle_Tree_Insertloc(Find_Last_Node(mt));

			// 如果返回值不为 NULL, 说明已经找到需要插入的节点 
			if (p != NULL) {
				if (p->left->left == NULL && p->right == NULL)
				{
					p->right = node;
					node->parent = p;
					// 更新哈希值 
					p->data = hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data);
				}
				// 如果返回的切点是中间节点 
				else
				{      
					i = p->depth - 1;
					New_Node(tmp, i);
					p->right = tmp;
					tmp->parent = p;
					printf("新中间节点 [%d] tree_depth=%d, depth=%d, data=%-6d \n", __LINE__, mt->depth, tmp->depth, tmp->data);
 
					p = p->right;
					i = p->depth - 1;		// 更新depth - 1 

					while (i > 0) {
						// 创建中间节点 
						New_Node(tmp, i);
						p->left = tmp;
						tmp->parent = p;
						printf("新中间节点 [%d] tree_depth=%d, depth=%d, data=%-6d \n", __LINE__, mt->depth, tmp->depth, tmp->data);

						p = p->left;
						i--;
					}

					p->left = node;
					node->parent = p;

					// 自底向上更新哈希值
					while (p != mt) {
						p->data = hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data);
						p = p->parent;
					}
					//  更新父节点哈希值  
					p->data = hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data);

				}
				// 节点插入成功，递归添加下一个值  
				mt = Merkle_Tree_Init(mt, arr + 1, nums - 1);
			}
			// 如果没有空的节点，说明当前是满二叉树，需要新增头节点
			else
			{
				tmp = mt;				// 保存当前头结点

				//创建一个新的头节点
				New_Node(mt, tmp->depth + 1);
				mt->left = tmp; 		// 头节点赋值 
				tmp->parent = mt;
				printf("新建头节点 [%d] tree_depth=%d, depth=%d, data=%-6d \n", __LINE__, mt->depth, mt->depth, mt->data);

				// 创建头节点下的第一个右子节点 
				New_Node(tmp, mt->depth - 1);
				mt->right = tmp;
				tmp->parent = mt;
				printf("新中间节点 [%d] tree_depth=%d, depth=%d, data=%-6d \n", __LINE__, mt->depth, tmp->depth, tmp->data);

				p = mt->right;
				i = p->depth - 1;

				// 根据树的深度创建同样深度的左树 
				while (i > 0) {
					// 创建结点
					New_Node(tmp, i);
					p->left = tmp;
					tmp->parent = p;
					printf("新中间节点 [%d] tree_depth=%d, depth=%d, data=%-6d \n", __LINE__, mt->depth, tmp->depth, tmp->data);

					p = p->left;
					i--;
				}
				// 叶子节点赋值 
				p->left = node;
				node->parent = p;

				// 自底向上更新哈希值
				while (p != mt) {
					p->data = hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data);
					p = p->parent;
				}
				// 更新父节点哈希值  
				p->data = hash_two_node(p->left->data, p->right == NULL ? 0 : p->right->data);

				// 递归调用 
				mt = Merkle_Tree_Init(mt, arr + 1, nums - 1);
			}
		}
	}
}

// 清空 Merkle tree, 回收所有分配的内存 
void Merkle_Tree_Recycle(MerkleTree* mt)
{
	// 如果是叶子节点，直接回收 
	if (mt->depth == 0) {
		printf("leaf node recycle，depth=%d, data=%d \n", mt->depth, mt->data);
		free(mt);
	}
	else {
		if (mt->left != NULL) {
			Merkle_Tree_Recycle(mt->left);
		}
		if (mt->right != NULL) {
			Merkle_Tree_Recycle(mt->right);
		}
		// 释放当前节点
		printf("Intermediate node recycle，depth=%d, data=%d \n", mt->depth, mt->data);
		free(mt);
	}
}

int main(void)
{
	int array[] = { 1,2,3,4,5,6,7,8,9,10,11, 22, 33 ,44 ,55 ,66, 77, 88, 99, 1111,2222,11111,33333};
	MerkleTree* mt = NULL;

	// 根据数组动态创建 Merkle Tree 
	mt = Merkle_Tree_Init(mt, array, sizeof(array) / sizeof(int));

	// 打印当前merkle Tree
	if (mt != NULL) {
		printf("\n Printing Merkle Tree:\n");
		Print_Merkle_Tree(mt, mt->depth);
		printf("\n\n");
	}

	// 回收内存
	Merkle_Tree_Recycle(mt);

	return 0;
}

