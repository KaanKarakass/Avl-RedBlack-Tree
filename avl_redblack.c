#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define uint64_t unsigned long 
#define uint32_t unsigned int

uint64_t cycles_now(){
#if defined(__GNUC__)
	uint32_t lo, hi;
  	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	return ((uint64_t)lo | ((uint64_t)hi <<32));
#else
	return 0;
#endif
}
//  *******************************************************************************AVL TREE 
typedef struct AVLNODE_s *AVLNODE;
typedef struct AVLNODE_s{
	AVLNODE right;
	AVLNODE left;
	void *data;
	unsigned long key;
	int height;
}AVLNODE_t[1];

typedef struct AVLTREE_s{
	AVLNODE root;
}AVLTREE_t[1],*AVLTREE;

AVLTREE avl_tree_init()
{
	AVLTREE t2;
	t2=(AVLTREE)malloc(sizeof(AVLTREE_t));
	t2->root=NULL;
	return t2;
}
void avl_tree_kill(AVLTREE t1)
{
	free(t1);
}
AVLNODE avl_node_init(unsigned long key,void* data)
{
	AVLNODE node;
	node=(AVLNODE)malloc(sizeof(AVLNODE_t));
	node->data=data;
	node->key=key;
	node->left=NULL;
	node->right=NULL;
	node->height=1;
}
void avl_node_kill(AVLNODE node)
{
	free(node);
}
int height(AVLNODE node)
{
	if(node==NULL)
	{
		return 0;
	}
	return node->height;
}
int is_balanced(AVLNODE node)
{
	if(node==NULL)
	{
		return 0;
	}
	return height(node->left) - height(node->right);
}
int max(int num1,int num2)
{
	if(num1>num2)
	{
		return num1;
	}
	else
	{
		return num2;
	}
}
AVLNODE avl_right_rotate(AVLNODE node)
{
	AVLNODE left=node->left;
	AVLNODE T2 =left->right;
	
	left->right=node;
	node->left=T2;
	
	node->height=max(height(node->left),height(node->right))+1;
	left->height=max(height(left->left),height(left->right))+1;
	
	return left;
}
AVLNODE avl_left_rotate(AVLNODE node)
{
	AVLNODE right =node->right;
	AVLNODE T2=right->left;
	
	right->left=node;
	node->right=T2;
	
	node->height=max(height(node->left),height(node->right))+1;
	right->height=max(height(right->left),height(right->right))+1;
	
	return right;
}
AVLNODE avl_insert(AVLNODE node,unsigned long key,void *data)
{
	if(node!=NULL)
	{
		if(key<node->key)
		{
			node->left=avl_insert(node->left,key,data);
		}
		else if(key > node->key)
		{
			node->right=avl_insert(node->right,key,data);
		}
	}
	else
	{
		node=avl_node_init(key,data);
	}
	node->height= 1 + max(height(node->left),height(node->right));
	
	int balanced=is_balanced(node);
	
	if(balanced>1 &&key < node->left->key)//left left
	{
		return avl_right_rotate(node);
	}
	if(balanced <-1 && key> node->right->key)//right right
	{
		return avl_left_rotate(node);
	}
	if(balanced>1 && key >node->left->key) //left right
	{
		node->left=avl_left_rotate(node->left);
		return avl_right_rotate(node);
	}
	if(balanced <-1  && key <node->right->key) //right  left
	{
		node->right=avl_right_rotate(node->right);
		return avl_left_rotate(node);
	}
	return node;
}
void avl_insert1(AVLTREE t1,unsigned long key,void* data)
{
	if(t1->root==NULL)
	{
		t1->root=avl_node_init(key,data);
	}
	else
	{
		t1->root=avl_insert(t1->root,key,data);
	}
}
AVLNODE avl_search(AVLNODE node,unsigned long key)
{
	if(node==NULL || node->key==key)
	{
		return node;
	}
	if(node->key < key)
	{
		return avl_search(node->right,key);
	}
	else if(node->key > key)
	{
		return avl_search(node->left,key);
	}
}
// ******************************************************************************RED BLACK TREE
typedef struct RBNODE_s *RBNODE;
typedef struct RBNODE_s{
	RBNODE right;
	RBNODE left;
	unsigned long key;
	void *data;
	int color;  //0 black 1 red
}RBNODE_t[1];
typedef struct RBTREE_s{
	RBNODE root;
}RBTREE_t[1],*RBTREE;

RBNODE red_black_node_init(unsigned long key,void* data)
{
	RBNODE node;
	node=(RBNODE)malloc(sizeof(RBNODE_t));
	node->data=data;
	node->key=key;
	node->color=1;
	node->left=NULL;
	node->right=NULL;
	return node;
}
RBTREE red_black_tree_init()
{
	RBTREE t1;
	t1=(RBTREE)malloc(sizeof(RBTREE_t));
	t1->root=NULL;
	return t1;
}

