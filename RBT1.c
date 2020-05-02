#include<stdio.h>
#include<stdlib.h>

enum COLOR{RED,BLACK};

typedef struct node
{
	int nvalue;
	int ncolor;
	struct node *pleft;
	struct node*pright;
	struct node *pfather;
}rbt;


rbt *prbt = NULL;

//右旋
void right(rbt **ptree)
{
	if(*ptree == NULL || (*ptree)->pleft == NULL)return ;
	rbt *pnode = *ptree;
	rbt *pmark = pnode->pleft;

	pnode->pleft = pmark->pright;
	pmark->pright = pnode;

	if(pnode->pfather == NULL)
	{
		prbt = pmark;	
	}
	else
	{
		if(pnode == pnode->pfather->pleft)
		{
			pnode->pfather->pleft = pmark;	

		}
		else
		{
			pnode->pfather->pright = pmark;	
		}
	}

	if(pnode->pleft!=NULL)
	{
		pnode->pleft->pfather = pnode;	
	}
	pmark->pfather = pnode->pfather;
	pnode->pfather = pmark;
}


//左旋
void left(rbt **ptree)
{
	if(*ptree == NULL || (*ptree)->pright == NULL)return ;
	printf("-----\n");
	rbt *pnode = *ptree;
	rbt *pmark = pnode->pright;

	pnode->pright = pmark->pleft;
	pmark->pleft = pnode;

	if(pnode->pfather == NULL)
	{	
		prbt = pmark;	
	}
	else
	{
		if(pnode == pnode->pfather->pleft)
		{
			pnode->pfather->pleft = pmark;	

		}
		else
		{
			pnode->pfather->pright = pmark;	

		}
	}
	if(pnode->pright!=NULL)
	{
		pnode->pright->pfather = pnode;	
	}
	pmark->pfather = pnode->pfather;
	pnode->pfather = pmark;
}
//查找
rbt *search(rbt*ptree,int num)
{
	while(ptree)
	{
		if(ptree->nvalue>num)
		{
			if(ptree->pleft ==NULL)
			{
				return ptree;	
			}
			ptree = ptree->pleft;	
		}
		else if(ptree->nvalue<num)
		{
			if(ptree->pright==NULL)
			{
				return	ptree;	
			}
			ptree = ptree->pright;	
		}
		else
		{	
			printf("数据错误\n");
			exit(1);
		}			
	}

	return NULL;

}

rbt  *getuncle(rbt *pnode)//要找叔叔，传入的是父亲节点
{
	if(pnode == pnode->pfather->pright)//如果父亲是爷爷右，
	{
		return	 pnode->pfather->pleft;//叔叔是爷爷左   
	}
	else
	{
		return	 pnode->pfather->pleft;//叔叔是爷爷右   
	}


}
void addnode(rbt *ptree ,int num)
{
	//查找要插入的位置     search()函数返回的是要添加节点的父亲节点
	rbt *pnode = NULL;//父亲节点
	//父亲节点
	pnode = search(prbt,num);

	//要插入的节点
	rbt *ptemp = NULL;
	ptemp = (rbt *)malloc(sizeof(rbt));
	ptemp->nvalue = num;
	ptemp->ncolor = RED;
	ptemp->pright = NULL;
	ptemp->pleft = NULL;
	ptemp->pfather = pnode;

	if(pnode == NULL)//树中无节点
	{
		prbt = ptemp;//创建的树的根		
		ptemp->ncolor = BLACK;
		return ;
	}

	//先将新来的和父亲相连     因为父子关系不会变化只是需要颜色调整
	if(pnode->nvalue>num)
	{
		pnode->pleft = ptemp;
	}
	else
	{
		pnode->pright = ptemp;
	}

	//2.1父亲是黑---连接父子就结束
	if(pnode->ncolor ==BLACK)
	{
		return;	
	}

	rbt *puncle =NULL ;//叔叔
	rbt *pgreadfather = NULL;// 爷爷
	//2.2父亲红色

	while(pnode->ncolor == RED)
	{
		puncle = getuncle(pnode); //叔叔
		pgreadfather = pnode->pfather;//爷爷
		//2.1.1 叔叔红
		if(puncle !=NULL && puncle->ncolor == RED)
		{
			pnode->ncolor = BLACK;
			puncle->ncolor = BLACK;
			pgreadfather->ncolor =RED;

			ptemp = pgreadfather;//爷爷为新Z  重新讨论
			pnode = ptemp->pfather;	//爷爷变了，父亲也得变
			//讨论原来爷爷是否是根   原来爷爷就是现在的新Z的父亲  即此时的pnode 
			if(pnode ==NULL)
			{
				prbt->ncolor = BLACK;
				break;
			}
			continue;
		}
		//2.1.2叔叔是黑
		if(puncle ==NULL ||  puncle->ncolor == BLACK)
		{
			//2.1.2.1//父亲是左
			if(pnode == pgreadfather->pleft)
			{
				//3.2.1.1 --------Z 为父亲右子
				if(ptemp = pnode->pright)
				{
					ptemp = pnode;//父亲成为新Z
					//旋转后父亲发生变化  爷爷不变
					//先以新Z为支点  左旋
					left(&ptemp);
					pnode = ptemp->pfather;//父亲变化
					continue;
				}

				//2.2.2.1.2  Z为父亲的左
				if(ptemp = pnode->pleft)
				{

					pnode->ncolor = BLACK;
					pgreadfather->ncolor = RED;
					right(&pgreadfather);
					break;
				}



			}

			//1父亲是爷爷右
			if(pnode == pgreadfather->pright)
			{

				//1.1 新Z 是父亲的左
				if(ptemp == pnode->pleft)
				{
					ptemp = pnode;//父亲为新Z	
					//以新Z为支点右旋
					right(&ptemp);
					//右旋完后父亲会发生变化
					pnode = ptemp->pfather;
					continue;
				}
				//1.2 新Z是父亲的右
				if(ptemp == pnode->pright)
				{
					pnode->ncolor = BLACK;
					pgreadfather->ncolor = RED;
					left(&pgreadfather);//以爷爷为支点左旋
					break;
				}

			}

		}	
	}
}

void createrbt(int arr[],int nlength)
{
	if(arr ==NULL || nlength<=0)return NULL;
	int i;
	for(i = 0;i<nlength;i++)
	{
		//向树中添加节点
		addnode(prbt,arr[i]);

	}




}

void qianxu(rbt *ptree)
{
	if(ptree==NULL)return;
	printf("val == %d\tcol == %d\n",ptree->nvalue,ptree->ncolor);
	qianxu(ptree->pleft);
	qianxu(ptree->pright);
}

int main()
{
	int arr[] = {11,2,14,1,7,15,5,8};
	createrbt(arr,sizeof(arr)/sizeof(arr[0]));
	qianxu(prbt);


	return 0;
} 
