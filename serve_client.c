#include <stdio.h>
#include <stddef.h>

#include <string.h>


pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;


void *downtime()
{
	for(int i = 0; i < 3; i++)
	{
		sleep(2);
		
		pthread_rwlock_wrlock(&lock);
		
		Node *oldRoot = root;
		
		root = balanceTree(root);
		
		freeSubtree(oldRoot);
		
		pthread_rwlock_unlock(&lock);
	} 
	return NULL;
}

void *ServeClient(char *client)
{
	FILE *fp;
	
	fp = fopen(client, "r");
	
	char *split;
	char clientID[50] = {};
	
	if((split = strstr(client, "_")) != NULL)
	{
		for(int h = 0; h < split - client; h++)
		{
			clientID[h] = *(client+h);
		}
	}
	
	
	if(fp!=NULL)
	{
		char c[100];
		int numNodes;
		float aver;
		
		while(fgets(c, 100, fp) != NULL)
		{			
			if(strstr(c, "addNode") != NULL)
			{
				pthread_rwlock_wrlock(&lock);
				
				char strNumAdd[50] = {};
				
				for(int i = 0; i < sizeof(c) - 7; i++)
				{
					strNumAdd[i] = *(c+7+i);
				}
				
				int numToAdd = atoi(strNumAdd);
				
				printf("%sinsertNode %d\n", clientID, numToAdd);
				root = addNode(root, numToAdd);
				
				pthread_rwlock_unlock(&lock);
			}
			
			else if(strstr(c, "removeNode") != NULL)
			{
				pthread_rwlock_wrlock(&lock);
			
				char strNumRem[50] = {};
				
				for(int j = 0; j < sizeof(c) - 10; j++)
				{
					strNumRem[j] = *(c+10+j);
				}
				
				int numToRemove = atoi(strNumRem);
				
				printf("%sdeleteNode %d\n", clientID, numToRemove);
				
				root = removeNode(root, numToRemove);
				
				pthread_rwlock_unlock(&lock);
			}
			
			else if(strstr(c, "countNodes") != NULL)
			{
				pthread_rwlock_rdlock(&lock);
				
				numNodes = countNodes(root);
				printf("%scountNodes = %d\n", clientID, numNodes);
				
				pthread_rwlock_unlock(&lock);
			}
			
			else if(strstr(c, "avgSubtree") != NULL)
			{
				pthread_rwlock_rdlock(&lock);
				
				aver = avgSubtree(root);
				printf("%savgSubtree = %f\n", clientID, aver);
				
				pthread_rwlock_unlock(&lock);
			}
				
		}
	}
	
	fclose(fp);
	
	return NULL;
}

