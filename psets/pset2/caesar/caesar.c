#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // validate the user args provided
    if (argc != 2)
    {
        // prompt with correct usage
        printf("Usage: ./caesar key\n");
        // exit with return status 1
        return 1;
    }

    // validate the arg passed in integer
    int key = 0;
    if (strspn(argv[1], "0123456789") == strlen(argv[1]))
    {
        //converting string to int
        key = atoi(argv[1]);
    }
    else
    {
        // invalid arg -- not an integer
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // adjust key to [0, 25] range
    key %= 26;
    //printf("key = %i\n",key);

    //Getting user input plaintext
    string text = get_string("plaintext: ");

    //Start printing ciphertext
    printf("ciphertext: ");

    // starting the cipher process
    // parsing through the input text and printing the ciphered
    for (int i  = 0; i < strlen(text); i++)
    {
        // for upper case letters
        if (isupper(text[i]))
        {
            text[i] = (text[i] - 'A' + key) % 26 + 'A';
        }
        // for lower case letters
        else if (islower(text[i]))
        {
            text[i] = (text[i] - 'a' + key) % 26 + 'a';
        }
        // for other cahrs

        // printing
        printf("%c", text[i]);
    }
    printf("\n");
}