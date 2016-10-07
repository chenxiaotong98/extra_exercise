/*
This program implements calculation (+, -, *, /, exp()) of high precision numbers, with 100000 digits.
Algorithm:
Use arrays to store and process each digit of the huge number;
add: directly add each digit after aligning the position, then add the carry
    from the last digit to the highest digit;
subtract: align position, use the longer number as the object to be subtracted,
    from the last digit to the highest digit, considering the borrowed '1';
multi: calculate product matrix of each digit, then add the carry
divide: first calculate the division of a number with proper digits by the divider,
    using single-multi as test; then calculate the desired quotient.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>>

#define MAXIMUM_LENGTH 100000

char* rand_generate(int);
char* add(char*, char*);
char* subtract(char*, char*);
char* multi(char*, char*);
char* expo(char*, int);
char* divide(char*, char*);
char* divide_improved(char*, char*);

int main()
{
    char* operand1 = "121411242"; char* operand2 = "87";
    char* sum; char* difference; char* product;
    char* exponent; char* quotient;
    //operand1 = rand_generate(MAXIMUM_LENGTH);
    //operand2 = rand_generate(MAXIMUM_LENGTH);
    sum = add(operand1, operand2);
    difference = subtract(operand1, operand2);
    product = multi(operand1, operand2);
    exponent = expo(operand1, 3);
    //quotient = divide(operand1, operand2); puts(quotient);
    quotient = divide_improved(operand1, operand2);

    printf("OK!\n");
    return 0;
}

char* rand_generate(int len)
{
    srand((unsigned)time(NULL));
    char* a = (char*)malloc(sizeof(char) * len);
    int i;
    for (i = 0; i < len; i++)
    {
        a[i] = rand() % 10 + '0';
    }
    while (a[0] == '0')
    {
        a[0] = rand() % 10 + '0';
    }
    return a;
}

char* add(char* a, char* b)
{
    char* c;
    if (a == NULL || b == NULL)
    {
        printf("error: no input\n");
        return NULL;
    }
    int len_a, len_b;
    len_a = strlen(a); len_b = strlen(b);
    int carry;
    if (len_a < len_b)
    {
        return add(b, a);
    }
    int* sum = (int*)malloc((len_a + 1) * sizeof(int));
    sum[0] = 0;
    int* temp = sum + 1;
    int i;
    for (i = len_a - 1; i > len_a - len_b - 1; i--)
    {
        temp[i] = a[i] - '0' + b[i + len_b - len_a] - '0';
    }
    for (i = len_a - len_b - 1; i >= 0; i--)
    {
        temp[i] = a[i] - '0';
    }
    for (i = 0; i < len_a; i++)
    {
        carry = temp[len_a - 1 - i] / 10;
        temp[len_a - 1 - i] = temp[len_a - 1 - i] % 10;
        temp[len_a - i - 2] += carry;
    }
    if (sum[0] == 1)
    {
        c = (char*)malloc(sizeof(char) * (len_a + 2));
        for (i = 0; i <= len_a; i++)
        {
            c[i] = sum[i] + '0';
        }
        c[len_a + 1] = '\0';
    }
    else
    {
        c = (char*)malloc(sizeof(char) * (len_a + 1));
        for (i = 0; i < len_a; i++)
        {
            c[i] = temp[i] + '0';
        }
        c[len_a] = '\0';
    }
    free(sum);
    return c;
}

char* subtract(char* a, char* b)
{
    char* c;
    if (a == NULL || b == NULL)
    {
        printf("error: no input\n");
        return NULL;
    }
    if ((strlen(a) < strlen(b)) || (strlen(a) == strlen(b) && strcmp(a, b) < 0))
    {
        c = subtract(b, a);
        char* c1 = (char*)malloc(sizeof(char) * (strlen(c) + 1));
        c1[0] = '-';
        strcpy(c1 + 1, c);
        return c1;
    }
    if (strcmp(a, b) == 0)
    {
        c = (char*)malloc(2 * sizeof(char));
        c[0] = '0'; c[1] = '\0';
        return c;
    }
    int len_a, len_b;
    len_a = strlen(a); len_b = strlen(b);
    int* diff = (int*)malloc(len_a * sizeof(int));
    int i;
    int minus_carry = 0;
    for (i = len_a - 1; i >= len_a - len_b; i--)
    {
        diff[i] = a[i] - b[i + len_b - len_a];
    }
    for (i = len_a - len_b - 1; i >= 0; i--)
    {
        diff[i] = a[i] - '0';
    }
    for (i = len_a - 1; i >= 0; i--)
    {
        diff[i] += minus_carry;
        if (diff[i] < 0)
        {
            minus_carry = -1;
            diff[i] += 10;
        }
        else
        {
            minus_carry = 0;
        }
    }
    for (i = 0; i < len_a ; i++)
    {
        if (diff[i] != 0)
            break;
    }
    int len_c = len_a - i;
    c = (char*)malloc(sizeof(char) * (len_c + 1));
    for (i = 0; i < len_c; i++)
    {
        c[i] = diff[i + len_a - len_c] + '0';
    }
    c[len_c] = '\0';
    free(diff);
    return c;
}

char* multi(char* a, char* b)
{
    if (a == NULL || b == NULL)
    {
        printf("error: no input\n");
        return NULL;
    }
    int len_a = strlen(a); int len_b = strlen(b);
    if (len_a < len_b)
    {
        return multi(b, a);
    }
    int* temp = (int*)malloc(sizeof(int) * (len_a + len_b));
    int i, j, k;
    for (i = len_a + len_b - 1; i >= len_b; i--)
    {
        temp[i] = 0;
        for (j = len_b - 1, k = i - 1 - j; j >= 0 && k < len_a; j--, k++)
        {
            temp[i] += (a[k] - '0') * (b[j] - '0');
        }
    }
    for (i = len_b - 1; i > 0; i--)
    {
        temp[i] = 0;
        for (k = 0, j = i - 1 - k; k < len_b && j >= 0; j--, k++)
        {
            temp[i] += (a[k] - '0') * (b[j] - '0');
        }
    }
    int carry = 0;
    for (i = len_a + len_b - 1; i > 0; i--)
    {
        temp[i] += carry;
        carry = temp[i] / 10;
        temp[i] = temp[i] % 10;
    }
    if (carry != 0)
    {
        temp[0] = carry;
        char* c = (char*)malloc(sizeof(char) * (len_a + len_b + 1));
        for (i = 0; i < len_a + len_b; i++)
        {
            c[i] = temp[i] + '0';
        }
        c[len_a + len_b] = '\0';
        return c;
    }
    else
    {
        char* c = (char*)malloc(sizeof(char) * (len_a + len_b));
        for (i = 0; i < len_a + len_b - 1; i++)
        {
            c[i] = temp[i + 1] + '0';
        }
        c[len_a + len_b - 1] = '\0';
        return c;
    }
}

char* expo(char* a, int n)
{
    int i;
    char* c = "1";
    for (i = 0; i < n; i++)
    {
        c = multi(a, c);
    }
    return c;
}

int compare_str(char* a, char* b)//for function "divide"
{
    int i; int len_a = strlen(a); int len_b = strlen(b);
    if (len_a > len_b)
        return 1;
    else if (len_a < len_b)
        return -1;
    else
    {
        for (i = 0; i < len_a; i++)
        {
            if (a[i] != b[i])
                break;
        }
        return (a[i] - b[i]);
    }
}

char* divide(char* a, char* b)
{
    if (a == NULL || b == NULL)
    {
        printf("error: no input\n");
        return NULL;
    }
    int len_a = strlen(a); int len_b = strlen(b);
    if (len_a < len_b || (len_a == len_b && strcmp(a, b) < 0))
    {
        char* c = "0";
        return c;
    }
    if(strcmp(a, b) == 0)
    {
        char* c = "1";
        return c;
    }
    int i;
    char* b_1 = (char*)malloc(sizeof(char) * len_a);
    if (len_a > len_b)
    {
        char* compli = (char*)malloc(sizeof(char) * (len_a - len_b));
        for (i = 0; i < len_a - len_b; i++)
        {
            compli[i] = '0';
        }
        strncpy(b_1, b, len_b);
        strcat(b_1, compli);
    }
    else
    {
        strcpy(b_1, b);
    }
    char* c = (char*)malloc(sizeof(char) * (len_a - len_b + 1));
    int current_digit = 0;
    int current_pos = 0;
    int rem = len_a - len_b;
    while (rem >= 0)
    {
        while ((compare_str(a, b_1) > 0))
        {
            a = subtract(a, b_1);
            current_digit++;
        }
        c[current_pos] = current_digit + '0';
        rem--;
        current_pos++;
        current_digit = 0;
        b_1[strlen(b_1) - 1] = '\0';
    }
    c[current_pos] = '\0';
    i = 0;
    while(c[i] == '0')
        c++;
    return c;
}

char* sub_str(char* a, int len)
{
    char* c = (char*)malloc(sizeof(char) * (len + 1));
    strncpy(c, a, len);
    c[len] = '\0';
    return c;
}

char* divide_improved(char* a, char* b)
{
    if (a == NULL || b == NULL)
    {
        printf("error: no input\n");
        return NULL;
    }
    int len_a = strlen(a); int len_b = strlen(b);
    int comp = compare_str(a, b);
    if (comp < 0)
    {
        char* c = (char*)malloc(2 * sizeof(char));
        c[0] = '0'; c[1] = '\0';
        return c;
    }
    else if (comp == 0)
    {
        char* c = (char*)malloc(2 * sizeof(char));
        c[0] = '1'; c[1] = '\0';
        return c;
    }
    else
    {
        int i;
        int len_c = len_a - len_b + 1;
        char* c = (char*)malloc((len_c) * sizeof(char));
        for (i = 0; i < len_c; i++)
        {
            c[i] = '0';
        }
        c[i] = '\0';
        char* a_1 = (char*)malloc(len_a * sizeof(char));
        strcpy(a_1, a); a_1[len_a] = '\0';
        int cur_pos = 0; int cur_digit = 0;
        char* temp; int len_temp; int len_temp_remain;
        while (compare_str(a_1, b) >= 0)
        {
            temp = sub_str(a_1, len_b);
            //cur_pos += (len_b - 1);
            if (compare_str(temp, b) < 0)
            {
                temp = sub_str(a_1, len_b + 1);
                //cur_pos++;
            }
            len_temp = strlen(temp);
            while (compare_str(temp, b) >= 0)
            {
                temp = subtract(temp, b); cur_digit++;
            }
            if (temp[0] == '0')
                len_temp_remain = 0;
            else
                len_temp_remain = strlen(temp);
            for (i = 0; i < len_temp_remain; i++)
            {
                a_1[len_temp - len_temp_remain + i] = temp[i];
            }
            c[cur_pos] = cur_digit + '0'; cur_digit = 0;
            cur_pos += (len_temp - len_temp_remain);
            a_1 += (len_temp - len_temp_remain);
        }
        c[cur_pos] = '\0';
        puts(c);
        return c;
    }
}
