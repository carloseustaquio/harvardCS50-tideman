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

        /* record vote before continue to the next voter */
        record_preferences(ranks);

        printf("\n");
    }

    /*
        DEBUG HELPER:
        Visualize the two dimentional array preferences[][]
    */

    /*
        printf("two dimentional array: \n");
        for (int i = 0; i < candidate_count; i++)
        {
            for (int j = 0; j < candidate_count; j++)
            {
                printf("%i ", preferences[i][j]);
            }
            printf("\n");
        }
    */

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    /*
        Irerates through the number of candidates.
        Finds the index of the voted candidate comparing his
        name with the candidates names, and saves his index
        to the ranks array.
        If candidate not found, returns false
    */
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            /* update de ranks array with candidate's index
                and returning true. */

            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    /*
        Creates/update the pairs. ranks[i] represents the winner
        and ranks[j] represents the loser, as j always start one
        place after i (j = i + 1).
        So, for example, if the voter rank was [2 0 1], the voter
        prefers 2 over 1 and 0, and prefers 1 over 0! It means that,
        the function should increment the pairs [2, 0] [2, 1] and
        [0, 1]
    */

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    /*
        Loops through all the preferences, and do a direct confrontation
        with each oposit pair. ex, if there's a pair [0, 1] with 3 votes,
        and [1, 0] with 5 votes, the second pair beats the first one by 2
        votes! It means that in a direct confront, the candidate with
        index 1 would beat candidate with index 0! The function then creates
        the pair winner=1 and loser=0.
        As j = i+1, we will never compare the pairs [x, x] with themselves [x, x]
        (they dont't even exist!), and we will never repeat a comparision, as
        j never gets a value below i.
        Then the function add the pair to the pairs array and increments the
        pair_count variable.
    */

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // create the pair
                pair p;
                p.winner = i;
                p.loser = j;

                // add the pair to the pair array
                pairs[pair_count] = p;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                // create the pair
                pair p;
                p.winner = j;
                p.loser = i;

                // add the pair to the pair array
                pairs[pair_count] = p;
                pair_count++;
            }
        }
    }

    /*
        DEBUG HELPER:
        Visualize all the pairs and pair_count!
    */
    /*
        printf("\n");
        for (int i = 0; i < pair_count; i++)
        {
            printf("winner: %i loser: %i\n", pairs[i].winner, pairs[i].loser);
        }
        printf("pair_count: %i\n", pair_count);
    */
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    /*
        Function does bubble sort to sort pairs in decreasing order.
        It goes back in the preferences array, and check the difference
        of votes amount between the direct opposite pairs, just like the
        add_pairs function.
        It does this process for every two adjacent pairs in the pairs array,
        then, it compares the diff between i and i+1, and if diff i < diff i+1,
        it swap them and increment the swapped counter.
        The while loop then restarts (and reset the swapped counter).
        This process is repeated until diff 1 is always greater than
        diff i+1, so the swapped counter remains 0, so the while loop
        is finalized.
    */

    pair tmp;
    int swapped;

    while (1)
    {
        swapped = 0;

        for (int i = 0; i < pair_count; i++)
        {
            // difference between the pairs (winer and loser)
            int diff_1 = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            int diff_2 = preferences[pairs[i + 1].winner][pairs[i + 1].loser] - preferences[pairs[i + 1].loser][pairs[i + 1].winner];

            if (diff_1 < diff_2)
            {
                tmp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = tmp;
                swapped = 1;
            }
        }

        if (swapped == 0)
        {
            break;
        }
    }

    /*
        DEBUG HELPER:
        shows the pairs already sorted and the winning diff of each one
    */
    /*
        for (int i = 0; i < pair_count; i++)
        {
            int diff = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            printf("%i: w = %i / l = %i / diff = %i \n", (i + 1), pairs[i].winner, pairs[i].loser, diff);
        }
    */
    return;
}

bool validate_lock(int end, int cycle_start)
{
    /*
        Really tricky recursive function here.
        The goal here is to check if the loser of one locked[][] relation is
        the winner of the (any other) next relation. This "check" is done
        recursivelly so, at some point it might be that in the end of the
        thread the "winner of the next relation" is winner over the guy in
        "cycle_start". This is what would characterize the "cycle loop".
        But, who's the guy inside "cycle_start" !?!?
        Well, he's the winner guy who started the validate check inside the lock_pairs
        function. He wants to be the winner over someone, but if "someone" wins over
        some third guy that wins over him, he cannot be the winner in that relation!
        To illustrate:
        cycle_start ----- wants to win ----> end (inside lock_pairs function)
        end == cycle_start ? (impossible)
        end wins over someone (i) ?
        if no, return.
        if yes, i becomes the end, and we go:
        end (that now has i value) == cycle_start ?
        if yes, loop found!
        if no, continue
        i wins over someone (j) ?
        if no, return.
        if yes, j becomes the end, and we go:
        [...]
        if no loop found, return false.
    */

    if (end == cycle_start)
    {
        /* return true to the upper iteration who called it */
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i])
        {
            if (validate_lock(i, cycle_start))
            {
                /* returns true to the lock_pairs function */
                return true;
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    /*
        Creates a two dimentional array similar to preferences array that
        contains the information wether one candidate is winner over the other.
        To do this, it iterates over the pairs array (that's already sorted in
        decreasing order!).
        For each pair, checks if it does'n close a cycle, and then set true
        for the pair value in the locked[pairs[].winner][pairs[].loser] position.
    */

    for (int i = 0; i < pair_count; i++)
    {
        if (!validate_lock(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    /*
        DEBUG HELPER:
        shows my two dimentional array locked[][]
    */
    /*
        printf("\n");
        printf("two dimentional locked array: \n");
        for (int i = 0; i < pair_count; i++)
        {
            for (int j = 0; j < pair_count; j++)
            {
                printf("%i ", locked[i][j]);
            }
            printf("\n");
        }
     */
    return;
}

// Print the winner of the election
void print_winner(void)
{
    /*
        This function iterates through the locked[][] array.
        One thing different is that it checks column by column
        instead of row by row (we do locked[j][i] with 'j' in the
        first brackets!). We are actually looking for the column where
        there is no true value, or, in other words, for the candidate
        that has no "arrows" pointing to him. Take a look (uncomment) the
        last helper debug block!
        For every column, we check every row, and if there is a false value
        in any locked[][] relation, we update the rank variable.
        If the rank variable reaches the candidate_count number, it means that
        we've checked the locked[][] relations between the candidate and every
        competitor, and found no arrow pointing to the candidate.
        So he is the winner.
        Prints out the candidates[index of winner candidate].
    */

    for (int i = 0; i < candidate_count; i++)
    {
        int rank = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                rank++;
            }
        }

        if (rank == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
