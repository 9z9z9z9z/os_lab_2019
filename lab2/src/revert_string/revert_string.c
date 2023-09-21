#include "revert_string.h"

void RevertString(char *str)
{
	int length = strlen(str);
    int start = 0;
    int end = length - 1;

    while (start < end) {
        // Обмениваем символы между началом и концом строки
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Перемещаем указатели к центру
        start++;
        end--;
    }
}

