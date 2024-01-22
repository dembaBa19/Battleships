# README to be finished
# Battleships
## The game of battleships on c++


## The Computer Algorithm
For a classic game of battleships with the following settings :
- 10x10 grid
- 1 boat (2x1)
- 2 submarines (3x1)
- 1 destroyer (4x1)
- 1 carrier (5x1)
  
It will take you around 95/100 guesses on average if you shoot completely randomly.
That's why if we were to invent an algorithm to play the game, we would have to make it significantly better in order to have a chance against humans.
Our algorithm does it in only around 37 moves in this particular game settings and gets the upper hand against a human around 8/10 times!

The algorithm in itself is divided into 2 parts due to the amount of time and space the complex algorithm requires.
### The simple human logic algorithm
This algorithm is fairly close to human logic and is used until the computer decides the slower and algorithm can be used.
It thinks like this :
- Case 1 : If we have found no unsunk shipsquares, we try to find some central square from the grid which also has a lot of room in all 4 directions. And so our algorithm evaluates each square with the number that we get in the following way :
  
  Combined maximum distance in each of the 4 directions until we hit the end of the grid or a square that isn't free (basically anything that isn't an unknown square anymore, such as a "*" - a known nothing square) + the minimum distance to the closest such wall (this distance is added again to give central squares an edge over non-central ones in the evaluation). Here we see the algorithm work for the square [5, 3], which we have marked as "E" in this visualization and the paths in each direction with "-"s. 
  
  0  1  2  3  4  5  6  7  8  9  -------------->  0  1  2  3  4  5  6  7  8  9                                                                                     
  ----------------------------- -------------->  ------------------------------                                                                                     
0|#  #  #  #  #  #  #  #  #  # ---------------> 0|18 18 18 11 12 13 18 18 18 18                                                                                     
1|#  #  #  #  #  #  #  #  #  # ---------------> 1|18 19 19 12 13 14 19 19 19 18                                                                                     
2|#  #  #  #  #  #  #  #  #  # ---------------> 2|18 19 20 11 13 15 20 20 19 18                                                                                     
3|#  #  #  *  #  #  #  #  #  # ---------------> 3|11 12 11 0 8 10 16 16 15 14                                                                                     
4|#  #  #  |  *  #  #  #  #  # ---------------> 4|12 13 13 8 0 8 14 15 14 13                                                                                     
5|-  -  -  E  -  *  #  #  #  # ---------------> 5|13 14 15 [10]8 0 12 13 13 12                                                                                     
6|#  #  #  |  #  #  #  #  #  # ---------------> 6|18 19 20 16 14 12 21 20 19 18                                                                                     
7|#  #  #  |  #  #  #  #  #  # ---------------> 7|18 19 20 16 15 13 20 20 19 18                                                                                     
8|#  #  #  |  #  #  #  #  #  # ---------------> 8|18 19 19 15 14 13 19 19 19 18                                                                                     
9|#  #  #  |  #  #  #  #  #  # ---------------> 9|18 18 18 14 13 12 18 18 18 18                                                                                     


  On [5,3]'s search, the algorithm will hit a nothing square on [3,3] and [5,5] and walls in the left and down directions so its evaluation will be 1 (up) + 1 (right) + 3 (left) + 4 (down) + 1 (the minimum of all these) = 10
  
- Case 2 : If we have found 1 unsunk shipsquare we will apply similar logic, just to its 4 neighbours and them only!


  0  1  2  3  4  5  6  7  8  9                    0 1 2 3 4 5 6  7 8 9
  
  ------------------------------                -----------------------
  
0|#  #  #  #  #  #  #  #  #  #                  0|0 0 0 0 0 0 0  0 0 0

1|#  #  #  #  #  #  #  #  #  #                  1|0 0 0 0 0 0 0  0 0 0

2|#  #  #  #  #  #  #  #  #  #                  2|0 0 0 0 0 0 0  0 0 0

3|#  #  #  *  #  #  #  #  #  #                  3|0 0 0 0 0 0 0  0 0 0

4|#  #  #  #  *  #  #  #  #  #      ----->      4|0 0 0 0 0 0 0  0 0 0

5|#  #  #  #  #  *  #  #  #  #                  5|0 0 0 0 0 0 8  0 0 0

6|#  #  #  #  #  #  X  #  #  #                  6|0 0 0 0 0 8 0 11 0 0

7|#  #  #  #  #  #  #  #  #  #                  7|0 0 0 0 0 0 11 0 0 0

8|#  #  #  #  #  #  #  #  #  #                  8|0 0 0 0 0 0 0  0 0 0

9|#  #  #  #  #  #  #  #  #  #                  9|0 0 0 0 0 0 0  0 0 0

