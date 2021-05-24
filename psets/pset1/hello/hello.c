#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // prompt for the user input
    string username = get_string("What is your name?\n");

    // printing to the output buffer
    printf("hello, %s\n", username);
}
