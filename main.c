#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//要实现的功能：
//1.添加联系人
//2.通过关键字查询
//3.查看通讯录所有人信息
//4.查看联系人详细信息
//5.修改联系人相关信息
//6.删除联系人


//采用结构体(双向链表)来存储联系人的信息
typedef struct contact
{
	char (*name)[15];			//存储联系人姓名的字符数组指针，字符数组元素大小为15个
	char (*number)[13];			//存储联系人号码的字符数组指针，字符数组元素大小为13个
	struct contact *pre_link;	//指向前一个节点的指针
	struct contact *link;		//指向下一个联系人的指针
}contact, List;

//List *Initialize();									//初始化联系人列表
int TotalNumberOfContacts(List *ptr);   			//计算所有联系人的数量

//添加新的联系人
List *AddContact(char (*name)[15], char (*number)[13], List *PtrL);
List *FindByName(char (*name)[15], List *PtrL); 		//通过名字查询联系人
List *FindByNumber(char (*number)[13], List *PtrL); 	//通过号码查询联系人

/*删除联系人
*ptr是指向将被删除联系人的指针
*用二级指针的目的是能够修改头指针的地址，否则头指针一直指向原先地址。
*函数中以一级指针为形参传进去的指针，也只是能够修改实参指针所指向地址的变量的值，而不能修改
*指针本身地址
*/
void DeleteContact(List *ptr, List **PtrL);


//查看某个联系人的相关信息
void Show(List *ptr);

//查看所有联系人的相关信息
void ShowAll(List *PtrL);

//改进fgets()函数，去掉每当输入完后fgets()函数会自动添加"\n"
void GetString(char (*)[], int count);

//计算字符串的长度，就是将'\0'去掉后的长度
int GetLength(char *);

//比较两字符串是否相等,相等时返回1，不相等时返回0
int StrEqual(char (*)[], char (*)[]);

