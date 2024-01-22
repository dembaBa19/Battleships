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
- If we have found no unsunk ships, we try to find some central square from the grid which also has a lot of room in all 4 directions. And so our algorithm evaluates each square with the number that we get in the following way :
  -- combined maximum distance in each of the 4 directions until we hit the end of the grid or a square that isn't free (basically anything that isn't an unknown square anymore) + the minimum distance to the closest such wall (this distance is added again to give central squares an edge over non-central ones)
  
   
