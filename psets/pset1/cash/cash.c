#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // getting the input
    float dollar = 0;
    do
    {
        dollar = get_float("Change owed: ");
    }
    while (dollar <= 0);

    // finding the cents from the given user input
    int cents = round(dollar * 100);

    // intializing variable to store the count of coins
    int count = 0;

    // no of 25c coins
    if (cents >= 25)
    {
        count += cents / 25;
        cents %= 25;
    }

    // no of 10c coins
    if (cents >= 10)
    {
        count += cents / 10;
        cents %= 10;
    }

    // no of 5c coins
    if (cents >= 5)
    {
        count += cents / 5;
        cents %= 5;
    }

    // no of 1c coins
    if (cents >= 1)
    {
        count += cents;
    }

    // display the count to the user
    printf("%i\n", count);
}