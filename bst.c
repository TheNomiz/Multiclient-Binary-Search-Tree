#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"

/*

Place for the BST functions from Exercise 1.

*/

struct _Node
{
	int data;
	struct _Node* left;
	struct _Node* right;
};

Node* addNode(Node* root, int value)
{
	if (root==NULL)
	{
		Node* newNode = malloc(sizeof(struct Node));
		newNode->data = value;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}
	else
	{
		if(value==root->data)
			return root;
		else if(value>root->data)
		{
			if(root->left==NULL)
				root->left = addNode(root->left, value);
			else
				addNode(root->left, value);
		}
		else
		{
			if(root->right==NULL)
				root->right = addNode(root->right, value);
			else
				addNode(root->right, value);
		}
		
		return root;
	}
	
}

Node* removeNode(Node* root, int value)
{
	if(root==NULL)
		return NULL;
	if(value>root->data)
	{
		root->left = removeNode(root->left, value);
	}
	else if(value<root->data)
	{
		root->right = removeNode(root->right, value);
	}
	else
	{
		if(root->left==NULL && root->right==NULL)
		{
			free(root);
			return NULL;
		}
		else if(root->left==NULL || root->right==NULL)
		{
			if(root->left==NULL)
			{
				
				Node* right = root->right;
				free(root);
				return right;
				
			}
			else if(root->right==NULL)
			{
				
				Node* left = root->left;
				free(root);
				return left;
			
			}
		}
		else
		{
			Node* replacement = root->left;
			
			while(replacement->right!=NULL)
			{
				replacement = replacement->right;
			}
			
			root->data = replacement->data;
			root->left = removeNode(root->left, replacement->data);
		}
	}
	return root;
}

void displaySubtree(Node* N)
{
	if(N->right!=NULL)
		displaySubtree(N->right);
	
	if(N!=NULL)	
		printf("%d\n", N->data);
	
	if(N->left!=NULL)
		displaySubtree(N->left);
}

int numberLeaves(Node* N)
{
	if(N==NULL)
		return 0;
		
	if(N->left==NULL && N->right==NULL)
		return 1;
	else
		return numberLeaves(N->left) + numberLeaves(N->right);
}

Node* freeSubtree(Node *N)
{
	if (N!=NULL)
	{
		freeSubtree(N->left);
		freeSubtree(N->right);
		free(N);
	}
	return NULL;
}

Node* removeSubtree(Node* root, int value)
{
	if(root==NULL)
		return NULL;
		
	if(value>root->data)
		root->left = removeSubtree(root->left, value);
	else if(value<root->data)
		root->right = removeSubtree(root->right, value);
	else
	{
		Node* left = root->left;
		Node* right = root->right;
		
		if(left!=NULL)
			left = removeSubtree(left, left->data);
			
		if(right!=NULL)
			right = removeSubtree(right, right->data);
			
		free(root);
		return NULL;
	}
	return root;
}

int nodeDepth (Node* R, Node* N)
{
	if(R==NULL || N==NULL)
		return -1;
	
	int count = 0;
	Node* current = R;
	
	while(current!=NULL)
	{
		if(N->data > current->data)
		{
			current = current->left;
			count = count + 1;
		}
		else if(N->data < current->data)
		{
			current = current->right;
			count = count + 1;
		}
		else
			return count;
	}
	return -1;
}


///////////////NEW FUNCTIONS///////////////////////////////


//Helper Function
int countNodes(Node* root)
{	
	if(root!=NULL)
		return 1 + countNodes(root->left) + countNodes(root->right);
	else
		return 0;
}

int sum(Node* N)
{
	if(N!=NULL)
		return N->data + sum(N->left) + sum(N->right);
	else
		return 0;
}

float avgSubtree(Node *N)
{
	// TODO: Implement this function
	
	if(N!=NULL)
	{
		float nodeSum = sum(N);
		float amount = countNodes(N);
		float avg = nodeSum/amount;
		return avg;
	}
	else
		return 0;	
}


//Helper function for balanceTree
int buildArray(Node* root, int* array, int arraySize, int currentIndex)
{
	int index = currentIndex;
	if(currentIndex<arraySize)
	{
		if(root!=NULL)
		{
			if(root->left!=NULL)
				index = buildArray(root->left, array, arraySize, index);
			
			*(array+index) = root->data;
			index = index + 1;
			
			if(root->right!=NULL)
				index = buildArray(root->right, array, arraySize, index);
				
			return index;
		}
	}
}

Node* rebuildTree(int *array, Node* root, int arraySize)
{
	Node* nRoot = root;
	
	int currentIndex = arraySize/2;
	
	if(nRoot==NULL)
		nRoot = addNode(NULL, *(array+currentIndex));
	else
		addNode(nRoot, *(array+currentIndex));
	
	int leftArray[currentIndex];
	int rightArray[arraySize - currentIndex - 1];
	
	for(int i = 0; i < currentIndex; i++)
	{
		leftArray[i] = *(array+i);
	}
	
	for(int j = 0; j < arraySize - currentIndex - 1; j++)
	{
		rightArray[j] = *(array+currentIndex+1+j);
	}
	
	if(currentIndex > 0)
		rebuildTree(leftArray, nRoot, currentIndex);
	
	if(arraySize - currentIndex - 1 > 0)
		rebuildTree(rightArray, nRoot, arraySize - currentIndex - 1);
	
	return nRoot;
}

// This functions converts an unbalanced BST to a balanced BST
Node* balanceTree(Node* root)
{
	// TODO: Implement this function
	int arraySize = countNodes(root);
	
	if(arraySize > 0)
	{
		int bstArray[arraySize];
		int *a = bstArray;
		
		buildArray(root, a, arraySize, 0);
		
		return rebuildTree(a, NULL, arraySize);
	}
	else
		return NULL;
		
}

