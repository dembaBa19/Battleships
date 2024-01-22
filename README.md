# The game of battleships on c++

## Rules
### Intro
1)Welcome to battleships! Here are the rules of the game :

Players each start by placing a few ships on a NxN grid.
When we come to battle phase players take turns trying to guess where the enemy ships are.
The input they give is coordinates [x,y] and only get an answer of whether it is a hit or miss
and when a ship is completely destroyed, a message is displayed to confirm its squares.
Game goes on until either player sinks all of their opponent's ships.

## Symbols (Characters) used in README and code
'S' - Ship                                                                                                                                                            
'#' - Unknown Square of Grid                                                                                                                                          
'X' - Known Square of Ship of Grid                                                                                                                                    
'O' - Known Sunk Square of Ship in Grid                                                                                                                               
'*' - Known Nothing Square in Grid                                                                                                                                    
N - size of grid

### Placing ships
3) The battlefield will be displayed in the following way during the phase of placing ships before each placement of a ship :
   0  1  2  3  4  5  6  7  8  9
 ------------------------------
 0|S  *  *  *  *  *  *  *  *  *
 1|S  *  *  *  *  *  *  *  S  *
 2|*  *  *  *  *  *  *  *  S  *
 3|*  *  *  *  *  *  *  *  S  *
 4|*  *  *  *  *  S  S  *  *  *
 5|*  S  S  *  *  *  *  *  *  *
 6|*  *  *  *  *  *  *  *  *  *
 7|*  *  S  S  S  *  *  *  *  *
 8|*  *  *  *  *  *  *  *  *  *
 9|*  *  *  *  *  *  *  *  *  *

Placing ships works by entering 2 coordinates [x,y] and 1 character [H/V] for whether the ship will have a horizontal or vertical orientation.
For instance the ship in the top left was obviously entered with [0 0 V] in the phase of placing the 2x1 ships

### Shooting
4) After all ships have been placed we reach the phase of shooting where the 2 players take turns to make guesses
The user will first get the known ships of the opponent that will look something like :

  0  1  2  3  4  5  6  7  8  9
 ------------------------------
0|O  *  #  #  #  #  *  #  #  *
1|O  #  *  #  #  #  #  *  O  #
2|#  O  O  O  O  O  #  #  O  #
3|#  #  #  *  #  #  #  #  O  #
4|X  X  X  #  *  #  #  #  #  *
5|*  O  O  #  #  *  #  #  #  #
6|#  *  *  #  #  #  *  #  #  #
7|#  #  O  O  O  #  #  *  #  #
8|#  *  #  #  #  *  #  #  *  #
9|*  #  #  *  O  O  O  O  #  *
Taking a few seconds for user to read...

A legal shoot here would be [4 3] since it is free (unknown for now), [0 0] would be not since it's already a square that has been targeted in the past.
If a shot is successful, there is a chance it completes a whole sunk ship in which case the game will print a message like :
[4, 0] -> [4, 3] has sunk!
To avoid any confusion a list of the sunk ships will always be provided at each move before the player's guess like this :
Sunk ships :
[0, 0] -> [1, 0]
[5, 1] -> [5, 2]
...
[2, 1] -> [2, 5]

### End of game
5)
At the end of the game a message will be displayed to say who won in how many moves and how far the other player was from winning.
Further instructions are provided throughout the game. Enjoy playing!

## The Computer Algorithm
### Placing battleships
The battleships are placed completely randomly by the computer.

### Shooting
For a classic game of battleships with the following settings :
- 10x10 grid
- 1 boat (2x1)
- 2 submarines (3x1)
- 1 destroyer (4x1)
- 1 carrier (5x1)
  
It will take you around 95/100 guesses on average if you shoot completely randomly.
That's why if we were to invent an algorithm to play the game, we would have to make it significantly better in order to have a chance against humans.
Our algorithm does it in only around 42 moves in these particular game settings and gets the upper hand against a human around 8/10 times!

The algorithm in itself is divided into 2 parts due to the amount of time and space the complex algorithm requires. On each step we will calculate the steps the complex algorithm would take at most by the following formula - (NxNx2)^(battleships left unsunk). If the number we get is above 10M steps, the algorithm will be deemed too slow to be used and we will proceed with the simpler one.

