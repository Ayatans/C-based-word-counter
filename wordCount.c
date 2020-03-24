#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h> 
//定义最终储存单词和出现次数的表 
typedef struct wordlist					
{
	char word[100];
	int t;
};
//定义字母树
typedef struct Word
{
	struct Word *next[26];	//存储26个英文字母
	int num;
};
struct wordlist list[1000000];			//所有单词表
char str[100] = "";					
int size = 0;							
//以字母树方式新建或计数
void wordtree(char *str, struct Word *qidian)
{
	int length = strlen(str), location;
	struct Word *p = qidian, *q;
	int i, j;
	for (i = 0; i < length; i++)//遍历单词判断当前字符是否为字母或'
	{
		if (str[i] >= 'a' && str[i] <= 'z')			//一个字母存一下,并通过该字母与a的距离获得数组的下标
			location = str[i] - 97;

		if (p->next[location] == NULL)					//若已到达链表结尾，开辟新的表存入字母
		{
			q = (struct Word *)malloc(sizeof(struct Word));
			for (j = 0; j < 26; j++)				//开辟一个空的表的过程
			{
				q->num = 0; 
				q->next[j] = NULL;
			}
			p->next[location] = q;						//q连在p的后面，将q和原来的链相连
			p = p->next[location];						//p后移一位到最后一位上
		}
		else										//若未到达链表结尾，指针指向下一个
		{
			p = p->next[location];
		}
	}
	p->num++;
}
//将字符数组还原成单词
void reverse(struct Word *p, int len)
{
	int i;
	for (i = 0; i < 26; i++)
	{
		if (p->next[i] != NULL)
		{
			str[len] = 97 + i;
			len++;
			reverse((struct Word*)p->next[i], len);
			len--;
		}
	}
	if (p->num != 0)
	{
		str[len] = '\0';
		strcpy(list[size].word, str);    //如果遇到单词结束标志，将str存入list[size].word，将对应num存入list里的词频计数
		list[size].t = p->num;		
		size++;							 //移动到下一个单词存储空间
	}
}
//变成小写  
char xiaoxie(char ab)
{
	if (ab >= 'A'&& ab <= 'Z')
	{
		ab = ab + 32;
	}
	return ab;
}
//冒泡排序
void paixu(struct wordlist a[], int n)
{
	struct wordlist temp;
	int i, j;
	for (i = 0; i < n - 1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (a[i].t > a[j].t || (a[i].t == a[j].t&&strcmp(a[i].word, a[j].word) > 0))
			{
				temp.t = a[i].t;
				a[i].t = a[j].t;
				a[j].t = temp.t;
				strcpy(temp.word, a[i].word);
				strcpy(a[i].word, a[j].word);
				strcpy(a[j].word, temp.word);
			}
		}
	}
}
//去掉单词中的'-
void rid(char w[], char c, char d)
{
	int i, j = 0;
	for (i = 0; i < strlen(w); i++)
	{
		if ((w[i] == c) || (w[i] == d))
		{
			for (j = i; j < strlen(w); j++)
			{
				if (strlen(w) - j == 1)
				{
					w[j] = '\0';
					break;
				}
				w[j] = w[j + 1];
			}
		}
	}
}

int main()
{
	long op, ed;
	op = clock();
	struct Word *head = (struct Word*)malloc(sizeof(struct Word));
	int i,len=0;
	char x;
	char zancun[100];
	for (i = 0; i < 26; i++)
		head->next[i] = NULL;
	FILE *f, *out;
	f = fopen("input.txt", "r");
	while ((x = fgetc(f)) != EOF)		//从txt文档中读入且x不是文档的结束处
	{									//是否是所需的4种字符？
		if ((x >= 'A'&&x <= 'Z') || (x >= 'a'&&x <= 'z') || (x == '\''&&len != 0) || (x == '-'&&len != 0))
		{
			zancun[len] = xiaoxie(x);
			len++;
		}
		else {
			if (zancun[0] == '\0')	//判断是否为空
				continue;
			zancun[len] = '\0';		//不空就说明是一个单词，要进行处理
			rid(zancun, 39, 45);		//去除'和-
			wordtree(zancun,head);	//导入字母树里
			len = 0;
		}
	}		
	fclose(f);
	reverse(head, 0);					//还原单词
	paixu(list, size);					//排序单词
	//输出单词和对应词频，屏幕显示+写入文件
	out = fopen("output.txt", "w");
	for (i = 0; i < size-1; i++)			
	{
		fprintf(out, "单词：%s   出现次数：%d\n", list[i].word, list[i].t);
		//printf("单词：%s   出现次数：%d\n", list[i].word, list[i].t);
	}
	fclose(out);
	ed = clock();
	printf("%ldms\n", ed - op);   //输出程序运行时间
	system("pause");
	return 0;
}

