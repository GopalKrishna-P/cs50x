#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
void printwinnerindex(int win_index);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO

    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name,  candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO

    // initialize a temp vaiable to store the max votes
    int tmp_largest = 0, win_index = -1;

    // look for the leader with most votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > tmp_largest)
        {
            tmp_largest = candidates[i].votes;
            win_index = i + 1;
        }
        else if (candidates[i].votes == tmp_largest)
        {
            win_index = 10 * win_index + i + 1;
            //printf("collision\n");
        }
        //printf("%s -- %i, index = %i\n", candidates[i].name, candidates[i].votes, win_index);
    }

    //print from the index given
    //printf("%i\n", win_index);
    printwinnerindex(win_index);

    return;
}

void printwinnerindex(int win_index)
{
    // ToDo use recursion to print the winner

    //debug
    //printf("called for index = %i \n", win_index);
    if (win_index > 9)
    {
        printwinnerindex(win_index / 10);
        printf("%s\n", candidates[win_index % 10 - 1].name);
    }
    else
    {
        printf("%s\n", candidates[win_index - 1].name);
    }

    //while (win_index > 0)
    //{
    //    printf("%s\n", candidates[win_index % 10 - 1].name);
    //    win_index /= 10;
    //}
}