int main()
{
	printf("欢迎使用通讯录小程序！\n\n");

	//初始化头指针
 	List *PtrL = NULL;

	//指针用来标示现在在链表的那个位置
	List *ptr = NULL;
	
	//用来记录用户想要执行的操作
	char operator = '0';
	
	//声明一个指向有15个元素的字符数组的指针，用来存储用户输入的联系人姓名
	char (*name)[15];

	//声明一个指向有13个元素的字符数组的指针，用来存储用户输入的联系人联系方式
	char (*number)[13];

	//用来清空输入缓冲区
	char flush;

	while(operator != '6')
	{
		printf("请输入您需要进行的操作的序号\n");
		printf("1.查看所有人的相关信息\n");
		printf("2.查看某个人的相关信息\n");
		printf("3.添加新的联系人\n");
		printf("4.修改联系人的相关信息\n");
		printf("5.删除联系人\n");
		printf("6.退出程序\n");

		//规范用户的输入，只能输入1-6之间的数字
		do
		{
			printf("请输入1-6中的数字！\n");
			operator = getchar();
			
			//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
			while((flush = getchar()) != EOF && flush != '\n');
		}while(operator < 49 || operator > 54);//验证输入的有效性

		
		//根据用户的选择进行操作
		switch(operator)
		{
			case '1':
				ShowAll(PtrL);
				break;

			case '2':
				printf("请输入您要所查找的姓名:\n");

				//新申请一块能存15个字符数组的内存地址，若不申请，使用name指针将会访问不存在的地址
				//导致错误。
				name = (char (*)[15])malloc(sizeof(char)*15);
				GetString(name, 15);	//得到想要查找联系人的姓名
			
				//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
				while((flush=getchar()) != EOF && flush != '\n');

				//此指针在程序结束时自动释放，若提前手动释放将会导致相关操作访问非法区域，
				//ptr2,ptr3类似
				ptr = FindByName(name, PtrL);	

				Show(ptr);	//显示所查找联系人的姓名	
				
				free(name);	//如果这里不释放，将导致内存泄露
				break;
			
			case '3':
				//新申请一块能存15个字符数组的内存地址，若不申请，使用name指针将会访问不存在的地址
				//导致错误。
				name = (char(*)[15])malloc(sizeof(char)*15);
				printf("请输入您要新添加联系人的姓名:\n");
				GetString(name, 15);

				//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
				while((flush = getchar()) != EOF && flush != '\n');
			
				//新申请一块能存13个字符数组的内存地址，若不申请，使用number指针将会访问不存在的地址
				//导致错误。
				number = (char (*)[13])malloc(sizeof(char)*13);
				printf("请输入您要新添加联系人的手机号码:\n");
				GetString(number, 13);
				
				//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
				while((flush = getchar()) != EOF && flush != '\n');

				PtrL = AddContact(name, number, PtrL);	//添加联系人

				printf("添加成功\n");
				break;

			case '4':
				
				printf("请输入您要所修改的联系人姓名:\n");

				//新申请一块能存15个字符数组的内存地址，若不申请，使用name指针将会访问不存在的地址
				//导致错误。
				name = (char (*)[15])malloc(sizeof(char)*15);
				GetString(name, 15);	//得到想要查找联系人的姓名	
				
				//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
				while((flush = getchar()) != EOF && flush != '\n');

			 	ptr = FindByName(name, PtrL);	//这里如何进行释放指针

				//若所得的指针为空，则打印该了联系人不存在
				if(ptr == NULL)
					printf("该联系人\"%s\"不存在!\n", *name);
				else
				{
					char op;	//用来接收用户想要的操作行为

					printf("您要修改该联系人的姓名吗？[y/n]\n");
					do
					{
						printf("请输入y/n\n");
						op = getchar();
						//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
						while((flush = getchar()) != EOF && flush != '\n');

					}while(op != 121 && op != 110);//验证输入的有效性

					if(op == 121)
					{
						printf("请输入联系人新的姓名！\n");
						GetString(name, 15);
						
						//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
						while((flush = getchar()) != EOF && flush != '\n');

						strcpy(*ptr->name, *name);
						printf("修改该联系人姓名成功！\n\n");
					}

					printf("您要修改该联系人的联系方式吗？[y/n]\n");
					do
					{
						printf("请输入y/n\n");
						op = getchar();
						getchar();	//去除回车键
						fflush(stdin);
					}while(op != 121 && op != 110);//验证输入的有效性
					//新申请一块能存15个字符数组的内存地址，若不申请，使用name指针
					//将会访问不存在的地址导致错误。
					number = (char (*)[13])malloc(sizeof(char)*13);
				
					if(op == 121)
					{
						printf("请输入联系人新的联系方式！\n");
						GetString(number, 13);	//得到修改后的联系方式

						//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
						while((flush = getchar()) != EOF && flush != '\n');

						strcpy(*ptr->number, *number);
						printf("修改该联系人联系方式成功！\n\n");
					}

					free(number);	//不释放将导致内存泄露
				}

				free(name);	//不释放将导致内存泄露
				break;

			case '5':
				printf("请输入您要所删除联系人的姓名:\n");

				//新申请一块能存15个字符数组的内存地址，若不申请，使用name指针将会访问不存在的地址
				//导致错误。
				name = (char (*)[15])malloc(sizeof(char)*15);
				GetString(name, 15);	//得到想要删除联系人的姓名

				//清空输入缓冲区，否则将导致未清除的输入将作为下一目的输入的输入，gcc中fflush不管用。
				while((flush = getchar()) != EOF && flush != '\n');

				ptr = FindByName(name, PtrL);	//返回想要删除的联系人在链表中的指针

				DeleteContact(ptr, &PtrL);

				free(name);	//释放申请的内存，否则导致内存泄露。

				break;

			default:
				printf("欢迎下次使用\n");
		}

	}

	return 0;
}


//查看某个联系人的相关信息
void Show(List *ptr)
{
	if(ptr != NULL)
	{
		printf("姓名\t\t\t手机号码\n");
		printf("%s\t\t\t%s\n\n", *ptr->name, *ptr->number);
	}
	else
		printf("不存在该联系人！\n\n");

}


//查看所有联系人的相关信息
void ShowAll(List *PtrL)
{
	printf("姓名\t\t\t手机号码\n");
	List *p = PtrL;
	
	//当指针不为空时，输出联系人相关信息
	while(p != NULL)
	{
		printf("%s\t\t\t%s\n", *p->name, *p->number);
		p = p->link;	//指向下一个联系人
	}

	printf("\n\n");

}