RBNODE red_black_right_rotate(RBNODE node)
{
	RBNODE left=node->left;
	RBNODE T2=left->right;
	
	left->right=node;
	node->left=T2;
	return left;
}
RBNODE red_black_left_rotate(RBNODE node)
{
	RBNODE right=node->right;
	RBNODE T2=right->left;
	
	right->left=node;
	node->right=T2;
	return right;
}
RBNODE red_black_recursive_insert(RBNODE node,unsigned long key,void *data,RBTREE t)
{
	RBNODE root1;
	root1=(RBNODE)t;
	RBNODE root=root1->right;
	if(node!=NULL)
	{
		if(key < node->key)
		{
			node->left=red_black_recursive_insert(node->left,key,data,t); 
		}
		else if(key > node->key )
		{
			node->right=red_black_recursive_insert(node->right,key,data,t);
		}
		else
		{
		    printf("error red black insert\n");
		}
	}
	else
	{
		node=red_black_node_init(key,data);
	}
	
	int control_parent,control_child; //left 0   right 1
	RBNODE parent=NULL;
	RBNODE child=NULL;
	RBNODE uncle=NULL;
	if(key < node->key &&node->left!=NULL)
	{
		parent=node->left;
		control_parent=0;
		if(node->right!=NULL)
		{
			uncle=node->right;
		}
	}
	else if(key>node->key && node->right!=NULL)
	{
		parent=node->right;
		control_parent=1;
		if(node->left!=NULL)
		{
			uncle=node->left;
		}
	}
	if(parent!=NULL)
	{
		if(key<parent->key && parent->left!=NULL)
		{
			child=parent->left;
			control_child=0;
		}
		else if(key > parent->key && parent->right!=NULL)
		{
			child=parent->right;
			control_child=1;
		}
	}
	
	if(parent!=NULL && child!=NULL)
	{
		
		if(parent!=root &&parent->color==1 &&child->color==1)
		{
			
			if(uncle!=NULL)
			{
				
				if(uncle->color==1)
				{
					parent->color=0;
					uncle->color=0;
					node->color=1;
				}
				else if(uncle->color==0)
				{
					if(control_parent==0 &&control_child==0)//left left
					{
						node->color=1;
						parent->color=0;
						return red_black_right_rotate(node);
					}
					else if(control_parent==0 && control_child==1) //left right
					{
						child->color=0;
						node->color=1;
						node->left=red_black_left_rotate(node->left);
						return red_black_right_rotate(node);
					}
					else if(control_parent==1 && control_child==1) //right  right
					{
						node->color=1;
						parent->color=0;
						return red_black_left_rotate(node);
					}
					else if(control_parent==1 &&control_child==0) //right left
					{
						child->color=0;
						node->color=1;
						node->right=red_black_right_rotate(node->right);
						return red_black_left_rotate(node);
					}
				}
			}
			else
			{
				
				if(control_parent==0 &&control_child==0)//left left
				{
					node->color=1;
					parent->color=0;
					return red_black_right_rotate(node);
				}
				else if(control_parent==0 && control_child==1) //left right
				{
					child->color=0;
					node->color=1;
					node->left=red_black_left_rotate(node->left);
					return red_black_right_rotate(node);
				}
				else if(control_parent==1 && control_child==1) //right  right
				{
					node->color=1;
					parent->color=0;
					return red_black_left_rotate(node);
				}
				else if(control_parent==1 &&control_child==0) //right left
				{
					child->color=0;
					node->color=1;
					node->right=red_black_right_rotate(node->right);
					return red_black_left_rotate(node);
				}
			}
		}
	}
	if(node==root)
	{
		node->color=0;
	}
	return node;
}
void red_black_tree_insert(RBTREE t1,unsigned long key,void *data)
{
	if(t1->root==NULL)
	{
		t1->root=red_black_node_init(key,data);
		t1->root->color=0;
	}
	else
	{
		t1->root=red_black_recursive_insert(t1->root,key,data,t1);
	}
}

int power_function(int n)
{
	int i;
	int sum=1;
	for(i=0;i<n;i++)
	{
		sum=sum*2;
	}
	return sum;
}
unsigned long *random_num(int size)
{
	int i;
	unsigned long *keys1=malloc(sizeof(unsigned long)*size);;
	for(i=0;i<size;i++)
	{
		keys1[i]=(unsigned long)random() * (unsigned long)random();
	}
	
	return keys1;
}

RBNODE red_black_search(RBNODE node,unsigned long key)
{
	if(node==NULL || node->key==key)
	{
		return node;
	}
	if(node->key < key)
	{
		return red_black_search(node->right,key);
	}
	else if(node->key > key)
	{
		return red_black_search(node->left,key);
	}
}
int main()
{
	AVLTREE t;
	t=avl_tree_init();
	
	RBTREE t1;
	t1=red_black_tree_init();
	
	int i;
	int size=power_function(5);
	
	unsigned long *keys=random_num(size);
	
	
	uint64_t st1,fn1,st,fn,st2,fn2,st3,fn3;
	
	st=cycles_now();
	
	for(i=0;i<size;i++)
	{
		unsigned long n=keys[i];
		red_black_tree_insert(t1,n,NULL);
	}	
	
	fn=cycles_now();
	printf("For %d keys, Red Black tree insert code took %lu clock cycles to execute. \n",size,(fn-st));
	
	st1=cycles_now();
	for(i=0;i<size;i++)
	{
		unsigned long n1=keys[i];
		avl_insert1(t,n1,NULL);
	}
	fn1=cycles_now();
	printf("For %d keys ,Avl tree insert code took %lu clock cycles to execute. \n",size,(fn1-st1));
	
	st2=cycles_now();
	for(i=0;i<size;i++)
	{
		unsigned long m=keys[i];
		red_black_search(t1->root,m);
	}
	fn2=cycles_now();
	printf("For %d keys, Red Black tree search code took %lu clock cycles to execute. \n",size,(fn2-st2));
	
	st3=cycles_now();
	for(i=0;i<size;i++)
	{
		unsigned long l=keys[i];
		red_black_search(t1->root,l);
	}
	fn3=cycles_now();
	printf("For %d keys, AVL tree search code took %lu clock cycles to execute. \n",size,(fn3-st3));
	free(keys);
	free(t);
	free(t1);
	return 0;
}

