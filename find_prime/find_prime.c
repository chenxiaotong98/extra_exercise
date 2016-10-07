/*
This program finds the least 1000000 primes.
Algorithm:
Using primes less than the square root of the object number to divide it.
*/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define DESIRED_NUM 10000000
// approximate number of primes: x/ln(x)

int main()
{
    int* primes = (int*)malloc(DESIRED_NUM * sizeof(int));
    primes[0] = 2; primes[1] = 3; primes[2] = 5; primes[3] = 7;
    int count = 4;
    int add_flag = 0;
    int is_prime = 1;
    int current_num = 11;
    double current_num_sqrt;
    int i;
    while (count < DESIRED_NUM)
    {
        current_num_sqrt = sqrt((double)current_num);
        for (i = 0; (current_num_sqrt >= (double)primes[i]) && (i < count); i++)
        {
            if ((current_num % primes[i]) == 0)
            {
                is_prime = 0;
                break;
            }
        }
        if (is_prime == 1)
        {
            primes[count] = current_num;
            count++;
        }
        else
        {
            is_prime = 1;
        }
        if (add_flag == 0)
        {
            current_num += 2;
            add_flag = 1;
        }
        else
        {
            current_num += 4;
            add_flag = 0;
        }
    }
    printf("Have found all %d primes! \n", DESIRED_NUM);
    return 0;
}