#### The simple human logic algorithm
This algorithm is fairly close to human logic and is used until the computer decides the slower and space consuming algorithm can be used.
It thinks like this :
- Case 1 : If we have found no unsunk shipsquares, we try to find some central square from the grid which also has a lot of room in all 4 directions. And so our algorithm evaluates each square with the number that we get in the following way :
  
  Combined maximum distance in each of the 4 directions until we hit the end of the grid or a square that isn't free (basically anything that isn't an unknown square anymore, such as a "*" - a known nothing square) + the minimum distance to the closest such wall (this distance is added again to give central squares an edge over non-central ones in the evaluation). Here we see the algorithm work for square [5, 3], which we have marked as "E" in this visualization and the paths in each direction with "-"s. PLEASE USE RAW VERSION OF THE README FOR THIS VISUALIZATION :
  
  0  1  2  3  4  5  6  7  8  9  -------------->    0  1  2  3  4  5  6  7  8  9                                                                                       
  ----------------------------- -------------->  ------------------------------                                                                                     
0|#  #  #  #  #  #  #  #  #  # ---------------> 0|18 18 18 11 12 13 18 18 18 18                                                                                     
1|#  #  #  #  #  #  #  #  #  # ---------------> 1|18 19 19 12 13 14 19 19 19 18                                                                                     
2|#  #  #  #  #  #  #  #  #  # ---------------> 2|18 19 20 11 13 15 20 20 19 18                                                                                     
3|#  #  #  *  #  #  #  #  #  # ---------------> 3|11 12 11 0  8  10 16 16 15 14                                                                                       
4|#  #  #  |  *  #  #  #  #  # ---------------> 4|12 13 13 8  0  8  14 15 14 13                                                                                       
5|-  -  -  E  -  *  #  #  #  # ---------------> 5|13 14 15[10]8  0  12 13 13 12                                                                                      
6|#  #  #  |  #  #  #  #  #  # ---------------> 6|18 19 20 16 14 12 21 20 19 18                                                                                     
7|#  #  #  |  #  #  #  #  #  # ---------------> 7|18 19 20 16 15 13 20 20 19 18                                                                                     
8|#  #  #  |  #  #  #  #  #  # ---------------> 8|18 19 19 15 14 13 19 19 19 18                                                                                     
9|#  #  #  |  #  #  #  #  #  # ---------------> 9|18 18 18 14 13 12 18 18 18 18                                                                                     

  On [5,3]'s search, the algorithm will hit a nothing square on [3,3] and [5,5] and walls in the left and down directions so its evaluation will be 1 (up) + 1 (right) + 3 (left) + 4 (down) + 1 (the minimum of all these) = 10
  
- Case 2 : If we have found 1 unsunk shipsquare we will apply similar logic, just to its 4 neighbours and them only, the rest will be valued at 0, because our only aim for now is to get the whole ship!  PLEASE USE RAW VERSION OF THE README FOR THIS VISUALIZATION :

  0  1  2  3  4  5  6  7  8  9  -------------->   0  1  2  3  4  5  6  7  8  9                                                                                        
  ----------------------------- --------------> ------------------------------                                                                                        
0|#  #  #  #  #  #  #  #  #  # ---------------> 0|0  0  0  0  0  0  0  0  0  0                                                                                        
1|#  #  #  #  #  #  #  #  #  # ---------------> 1|0  0  0  0  0  0  0  0  0  0                                                                                        
2|#  #  #  #  #  #  #  #  #  # ---------------> 2|0  0  0  0  0  0  0  0  0  0                                                                                        
3|#  #  #  *  #  #  #  #  #  # ---------------> 3|0  0  0  0  0  0  0  0  0  0                                                                                        
4|#  #  #  #  *  #  #  #  #  # ---------------> 4|0  0  0  0  0  0  0  0  0  0                                                                                        
5|#  #  #  #  #  *  #  #  #  # ---------------> 5|0  0  0  0  0  0  8  0  0  0                                                                                        
6|#  #  #  #  #  # [X] #  #  # ---------------> 6|0  0  0  0  0  8 [0] 11 0  0                                                                                        
7|#  #  #  #  #  #  #  #  #  # ---------------> 7|0  0  0  0  0  0  11 0  0  0                                                                                        
8|#  #  #  #  #  #  #  #  #  # ---------------> 8|0  0  0  0  0  0  0  0  0  0                                                                                        
9|#  #  #  #  #  #  #  #  #  # ---------------> 9|0  0  0  0  0  0  0  0  0  0                                                                                         

