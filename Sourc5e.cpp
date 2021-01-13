#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#define MAX_LENGTH 1024

char getLet(char* str, int* isWordBeginning);
int isLetPresent(char let, char lettersValue[]);
void replaceLetNum(char* strLet, char* strNum, char lettersValue[]);
void replaceNumLet(char let, char* strNum, char lettersValue[]);
int isStringDone(char* str, int* success);


char* rebusSolve(char* str)
{
	static char lettersValue[10] = { NULL };
	static char strNum[1024];
	strncpy(strNum, str, strlen(str));

	char curLet = NULL;
	int isWordBeginning = 0;
	int success = 0;
	char* result = NULL;

	if (isStringDone(strNum, &success))
		if (success)
			return strNum;
		else return 0;

	do {
		curLet = getLet(str, &isWordBeginning);
		if (curLet == '\0')
			return 0;
	} while (isLetPresent(curLet, lettersValue) != -1);

	for (int i = isWordBeginning; i < 10; i++)
	{
		if (lettersValue[i] == NULL)
			lettersValue[i] = curLet;
		else continue;

		replaceLetNum(str, strNum, lettersValue);

		result = rebusSolve(strNum);
		if (!result)
		{
			replaceNumLet(curLet, strNum, lettersValue);
			lettersValue[i] = NULL;
		}
	}
	return result;
}

char getLet(char* str, int* isWordBeginning)
{
	char c;
	int i = 0;
	do {
		c = str[i];
		i++;
	} while (!isalpha(c) && c != '\0');

	if (isalpha(c) && (i - 1 == 0 || str[i - 2] == ' '))
		*isWordBeginning = 1;

	return c;
}

int isLetPresent(char let, char lettersValue[])
{
	for (int i = 0; i < 10; i++)
	{
		if (let == lettersValue[i])
			return i;
	}
	return -1;
}

void replaceLetNum(char* strLet, char* strNum, char lettersValue[])
{
	int num;
	for (int i = 0; strLet[i] != '\0'; i++)
	{
		num = isLetPresent(strLet[i], lettersValue);
		if (num != -1)
		{
			char letNum[2] = { '\0' };
			_itoa(num, letNum, 10);
			strNum[i] = letNum[0];
		}
	}
}

void replaceNumLet(char let, char* strNum, char lettersValue[])
{
	int num = isLetPresent(let, lettersValue);
	char letNum[2] = { '\0' };
	_itoa(num, letNum, 10);

	for (int i = 0; strNum[i] != '\0'; i++)
		if (strNum[i] == letNum[0])
			strNum[i] = let;
}

int isStringDone(char* str, int* success)
{
	int numbers[7] = { 0 };
	int answer = 0;
	int n = 0;
	char digit[2] = { '\0' };

	int i = 0;
	while (str[i] != '=')
	{
		if (isalpha(str[i]))
			return 0;
		if (isdigit(str[i]))
		{
			digit[0] = str[i];
			numbers[n] = numbers[n] * 10 + atoi(digit);
		}
		if (str[i] == '+')
			n++;
		i++;
	}
	while (str[i] != '\0')
	{
		if (isalpha(str[i]))
			return 0;
		if (isdigit(str[i]))
		{
			digit[0] = str[i];
			answer = answer * 10 + atoi(digit);
		}
		i++;
	}

	int sum = 0;
	for (int i = 0; i < n + 1; i++)
		sum += numbers[i];
	if (sum == answer)
		*success = 1;
	return 1;
}

int main()
{
	char strLet[MAX_LENGTH];

	gets_s(strLet, MAX_LENGTH);

	clock_t start, stop;
	start = clock();
	printf("%s\n", rebusSolve(strLet));
	stop = clock();
	printf("execution took %.3f seconds", ((float)(stop - start) / CLK_TCK));
	return 0;
}