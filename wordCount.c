#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h> 
//�������մ��浥�ʺͳ��ִ����ı� 
typedef struct wordlist					
{
	char word[100];
	int t;
};
//������ĸ��
typedef struct Word
{
	struct Word *next[26];	//�洢26��Ӣ����ĸ
	int num;
};
struct wordlist list[1000000];			//���е��ʱ�
char str[100] = "";					
int size = 0;							
//����ĸ����ʽ�½������
void wordtree(char *str, struct Word *qidian)
{
	int length = strlen(str), location;
	struct Word *p = qidian, *q;
	int i, j;
	for (i = 0; i < length; i++)//���������жϵ�ǰ�ַ��Ƿ�Ϊ��ĸ��'
	{
		if (str[i] >= 'a' && str[i] <= 'z')			//һ����ĸ��һ��,��ͨ������ĸ��a�ľ�����������±�
			location = str[i] - 97;

		if (p->next[location] == NULL)					//���ѵ��������β�������µı������ĸ
		{
			q = (struct Word *)malloc(sizeof(struct Word));
			for (j = 0; j < 26; j++)				//����һ���յı�Ĺ���
			{
				q->num = 0; 
				q->next[j] = NULL;
			}
			p->next[location] = q;						//q����p�ĺ��棬��q��ԭ����������
			p = p->next[location];						//p����һλ�����һλ��
		}
		else										//��δ���������β��ָ��ָ����һ��
		{
			p = p->next[location];
		}
	}
	p->num++;
}
//���ַ����黹ԭ�ɵ���
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
		strcpy(list[size].word, str);    //����������ʽ�����־����str����list[size].word������Ӧnum����list��Ĵ�Ƶ����
		list[size].t = p->num;		
		size++;							 //�ƶ�����һ�����ʴ洢�ռ�
	}
}
//���Сд  
char xiaoxie(char ab)
{
	if (ab >= 'A'&& ab <= 'Z')
	{
		ab = ab + 32;
	}
	return ab;
}
//ð������
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
//ȥ�������е�'-
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
	while ((x = fgetc(f)) != EOF)		//��txt�ĵ��ж�����x�����ĵ��Ľ�����
	{									//�Ƿ��������4���ַ���
		if ((x >= 'A'&&x <= 'Z') || (x >= 'a'&&x <= 'z') || (x == '\''&&len != 0) || (x == '-'&&len != 0))
		{
			zancun[len] = xiaoxie(x);
			len++;
		}
		else {
			if (zancun[0] == '\0')	//�ж��Ƿ�Ϊ��
				continue;
			zancun[len] = '\0';		//���վ�˵����һ�����ʣ�Ҫ���д���
			rid(zancun, 39, 45);		//ȥ��'��-
			wordtree(zancun,head);	//������ĸ����
			len = 0;
		}
	}		
	fclose(f);
	reverse(head, 0);					//��ԭ����
	paixu(list, size);					//���򵥴�
	//������ʺͶ�Ӧ��Ƶ����Ļ��ʾ+д���ļ�
	out = fopen("output.txt", "w");
	for (i = 0; i < size-1; i++)			
	{
		fprintf(out, "���ʣ�%s   ���ִ�����%d\n", list[i].word, list[i].t);
		//printf("���ʣ�%s   ���ִ�����%d\n", list[i].word, list[i].t);
	}
	fclose(out);
	ed = clock();
	printf("%ldms\n", ed - op);   //�����������ʱ��
	system("pause");
	return 0;
}