- Case 3 : If we have found 2 or more unsunk shipsquare we will again apply similar logic, but only to the edges of the found shipsquares in whichever direction this ship seems to be going. In the following case we have found 2 shipsquares going vertically. PLEASE USE RAW VERSION OF THE README FOR THIS VISUALIZATION :

  0  1  2  3  4  5  6  7  8  9  -------------->   0  1  2  3  4  5  6  7  8  9                                                                                        
  ----------------------------- --------------> ------------------------------                                                                                        
0|#  #  #  #  #  #  #  #  #  # ---------------> 0|0  0  0  0  0  0  0  0  0  0                                                                                        
1|#  #  #  #  #  #  #  #  #  # ---------------> 1|0  0  0  0  0  0  0  0  0  0                                                                                        
2|#  #  #  #  #  #  #  #  #  # ---------------> 2|0  0  0  0  0  0  0  0  0  0                                                                                        
3|#  #  #  *  #  #  #  #  #  # ---------------> 3|0  0  0  0  0  0  0  0  0  0                                                                                        
4|#  #  #  #  *  #  #  #  #  # ---------------> 4|0  0  0  0  0  0  0  0  0  0                                                                                        
5|#  #  #  #  #  *  #  #  #  # ---------------> 5|0  0  0  0  0  0  6  0  0  0                                                                                        
6|#  #  #  #  #  # [X] #  #  # ---------------> 6|0  0  0  0  0  0 [0] 0  0  0                                                                                        
7|#  #  #  #  #  # [X] #  #  # ---------------> 7|0  0  0  0  0  0 [0] 0  0  0                                                                                        
8|#  #  #  #  #  #  #  #  #  # ---------------> 8|0  0  0  0  0  0  2  0  0  0                                                                                        
9|#  #  #  #  #  #  #  #  #  # ---------------> 9|0  0  0  0  0  0  0  0  0  0                                                                                                
However it could happen that none of the neighbours are a continuation of this ship which would look something like this :

[* X X *], in which case we give up looking for a continuation, since the ship hasn't sunk and it has no room to grow in that direction. In that case we conclude it's parts of different ships and just use the method for Case 2 until we sink something.

#### The complex brute force computer algorithm
After we have sunk enough ships, we get to the more complex when it comes to time and memory solution. The logic behind it is very simple and uses a brute force algorithm. We will take the known field and try to compare it with EACH POSSIBLE different legal way to place the ships on a NxN grid and see if it matches (meaning all the ships from a given initial variation matches the current known field). When we find such a legal variation we add 1 to value of the final matrix only to the squares in which we have placed a new ship and 0 to the rest in order to get something of a probability for each square as shown. PLEASE USE RAW VERSION OF THE README FOR THIS VISUALIZATION :

  0  1  2  3  4  5  6  7  8  9  -------------->   0  1   2   3   4   5   6  7  8  9                                                                                   
  ----------------------------- --------------> -----------------------------------                                                                                   
0|O  O  #  #  #  #  #  #  #  # ---------------> 0|0  0   5   13  18  23  20 20 15 10                                                                                  
1|#  *  #  #  #  #  #  #  #  # ---------------> 1|5  0   5   13  18  23  20 23 20 15                                                                                  
2|#  #  *  #  #  #  #  #  #  # ---------------> 2|9  1   0   8   13  18  18 23 23 20                                                                                  
3|#  #  #  #  #  #  *  #  #  # ---------------> 3|18 11  13  16  13  8   0  8  13 20                                                                                  
4|#  #  *  O  O  O  #  #  #  # ---------------> 4|17 1   0   0   0   0   3  8  13 25                                                                                  
5|#  #  X  X  X  #  #  *  #  # ---------------> 5|70 148 242 242 242 152 56 0  5  22  (Those known ships exist in all variations obviously - 242)                     
6|#  O  O  O  *  #  #  #  *  # ---------------> 6|17 0   0   0   0   5   0  3  0  20                                                                                  
7|#  #  *  #  #  #  *  #  #  # ---------------> 7|13 0   0   0   0   5   0  3  0  15                                                                                  
8|#  #  #  #  #  #  #  #  #  # ---------------> 8|14 10  15  20  22  27  20 18 10 15                                                                                  
9|#  #  #  #  #  #  #  #  #  # ---------------> 9|10 10  15  20  22  26  20 18 10 10     

The highest value is for [5,5] - 152. Thus [5, 5] will be the answer.
