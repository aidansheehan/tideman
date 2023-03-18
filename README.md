## Setup
You will need cs50.h installed to run this program. 
* https://cs50.readthedocs.io/libraries/cs50/c/

## Compilation
Run 
````
make tideman
````
to compile the program.

## Usage

This C program runs a Tideman election and is my implementation of the pset3 task from cs50 2021.
* https://cs50.harvard.edu/x/2021/psets/3/tideman/

Enter the names of the candidates as command line input. For example, for an election between 3 candidates, Alice, Bob and Charlie, run:
````
./tideman Alice Bob Charlie
````

You will be prompted for the number of voters, and must then input each voters ranked preferences. The program calculates and outputs the winner of the election.