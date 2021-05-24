
// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// Creating 26 bucket for each alphabet
const unsigned int N = 26;

// Hash table
// declare & initialize Global variables for further use
node *table[N];
unsigned int word_counter = 0;


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    // calc the index through hash value
    unsigned int hash_index = hash(word);

    // look-up the index and initiale a point to that position
    node *ptr = table[hash_index];

    //traverse the Linked List to search for the word
    while (ptr != NULL)
    {
        // case-insensitive compare
        if (strcasecmp(ptr->word, word) == 0)
        {
            // if found the word return true
            return true;
        }
        // jump to next node
        ptr = ptr->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // open file
    FILE *file = fopen(dictionary, "r");

    // allocate memory to a word and valiadate the allocation
    char *word = malloc(LENGTH);
    if (word == NULL)
    {
        return false;
    }

    // read through the file word-by-word untill EndOfFile
    while (fscanf(file, "%s", word) != EOF)
    {
        // create node for insertion and validatet the allocation
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // copy the word and update the counter
        strcpy(n->word, word);
        word_counter++;

        n->next = table[hash(word)];
        table[hash(word)] = n;
    }

    fclose(file);
    free(word);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    // initiate tmp node for further use
    node *tmp1 = NULL, *tmp2 = NULL;

    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }

        tmp1 = table[i];
        tmp2 = tmp1;

        while (tmp1->next != NULL)
        {
            tmp1 = tmp1->next;
            free(tmp2);
            tmp2 = tmp1;
        }

        free(tmp1);
    }

    return true;
}