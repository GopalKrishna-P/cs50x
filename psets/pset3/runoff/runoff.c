#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO

    // iterating to find the candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // name based look-up
        if (strcmp(name,  candidates[i].name) == 0)
        {
            // recording the vote based on the input
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    for (int j = 0; j < voter_count; j++)
    {
        int i = 0;
        // for every voter loops until I find a candidate that it is still in the race
        while (candidates[preferences[j][i]].eliminated && i++ < 3) {}

        // then adds one vote for the candidate of next prioprity
        candidates[preferences[j][i]].votes++;
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO

    // iterating through the candidates to find the winner
    for (int j = 0; j < candidate_count; j++)
    {
        // if votes of the candidate exceed the 50% mark print the winner
        if (candidates[j].votes > voter_count / 2)
        {
            printf("%s\n", candidates[j].name);
            return true;
        }
    }
    // winner isn't found
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO

    // initializing the min no of votes to the max possible
    int minimumVotes = voter_count;

    // iterating through candidates to find the min
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            // whenever found less reset the min value to current
            if (candidates[i].votes < minimumVotes)
            {
                minimumVotes = candidates[i].votes;
            }
        }
    }
    // return the value found
    return minimumVotes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO

    // calculating the winning votes (Max) required
    int win = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > win)
        {
            win = candidates[i].votes;
        }
    }

    // logic if all people have same no of votes to win -> tie
    int winner = 0, remain = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            // no of. still in contest
            remain++;
            if (candidates[i].votes == win)
            {
                // no of candidates who have winning votes
                winner++;
            }
        }
    }

    // return true if all have the same no of votes
    return (winner == remain)? true : false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
