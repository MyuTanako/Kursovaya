#define _CRT_SECURE_NO_WARNINGS
#define MAXLEN 1000
#define WINDOW 8
#define YES 1
#define NO 0
#define INPFILE "wordsREAD"
#include <stdio.h>
#include <stdlib.h>

struct words
{
	int pos;
	int length;
	char word[32];
	struct words *next;
};

struct words* init(void) // ����������� ������ ��� ������ ������
{
	struct words *lst;
	lst = (struct words*)malloc(sizeof(struct words));
	lst->next = NULL;
	return(lst);
}

struct words* Appendlist(struct words* headRef)
{
	struct words* current = headRef;
	struct words* lst;
	lst = (struct words*)malloc(sizeof(struct words));
	lst->next = NULL;
	// ���� ������ ����
	if (current == NULL) {
		headRef = lst;
	}
	else {
		// �����
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = lst;
	}
	return(lst);
}


void freeStruct(struct words * head)
{
	struct words * current = head;
	struct words * next = NULL;
	do
	{
		next = current->next;
		free(current);
		current = next;
	} while (current != NULL);
	free(current);
}

void inputFile(struct words * head)
{
	int pos = 0, pastPos = 0;
	int length = 1, pastLength = 1;
	int flag = NO;
	FILE *finp;
	FILE *fout;
	char *ptr;
	char line[MAXLEN];
	char word[32], pastWord[32];
	int empty;
	struct words * current;
	struct words * newWord;
	finp = fopen("wordsREAD.txt", "r");
	fout = fopen("wordsREC.txt", "w");
	while (!feof(finp))
	{
		ptr = fgets(line, sizeof(line), finp);
		pos = 0;
		do
		{
			empty = NO;
			word[length-1] = *ptr;
			word[length] = '\0';
			current = head;
			while (current != NULL)
			{
				if (!strcmp(word, current->word))
				{
					length++;
					pastPos = current->pos;
					pastLength = current->length;
					strcpy(pastWord, current->word);
					empty = YES;
					break;
				}
				current = current->next;
			}
			if (!empty)
			{
				newWord = Appendlist(head);
				newWord->length = length;
				newWord->pos = pos - length + 1;
				strcpy(newWord->word, word);
				if (flag)
				{
					fprintf(fout, "[%d|%d]", pastPos, pastLength);
					flag = NO;
				}
				fprintf(fout, "%s", word+length-1);
				length = 1;
			}
			else
			{
				flag = YES;
			}
			pos++;
			ptr++;
		} while (*ptr || *ptr == '\n');
		if (flag)
		{
			fprintf(fout, "[%d|%d]", pastPos, pastLength);
		}
	}
	fclose(fout);
	fclose(finp);
}

void deCode(void)
{
	FILE *finp;
	FILE *fout;
	fout = fopen("wordsRECdecode.txt", "w");
	finp = fopen("wordsREC.txt", "r");
	char *ptr;
	char *begin;
	char buf[100];
	char newLine[MAXLEN];
	int pos, length, posBegin;
	int i, gPos = 0;
	int j = 0;
	char line[MAXLEN];
	while (!feof(finp))
	{
		ptr = fgets(line, sizeof(line), finp);
		do
		{
			if (*ptr == '[')
			{
				begin = ptr;
				posBegin = gPos;
				gPos++;
				ptr++;
				i=0;
				while (*ptr != '|')
				{
					buf[i] = *ptr;
					i++;
					ptr++;
					gPos++;
				}
				buf[i] = '\0';
				pos = atoi(buf);
				ptr++;
				gPos++;
				i = 0;
				while (*ptr != ']')
				{
					buf[i] = *ptr;
					i++;
					ptr++;
					gPos++;
				}
				buf[i] = '\0';
				length = atoi(buf);
				for (i = 0; i < length; i++)
				{
					newLine[j++] = newLine[pos + i];
				}
			}
			else
			{
				newLine[j++] = *ptr;
			}
			gPos++;
		} while (*ptr++);
		fprintf(fout, "%s", newLine);
	}
	fclose(finp);
	fclose(fout);
}

void recordData(struct words* headRef)
{
	FILE *fpin;
	struct words* current = headRef->next;
	fpin = fopen("slovar.txt", "w");
	while (current != NULL)
	{
		fprintf(fpin, "%s %d %d\n", current->word, current->pos, current->length);
		current = current->next;
	}
	fclose(fpin); // ������� ������� ����
}

void main(void)
{
	struct words* firstWord;
	char callback_main;
	//firstWord = inputWords();
	/* ���������� ���������� �������*/
	system("chcp 1251");
	system("cls");
	do
	{
		system("cls");
		printf("[0] ����� �� ���������\n[1] ���������� �����\n[2] ������������ �����\n�������� ����� �� ������ : ");
		scanf("%s", &callback_main);
		switch (callback_main)
		{
		case '1': 
			system("cls");
			firstWord = init();
			inputFile(firstWord);
			printf("������ � ���� wordREC\n");
			recordData(firstWord);
			freeStruct(firstWord);
			system("pause");
			break;
		case '2':
			system("cls");
			deCode();
			printf("������ � ���� wordRECdecode\n");
			system("pause");
			break;
		default:
			break;
		}
	} while (callback_main != '0');
}