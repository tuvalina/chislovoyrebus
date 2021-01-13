#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>


#define MAX_WORDS 7
#define MAX_LENGTH 1024
#define NOT_PRESENT -1

char getletter(char* str, char words[][MAX_LENGTH], char* answer, int* isWordBeginning);
int isLetPresent(char let, char lettersValue[]);
int checkWordBeginning(char c, char* str);
void replaceLetNum(char* strLet, char* strNum, char lettersValue[]);
void replaceNumLet(char let, char* strNum, char lettersValue[]);
void isRebusDone(char* str, char words[][MAX_LENGTH], char* answer, int* lettersRemain, int* success);
void tokeniseStr(char* str, char words[][1024], char* answer);
int isFirstLetter(char words[][MAX_LENGTH]);

char* rebusSolve(char* str)
{
	static char lettersValue[10] = { 0 };
	static char strNum[MAX_LENGTH];
	strncpy(strNum, str, strlen(str));

	char curLet = 0;
	int isWordBeginning = 0;
	int success = 0;
	int lettersRemain = 0;
	char* result = NULL;

	static char words[MAX_WORDS][MAX_LENGTH] = { 0 };
	static char answer[MAX_LENGTH] = { 0 };

	tokeniseStr(strNum, words, answer);
	isRebusDone(strNum, words, answer, &lettersRemain, &success);

	if (success)
	{
		if (!lettersRemain)
			return strNum;
	}
	else
		return 0;

	curLet = getletter(strNum, words, answer, &isWordBeginning);
	if (curLet == '\0')
		return 0;


	for (int i = isWordBeginning; i < 10; i++)
	{
		if (lettersValue[i] == 0)
			lettersValue[i] = curLet;
		else continue;

		replaceLetNum(str, strNum, lettersValue);

		result = rebusSolve(strNum);
		if (!result || isFirstLetter(words))
		{
			replaceNumLet(curLet, strNum, lettersValue);
			lettersValue[i] = 0;
		}
	}
	return result;
}

char getletter(char* str, char words[][MAX_LENGTH], char* answer, int* isWordBeginning)
{
	char letter = 0;
	char c = 0;
	int found = 0;
	int strlenWords[MAX_WORDS];
	for (int i = 0; i < MAX_WORDS && words[i][0]; i++)
		strlenWords[i] = strlen(words[i]);
	int strlenAnswer = strlen(answer);

	for (int i = 1; !found; i++)
	{
		for (int j = 0; words[j][0] && !found; j++)
		{

			c = words[j][strlenWords[j] - i];
			if (isalpha(c))
			{
				letter = c;
				found = 1;
			}
		}
		c = answer[strlenAnswer - i];
		if (isalpha(c))
		{
			letter = c;
			found = 1;
		}
	}

	if (found && checkWordBeginning(letter, str))
		*isWordBeginning = 1;

	return letter;
}

int checkWordBeginning(char c, char* str)
{
	if (str[0] == c)
		return 1;
	int i = 1;
	while (str[i])
	{
		if (str[i] == c && str[i - 1] == ' ')
			return 1;
		i++;
	}
	return 0;
}

int isLetPresent(char let, char lettersValue[])
{
	for (int i = 0; i < 10; i++)
	{
		if (let == lettersValue[i])
			return i;
	}
	return NOT_PRESENT;
}

void replaceLetNum(char* strLet, char* strNum, char lettersValue[])
{
	int num;
	for (int i = 0; strLet[i] != '\0'; i++)
	{
		num = isLetPresent(strLet[i], lettersValue);
		if (num != NOT_PRESENT)
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

void isRebusDone(char* str, char words[][MAX_LENGTH], char* answer, int* lettersRemain, int* success)
{
	int n = 0;
	char c;
	char digit[2] = { '\0' };
	int numSum[MAX_WORDS] = { 0 };
	int numSumAll = 0;
	int numAnswer = 0;
	int minNumOfDigits = 0;

	int strlenWords[MAX_WORDS];
	for (int i = 0; i < MAX_WORDS && words[i][0]; i++)
		strlenWords[i] = strlen(words[i]);
	int strlenAnswer = strlen(answer);

	for (int i = 1; !*lettersRemain && i < strlenAnswer + 1; i++)
	{
		for (int j = 0; words[j][0] && !*lettersRemain; j++)
		{
			c = words[j][strlenWords[j] - i];
			if (isdigit(c))
			{
				digit[0] = c;
				numSum[j] += atoi(digit) * (int)pow(10, minNumOfDigits);
			}
			if (isalpha(c))
			{
				*lettersRemain = 1;
			}
		}
		c = answer[strlenAnswer - i];
		if (isdigit(c))
		{
			digit[0] = c;
			numAnswer += atoi(digit) * (int)pow(10, minNumOfDigits);
		}
		if (isalpha(c))
		{
			*lettersRemain = 1;
		}

		if (!*lettersRemain) minNumOfDigits++;
	}

	for (int i = 0; i < MAX_WORDS; i++)
	{
		numSumAll += numSum[i];
	}

	if (!minNumOfDigits)
		*success = 1;
	if (!*lettersRemain)
	{
		if (numSumAll == numAnswer)
			*success = 1;
	}
	else
	{
		int numDivider = (int)pow(10, minNumOfDigits);
		if (numSumAll % numDivider == numAnswer % numDivider)
			*success = 1;
	}
}

void tokeniseStr(char* str, char words[][1024], char* answer)
{
	int i = 0;
	int j = 0;
	int wordsCount = 0;
	while (str[i] != '=')
	{
		if (isalnum(str[i]))
		{
			words[wordsCount][j] = str[i];
			j++;
		}
		if (str[i] == '+')
		{
			wordsCount++;
			j = 0;
		}
		i++;
	}
	j = 0;
	while (str[i])
	{
		if (isalnum(str[i]))
		{
			answer[j] = str[i];
			j++;
		}
		i++;
	}
}

int isFirstLetter(char words[][MAX_LENGTH])
{
	int i = 0;
	int j = 0;

	for (i = 0; words[i][0]; i++)
		if (words[i][0] == '0')
			return 1;

	return 0;
}

int main()
{
	char strLet[MAX_LENGTH];

	gets_s(strLet, MAX_LENGTH);

	clock_t start, stop;
	start = clock();
	printf("%s\n", rebusSolve(strLet));
	stop = clock();
	printf("Program running time %.3f seconds", ((float)(stop - start) / CLK_TCK));
	return 0;
}