//删除联系人
void DeleteContact(List *ptr, List **ptrPtrL)	//ptr是指向将被删除联系人的指针
{
	
	if(ptr == NULL)	//要删除的指针为空，删除不成功
	{
		printf("您当前没有联系人或者该联系人不存在，不能进行删除\n");
		return;
	}
	else
	{
		//当只有一个联系人时
		if(ptr->pre_link == NULL && ptr->link == NULL) 
		{
			printf("联系人%s删除成功！\n", *ptr->name);
			//需要改变头指针PtrL的值，否则头指针PtrL一直指向被释放的内存位置
			*ptrPtrL = NULL;
			free(ptr);

		}
		//当ptr为第一个节点时
		else if(ptr->pre_link == NULL)
		{
			//将ptr下一个节点的pre->link值赋位NULL,即该点成为头结点
			ptr->link->pre_link = NULL;	
			*ptrPtrL = ptr->link;	//需要改变头指针PtrL的值，否则头指针PtrL一直指向被释放的内存位置
			printf("联系人%s删除成功！\n", *ptr->name);
			free(ptr);
		}
		//当ptr为最后一个节点时
		else if(ptr->link == NULL)
		{
			ptr->pre_link->link = NULL;	//给ptr前一个节点的link赋值为NULL
			printf("联系人%s删除成功！\n", *ptr->name);
			free(ptr);
		}
		//当ptr既有前继也有后继时
		else
		{
			ptr->pre_link->link = ptr->link;
			ptr->link->pre_link = ptr->pre_link;
			printf("联系人%s删除成功！\n", *ptr->name);
			free(ptr);
		}
	}
}


//添加新的联系人
List *AddContact(char (*name)[15], char (*number)[13], List *PtrL)
{
	List *p, *s;

	//当添加第一位联系人时,初始化头指针
	if(PtrL == NULL)
	{
		PtrL = (List*)malloc(sizeof(contact));
		PtrL->name = name;
		PtrL->number =  number;
		PtrL->pre_link = NULL;
		PtrL->link = NULL;
		return PtrL;
	}
	else
	{
		//寻找到最后一名联系人的位置
		p = PtrL;
		while(p->link != NULL)
		{
			p = p->link;
		}

		s = (List*)malloc(sizeof(contact));
		s->name = name;
		s->number = number;

		p->link = s;
		s->pre_link = p; 
		s->link = NULL;
		return PtrL;
	}
}


//通过名字查询联系人，返回List类型的指针
List *FindByName(char (*name)[15], List *PtrL)
{
	//声明一个临时List类型的指针变量，并将此值进行返回
	List *ptr = PtrL;
	
	while(ptr != NULL && !StrEqual(name, ptr->name))
	{
		ptr = ptr->link;
	}

	//当所查联系人不在列表时，ptr为NULL；若存在时返回对应的ptr
	return ptr;
}


//通过电话号码查询联系人
List *FindByNumber(char (*number)[13], List *PtrL)
{
	List *ptr = PtrL;
	while(ptr != NULL && !StrEqual(number, ptr->number))
	{
		ptr = ptr->link;
	}

	//当所查的号码在联系人链表时，返回ptr;若不在，返回NULL
	return ptr;	//返回该联系人的指针
}


//计算所有联系人的数量
int TotalNumberOfContacts(List *PtrL)
{
	List *ptr = PtrL;	//ptr指向链表的第一个节点
	int i = 0;
	while(ptr != NULL)
	{
		ptr = ptr->link;//ptr指向链表的第i个节点
		i++;
	}

	return i;
}


//改良fget函数，fegts函数会自动添加'\n'到字符串中
void GetString(char (*str)[], int count)
{
	//使用fgets函数接受字符串，使用\0替换字符数组中的最后一位'\n'
	fgets(*str, count, stdin);

	//返回\n字符所在的指针
	char *find = strchr(*str, '\n');
	if(find)//如果找到了
		*find = '\0';	//根据找到的指针，修改指向的元素为'\0'
}


//返回字符串长度，即不算'\0'后的长度。
int GetLength(char *str)
{
	//用来统计字符的长度
	int count = 0;

	//首先要判断传进来的字符串指针是否为空
	if(str != NULL)
	{
		while(str[count] != '\0')
			count++;
	}

	return count;
}


//比较两字符串str1和str2是否相等，当返回值为1时相等，0时不相等。
int StrEqual(char (*str1)[], char (*str2)[])
{
	//计算字符串str1的长度
	int count1;
	
	//计算字符串str2的长度
	int count2;

	//判断传进来的字符串指针str1和str2是否为空，为空时不相等
	if(str1 == NULL || str2 == NULL)
		return 0;
	else
	{
		count1 = GetLength(*str1);
		count2 = GetLength(*str2);

		//当两个字符串长度相等时，两字符串才能相等
		if(count1 == count2)
		{
			//当所查联系人为头结点时,判断两个字符数组的每个字符是否相等
			for(int i=0; i<count1; i++)
			{
				//如果不同，跳出循环
				if((*str1)[i] != (*str2)[i])
				{
					return 0;
				}
			}

			//当循环结束后，没有返回0，就返回1
			return 1;
		}
		else	//两字符串长度不相等
			return 0;
	}
}

		
