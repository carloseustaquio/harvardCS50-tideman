# harvardCS50-tideman
This is my resolution for the final problem set of Algorithms, week 3 of Harvard CS50x course.\
It was definitely not easy to finish this challenge! Some parts are so tricky! Like the recursive function lock_pairs, and some others.\
So, I decided to put my resolution here, with all my comments explaining the way that I understand each bite of the algorithm. I hope this could help some other students!\
\
Some Algorithms used: Ranked Pairs, Bubble Sort, Recursion.

## Demo:
![Tideman Demo](/tideman_test_2.gif)

## Description
Tideman is an election algorithm, that utilizes rank-choice system.
To determine the winner, it does a direct confront (1 x 1) with every possible pair of candidates, as if only them would be competing the elections. Then, by the number and size of the victories, it determines who is the most preferred candidate.

"Put more formally, the Tideman voting method consists of three parts:

* Tally *: Once all of the voters have indicated all of their preferences, determine, for each pair of candidates, who the preferred candidate is and by what margin they are preferred. \
* Sort *: Sort the pairs of candidates in decreasing order of strength of victory, where strength of victory is defined to be the number of voters who prefer the preferred candidate. \
* Lock *: Starting with the strongest pair, go through the pairs of candidates in order and “lock in” each pair to the candidate graph, so long as locking in that pair does not create a cycle in the graph. \
Once the graph is complete, the source of the graph (the one with no edges pointing towards it) is the winner!"
