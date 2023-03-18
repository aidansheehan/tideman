#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //Check name in array
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            //if candidate found set index of candidate (i) as ranked preference
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //Loop through each ranked candidate in ranks
    for (int i = 0; i < candidate_count; i++)
    {
        //find index of candidate in candidates array
        int candidate_i_index = ranks[i];

        //Loop through candidates to compare with our candidate
        for (int j = 0; j < candidate_count; j++)
        {
            //find new candidate in ranks and get index
            int j_rank;
            for (int z = 0; z < candidate_count; z++)
            {
                if (ranks[z] == j)
                {
                    j_rank = z;
                }
            }
            //if voter prefers candidate i to candidate j, update array
            if (i < j_rank)
            {
                preferences[candidate_i_index][j]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    pair_count = 0; //number of recorded pairs

    //loop through unevaluated pairs on each new i and evaluate winner and loser
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;

                pair_count++;
            }
        }

    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swap_count = 1;     //initialise swap count (unsure if should be 0?)
    int sorted = 1;     //how many pairs sorted pair_count - 1 for bubble sort ???
    do
    {
        swap_count = 0;
        //loop through unsorted pairs
        for (int i = 0; i < pair_count - sorted; i++)
        {
            pair p1 = pairs[i];     //this pair
            pair p2 = pairs[i + 1]; //next pair

            //if this pair win margin < next pair win margin swap pairs
            if (preferences[p1.winner][p1.loser] < preferences[p2.winner][p2.loser])
            {
                pairs[i] = p2;
                pairs[i + 1] = p1;

                swap_count++;       //increment swap counter if triggered
            }
        }
        sorted++;   //one pair sorted at end per round do not check next iteration
    }
    while (swap_count > 0);

    return;
}

//check if proposed lock allowed
bool lock_allowed(int loser, int winner)
{
    //check if loser is locked over winner
    if (locked[loser][winner])
    {
        return false;
    }
    //move along chain check if any already locked over winner
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (!lock_allowed(i, winner))
            {
                return false;
            }
        }
    }

    return true;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //Iterate through pairs array and lock if permitted
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        if (lock_allowed(loser, winner))
        {
            locked[winner][loser] = true;
        }
    }

    return;
}

//Check if a candidate is defeated
bool is_undefeated(int c)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][c])
        {
            return false;
        }
    }
    return true;
}

// Print the winner of the election
void print_winner(void)
{
    //find a candidate i who is locked over all other candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (is_undefeated(i))
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}