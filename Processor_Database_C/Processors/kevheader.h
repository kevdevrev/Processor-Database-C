#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>
#define CLS system("cls")
#include <ctype.h>
#define PAUSE system("pause")
#define FLUSH myFlush()
void myFlush() {
	while (getchar() != '\n');
} // end myFlush

void sentenceCase(char s[]) {
		int i;
		for (i = 0; i < strlen(s); i++) {
			s[i] = tolower(s[i]);
		}
		for (i = 0; i < strlen(s); i++)
		{
			if (i == 0 || s[i - 1] == ' '&&s[i] >= 'a'&&s[i] <= 'z')
				s[i] = toupper(s[i]);
		}
	}//my own function to setence case strings.
