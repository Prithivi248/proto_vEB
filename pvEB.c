#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

typedef struct pvEB{
	int u;		
	union{
		struct{
			int bits[2];
		}base;
		struct{
			struct pvEB *summary;
			struct pvEB **cluster;
		}composed;
	};	
}pvEB;

pvEB *create(int u){
	pvEB *node = malloc(sizeof(pvEB));
	
	if(u==2){
		node->u=2;
		node->base.bits[0]=0;
		node->base.bits[1]=0;
	}	
	else{
		node->u = u;
		node->composed.summary = create(sqrt(u));
		node->composed.cluster = (pvEB**)malloc(sqrt(u)*sizeof(pvEB*));
		for(int i=0;i<sqrt(u);i++)
			node->composed.cluster[i] = create(sqrt(u));
	}
	
	return node;
}

int high(int x,int u){
	return (int)floor(x / sqrt(u));
}

int low(int x,int u){
	return x % (int)ceil(sqrt(u));
}

int index(int x,int y, int u){
	return x * (int)sqrt(u) + y;
}

void insertElement(pvEB *head,int x){
	if(head->u==2)
		head->base.bits[x]=1;
	else{
		insertElement(head->composed.cluster[high(x,head->u)],low(x,head->u));
		insertElement(head->composed.summary,high(x,head->u));	
	}
	return;
}

int isMember(pvEB *head,int x){
	if(head->u==2)
		return head->base.bits[x];
	else
		return isMember(head->composed.cluster[high(x,head->u)],low(x,head->u));
}

void delElement(pvEB *head,int x){
	if(head->u == 2)
		head->base.bits[x] = 0;
	else{
		delElement(head->composed.cluster[high(x,head->u)],low(x,head->u));
		if(isMember(head->composed.cluster[high(x,head->u)],low(x,head->u))==0){
			delElement(head->composed.summary,high(x,head->u));
		}
	}
}

int minimum(pvEB *head){
	if(head->u == 2){
		if(head->base.bits[0]==1)
			return 0;
		else if(head->base.bits[1]==1)
			return 1;
		return -1;	
	}
	else{
		int minCluster = minimum(head->composed.summary);
		if(minCluster == -1)
			return -1;
		else{
			int offset = minimum(head->composed.cluster[minCluster]);
			return index(minCluster,offset,head->u);
		}
	}
}

int maximum(pvEB *head){
	if(head->u == 2){
		if(head->base.bits[1]==1)
			return 1;
		else if(head->base.bits[0]==1)
			return 0;
		return -1;	
	}
	else{
		int maxCluster = maximum(head->composed.summary);
		if(maxCluster == -1)
			return -1;
		else{
			int offset = maximum(head->composed.cluster[maxCluster]);
			return index(maxCluster,offset,head->u);
		}
	}
}

int predecessor(pvEB *head,int x){
	if(head->u==2){
		if(x==1 && head->base.bits[0]==1)
			return 0;
		return -1;
	}
	else{
		int predCluster = predecessor(head->composed.cluster[high(x,head->u)],low(x,head->u));
		if(predCluster != -1){
			//int offset = maximum(head->composed.cluster[predCluster]);
			return index(high(x,head->u),predCluster,head->u);
		}
		else{
			int predCluster = predecessor(head->composed.summary,high(x,head->u));
			if(predCluster==-1)
				return -1;
			else{
				int offset = maximum(head->composed.cluster[predCluster]);
				return index(predCluster,offset,head->u);
			}
		}
	}
}

int successor(pvEB *head,int x){
	if(head->u==2){
		if(x==0 && head->base.bits[1]==1)
			return 1;
		return -1;
	}
	else{
		int succCluster = successor(head->composed.cluster[high(x,head->u)],low(x,head->u));
		if(succCluster != -1){
			//int offset = minimum(head->composed.cluster[succCluster]);
			return index(high(x,head->u),succCluster,head->u);
		}
		else{
			int succCluster = successor(head->composed.summary,high(x,head->u));
			if(succCluster==-1)
				return -1;
			else{
				int offset = minimum(head->composed.cluster[succCluster]);
				return index(succCluster,offset,head->u);
			}
		}
	}
}

int main(){
	
	pvEB *head = create(16);
	
	printf("Operations on pvEB \n\n"); 
	//MEMBER,INSERT,DELETE,MAXIMUM,MINIMUM,PREDECESSOR,SUCCESSOR
	
	//Insert an element
	insertElement(head,5);
	printf("insert(5)\n");
	insertElement(head,10);
	printf("insert(10)\n");
	insertElement(head,2);
	printf("insert(2)\n");
	insertElement(head,8);
	printf("insert(8)\n");
	
	//Check isMember
	printf("\n");
	printf("isMember(5): %d \n",isMember(head,5));
	printf("isMember(2): %d \n",isMember(head,2));
	printf("isMember(10): %d \n",isMember(head,10));
	printf("isMember(7): %d \n",isMember(head,7));
	
	//Minimum element
	printf("\n");
	printf("minimum(): %d\n",minimum(head));
	
	//Maximum element
	printf("\n");
	printf("maximum(): %d\n",maximum(head));
	
	//Predecessor 
	printf("\n");
	printf("predecessor(10): %d\n",predecessor(head,10));
	
	//Successor
	printf("\n");
	printf("successor(2): %d\n",successor(head,2));
	
	//Delete an element
	printf("\n");
	delElement(head,5);
	printf("remove(5)\n");
	printf("isMember(5): %d \n",isMember(head,5));
	
	printf("\n");
	printf("successor(2): %d\n",successor(head,2));
	
	return 0;
}
