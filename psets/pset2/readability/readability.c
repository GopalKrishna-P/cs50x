
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int ColemanLiau_Index(float w, float l, float s);

int main(void)
{
    // getting the user input
    string Text = get_string("Text: ");

    // initializing variables for furthur use
    int letters = 0, words = 1, sentences = 0;

    // parsing through input
    for (int i = 0, n = strlen(Text); i < n; i++)
    {
        //calculating no of letters
        if (isupper(Text[i]) || islower(Text[i]))
        {
            letters++;
        }

        //calculating no of words
        if (Text[i] == 32)
        {
            words++;
        }


        //calculating no of sentences
        if (Text[i] == 33 || Text[i] == 46 || Text[i] == 63)
        {
            sentences++;
        }

    }

    //calculating the Coleman-Liau index
    int index = ColemanLiau_Index(letters, words, sentences);

    //debugging
    //printf("letters = %i, words = %i, sentence = %i, grade = %i\n", letters, words, sentences, index);

    //Printing the Grade
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }


}


int ColemanLiau_Index(float l, float w, float s)
{
    // ToDo
    // calculate and return the Coleman-Liau Index
    float L = l * 100.0 / w;
    float S = s * 100.0 / w;

    //debug
    //printf("L = %f, S = %f \n", L, S);

    //return the value calc based on formula
    return round(0.0588 * L - 0.296 * S - 15.8);
}