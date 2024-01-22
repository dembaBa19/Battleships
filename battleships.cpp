/**
*
* Solution to course project #1 (Battleships)
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Krum Borisov Angelov
* @idnumber 0MI0600330
* @compiler GCC
*
* Game of Battleships
*
*/

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<random>
#include<windows.h>

using namespace std;

//default characters
char const symbolForShip = 'S';
char symbolForKnownShip = 'X';
char symbolForSunkShip = 'O';
char symbolForHidden = '#';
char symbolForNothing = '*';

//default settings
int defaultBoats = 2;
int defaultSubmarines = 2;
int defaultDestroyers = 1;
int defaultCarriers = 1;
int defaultGridSize = 7;
int minGridSize = 5;
int maxGridSize = 10;

string computerName1 = "HAL 9000";
string computerName2 = "Computer";

enum Direction {
    Horizontal,
    Vertical
};

enum ShipType {
    Boat,
    Submarine,
    Destroyer,
    Carrier
};

//variable to keep ship info
struct Ship {
    ShipType shipType;
    Direction direction;
    int x, y;
    int length;
    bool sunk = false;
};

//variable type player for each player
struct Player {
    string name;
    bool isHuman;
    char** battlefield; //battlefield for the placement of that player's ships
    bool** revealedByOpponent; //true or false for whether that square has been targeted and is thus revealed
    Ship* ships;
    int remainingShipsOfKind[4] = {0, 0, 0, 0};
    int remainingShipsTotalCount;
    int remainingShipSquares;
};

//variable for game settings
struct Game {
    int battlefieldSize;
    int boatsCount, submarinesCount, destroyersCount, carriersCount;
    int shipsCount;
    Player* player1;
    Player* player2;
    bool defaultSettings = false;
    int moveCount = 1;
    bool regularMoveOrder = true;
    bool slowMode = false;
};

Game game;
Player player1, player2;

void addPlayersToGame() {
    game.player1 = &player1;
    game.player2 = &player2;
}

void walls(int i) {
    while(i > 0) {
        cout<<"----------------------------------------------------------------------------------------------------------------------------------------------\n";
        i--;
    }
}

void lookAway() {
    walls(40);
    cout<<"\n";
}

int validCheckDigit(char input) {
    if(input>= '0' && input <= '9') return (input - '0');
    else return -666;
}

//find the other player
Player* otherPlayerFinder(Player* player) {
    Player* otherPlayer;
    if(player->name == game.player1->name) otherPlayer = game.player2;
    else otherPlayer = game.player1;
    return otherPlayer;
}

//finding which ship we hit from the coordinates
Ship* findShipByCoordinates(Player* player, int x, int y) {
    for(int i = 0; i < game.shipsCount; i++) {
        if(player->ships[i].direction == Horizontal) {
            if(x == player->ships[i].x && y >= player->ships[i].y && y <= player->ships[i].y + player->ships[i].length - 1) {
                return &(player->ships[i]);
            }
        } else {
            if(y == player->ships[i].y && x >= player->ships[i].x && x <= player->ships[i].x + player->ships[i].length - 1) {
                return &(player->ships[i]);
            }
        }
    }
    cout <<"NO SHIP FOUND TO MATCH COORDINATES " << x << ", " << y <<"\n\n";
    return &(player->ships[0]);
}

//for printing battlefield
void printBattlefield(Player* player, bool showShips) {
    //bool showShips for whether we want to show the placement of the ships or just the opponent's attempts and hide the rest
    if(showShips) cout << "Battlefield for player " << player->name << " so far :\n\n";
    else cout << "Known battlefield for " << player->name << " so far :\n\n";

    cout << "  ";
    if(game.battlefieldSize>10) cout << " ";
    for(int i = 0; i < game.battlefieldSize; i++) {
        cout << i;
        if(i<=8) cout << " ";
        cout << " ";
    }

    cout << "\n  ";
    if(game.battlefieldSize>10) cout << " ";
    for(int i = 0; i < game.battlefieldSize; i++) {
        cout << "---";
    }
    cout << "\n";

    for(int i = 0; i < game.battlefieldSize; i++) {
        if(game.battlefieldSize>10 && i<10) cout << " ";
        cout<<i<<"|";
        for(int j = 0; j < game.battlefieldSize; j++) {
            if(showShips) {
                cout << player->battlefield[i][j];
            } else {
                if(player->revealedByOpponent[i][j]) {
                    if(player->battlefield[i][j] == symbolForShip) {
                        Ship* ship = findShipByCoordinates(player, i, j);
                        if(ship->sunk) {
                            cout << symbolForSunkShip;
                        } else {
                            cout << symbolForKnownShip;
                        }
                    }
                    else cout << player->battlefield[i][j];
                } else {
                    cout << symbolForHidden;
                }
            }
            cout << "  ";
        }
        cout<<"\n";
    }
    cout<<"\n";
}

void printCharMatrix(char** matrix) {
    cout << "  ";
    if(game.battlefieldSize>10) cout << " ";
    for(int i = 0; i < game.battlefieldSize; i++) {
        cout << i;
        if(i<=8) cout << " ";
        cout << " ";
    }

    cout << "\n  ";
    if(game.battlefieldSize>10) cout << " ";
    for(int i = 0; i < game.battlefieldSize; i++) {
        cout << "---";
    }
    cout << "\n";

    for(int i = 0; i < game.battlefieldSize; i++) {
        if(game.battlefieldSize>10 && i<10) cout << " ";
        cout<<i<<"|";
        for(int j = 0; j < game.battlefieldSize; j++) {
            cout << matrix[i][j] << " ";
        }
        cout<<"\n";
    }
    cout<<"\n";
}

void printIntMatrix(int** matrix) {

    cout << "  ";
    if(game.battlefieldSize>10) cout << " ";
    for(int i = 0; i < game.battlefieldSize; i++) {
        cout << i;
        if(i<=8) cout << " ";
        cout << " ";
    }

    cout << "\n  ";
    if(game.battlefieldSize>10) cout << " ";
    for(int i = 0; i < game.battlefieldSize; i++) {
        cout << "---";
    }
    cout << "\n";

    for(int i = 0; i < game.battlefieldSize; i++) {
        if(game.battlefieldSize>10 && i<10) cout << " ";
        cout<<i<<"|";
        for(int j = 0; j < game.battlefieldSize; j++) {
            cout << matrix[i][j] << " ";
        }
        cout<<"\n";
    }
    cout<<"\n";
}

void printLongLongMatrix(long long** matrix) {

    cout << "  ";
    if(game.battlefieldSize>10) cout << " ";
    for(int i = 0; i < game.battlefieldSize; i++) {
        cout << i;
        if(i<=8) cout << " ";
        cout << " ";
    }

    cout << "\n  ";
    if(game.battlefieldSize>10) cout << " ";
    for(int i = 0; i < game.battlefieldSize; i++) {
        cout << "---";
    }
    cout << "\n";

    for(int i = 0; i < game.battlefieldSize; i++) {
        if(game.battlefieldSize>10 && i<10) cout << " ";
        cout<<i<<"|";
        for(int j = 0; j < game.battlefieldSize; j++) {
            cout << matrix[i][j] << " ";
        }
        cout<<"\n";
    }
    cout<<"\n";
}

void setHumanName(Player* player) {

    cout<<"What name would you like to use?\n";
    string name;
    cin>>name;

    player->name = name;

    cout << "Thank you, " << name << "!\n\n";
}

//intro
void pasteIntro() {
    cout<<
    "............................................................................................................................\n"
    "............................................................................................................................\n"
    "....::::::::::::::::::...::::..:-:..:::.:::.::::::......:=++===+#+----:...............:+****-..-*+:-*****++****-+**-.-**+*=.\n"
    "...:*@##%+#--@%**++%@=.:*%++#+*%=%%-=%%+*@%:*@%=*+.....:%#*%#%*#@##%#%#%=..=-..........*@%*@#:-*%@=-=#@%-+=*@*+-*@%:.-%@++=.\n"
    "....-%@@@%+.:@%%*:=%@-.-@%=.:*@%-%@#-%@%%@#:+@%**.......+%##+#-*%=.%=%##-.:**-:........*@%*@*:=##@*:.#@%:..+@*:.*@%::=%@**-.\n"
    "....:+@#%%-.:@%+=+=%@-+*%%=:=+%%-%@=-##**@#:+@%:+=.......:=*%%%%@%#%#+*##%%%@#%%#*=...:#%%#%*=#=+%%-:#%%-.:*%#-.*%%*#*%%*#*.\n"
    ".....:-:--..:==-=--==-=-:-=-:.:--=:.-----==:-==-=-.........::-*@@@-:.:####*%@-%-#*%+........................................\n"
    "......................................................-+##%%%*#@@@%%##*++#%@@%%%%*-.........................................\n"
    "......................................................-+#%#%%#%@@@=-%*#@%#%@%=.........-**+=:++++++:=++=-++++:.-**+=........\n"
    "..................=*%%##++#**#-.................-+:..+:+#::--*%@@@@#+--::-#@@#:.......-%%+=*:#@*+@#:=@%--%%#%%*%%==+........\n"
    "..................::#@#:=@%+*@*:................-@%=:*+:@%*-*#@@@@%=#%%%%%@@@@@@%%+:..:+%@@#:#@#*@#:=@%--%%##=:*%@@*........\n"
    "....................#@#.:%%+*@+:................-@@%#%%%@@%--*@@@@@:..::=#@@@@=::..::.=%==@*:#@**@#:+@%--%%=..=%-=%+........\n"
    "...................:+**:.:=*+-........:+%%%*-:.:+@%*#@%*@@%-.=%@@@@#+===+%@@@@:...=%@%#-::...::::::.:::..::....:::..........\n"
    "....-##+-:........................-%%%%#=:*%@@%%@#-:*@@+@%:.:+@@@@@-:+@@@@@@@@-..:%@@@*:....................................\n"
    "..:#%*-:+%@%%%+:....................:=%@@%*::@@@@*::*@@%:...=%@@@@@%*##@@@@@@@#.:#@@@%+.....................................\n"
    "..:*@@@@%@%%=-+%%%%%*-:...:-:::........:+%@@%@@%%@%#%@@@%%%%@@@@@@@@@@@#*%@%@@@:*@@@@@=.....................................\n"
    "...:#@@%--#@@@@@@%*-#%*@%@@@@@@@%+...-####%@@@@*..=%@@@@@@@@@@@@@@@@@@@*.....*%@@@@@@@=.....................................\n"
    "....-%@@#:.....:-*%@@@@@@@@@@@@@@@*:=%@@%%@@@@@@@@@@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%@@@%%%%#:.............................\n"
    ".....-%@@*:...........:-*%%%@@@@@@@@@@%+.:#@@%...=@@=..:@@@%*::...%@@@@@@@@=::-=*#%%%#++==+@@%+:::..........................\n"
    "......=%@@=..................::-*%%%%%@@%%@@@@#:-#@@*::=@@@@@@@%*-%@@@@@@@@@%%%%%%%%%@@@@@@::-%@@#-...........:%:...........\n"
    ".......=%@@:..........................::-+#%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@=::......:::+%@@@@@@%%@%%:........+%*...........\n"
    "........=%@%:....................................::-=+#%%%%%%%@@@@@@@@@@@@@@@@@@%%%%%%%%%@@@@@@%=+%@@=..:::::=%@@:..........\n"
    ".........=%@#:.................................................:::-==**####################%@@@#--#@@*.=@@@@@@@@@+..........\n"
    "..........=@%*.............................................................................:%@@%##%@@@#%@@@@@@@@@%+-=----:..\n"
    "...........=%%+.............................................................................=#######%%%%%%%%%#########%@*:..\n"
    "............=%%+::::::::::::::::::::::::::::........................................................................:+%#-...\n"
    ".............=%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%%%%%%%%%%#######************+++++==============--------=#@*-....\n"
    "..............=%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%*:.....\n"
    "................:::::::::::::::::::--------------------------------------------------------====------===============-:......\n"
    "............................................................................................................................\n\n";

    cout << "\n" << "Welcome to battleships!\n";
}

void rules() {
    char yesOrNo;

    cout << "Would you like default settings? (enter Y/N) They include :\n- skip rules\n"
    "- " << defaultGridSize << "x" << defaultGridSize <<" grid\n"
    "- " << defaultBoats << " boat(s), " << defaultSubmarines << " submarine(s), " << defaultDestroyers << " destroyer(s), " << defaultCarriers <<" carrier(s)\n";

    cin >> yesOrNo;

    while(yesOrNo!='Y' && yesOrNo!='y' && yesOrNo!='N' && yesOrNo!='n') {
        cout << "Please enter Y for default settings or N for choosing them\n";
        cin >> yesOrNo;
    }

    if(yesOrNo=='Y' || yesOrNo=='y') {
        game.defaultSettings = true;
        return ;
    }

    cout << "\nWould you like to read the rules? (enter Y/N)\n";
    cin >> yesOrNo;

    while(yesOrNo!='Y' && yesOrNo!='y' && yesOrNo!='N' && yesOrNo!='n') {
        cout << "Please enter Y for rules or N for skipping them\n";
        cin >> yesOrNo;
    }

    if(yesOrNo=='Y' || yesOrNo=='y') {
        cout << "\n1)Welcome to battleships! Here are the rules of the game :\n\n"
        << "Players each start by placing a few ships on a NxN grid.\n"
        << "When we come to battle phase players take turns trying to guess where the enemy ships are.\n"
        << "The input they give is coordinates [x,y] and only get an answer of whether it is a hit or miss\n"
        << "and when a ship is completely destroyed, a message is displayed to confirm its squares.\n"
        << "Game goes on until either player sinks all of their opponent's ships.\n";
        if(game.slowMode) cout << "Taking a few seconds for user to read...\n\n";
        if(game.slowMode) Sleep(20000);
        walls(2);
        cout << "2)Here are some symbols that will be used throughout the rules.\n"
        << "Unless changed, they will be the same ones for the whole game :\n\n"
        << "S - Ship in Grid\n"
        << "# - Unknown Square of Grid\n"
        << "X - Known Square of Ship of Grid\n"
        << "O - Known Sunk Square of Ship in Grid\n"
        << "* - Known Nothing Square in Grid\n";
        if(game.slowMode) cout << "Taking a few seconds for user to read...\n\n";
        if(game.slowMode) Sleep(15000);
        walls(2);
        cout << "3)The battlefield will be displayed in the following way during the phase of placing ships before each placement of a ship :\n"
        << "   0  1  2  3  4  5  6  7  8  9\n"
        << " ------------------------------\n"
        << " 0|S  *  *  *  *  *  *  *  *  *\n"
        << " 1|S  *  *  *  *  *  *  *  S  *\n"
        << " 2|*  *  *  *  *  *  *  *  S  *\n"
        << " 3|*  *  *  *  *  *  *  *  S  *\n"
        << " 4|*  *  *  *  *  S  S  *  *  *\n"
        << " 5|*  S  S  *  *  *  *  *  *  *\n"
        << " 6|*  *  *  *  *  *  *  *  *  *\n"
        << " 7|*  *  S  S  S  *  *  *  *  *\n"
        << " 8|*  *  *  *  *  *  *  *  *  *\n"
        << " 9|*  *  *  *  *  *  *  *  *  *\n\n"
        << "Placing ships works by entering 2 coordinates [x,y] and 1 character [H/V] for whether the ship will have a horizontal or vertical orientation.\n"
        << "For instance the ship in the top left was obviously entered with [0 0 V] in the phase of placing the 2x1 ships\n";
        if(game.slowMode) cout << "Taking a few seconds for user to read...\n\n";
        if(game.slowMode) Sleep(25000);
        walls(2);
        cout << "4)After all ships have been placed we reach the phase of shooting where the 2 players take turns to make guesses\n"
        << "The user will first get the known ships of the opponent that will look something like : \n\n"
        << "  0  1  2  3  4  5  6  7  8  9\n"
        << " ------------------------------\n"
        << "0|O  *  #  #  #  #  *  #  #  *\n"
        << "1|O  #  *  #  #  #  #  *  O  #\n"
        << "2|#  O  O  O  O  O  #  #  O  #\n"
        << "3|#  #  #  *  #  #  #  #  O  #\n"
        << "4|X  X  X  #  *  #  #  #  #  *\n"
        << "5|*  O  O  #  #  *  #  #  #  #\n"
        << "6|#  *  *  #  #  #  *  #  #  #\n"
        << "7|#  #  O  O  O  #  #  *  #  #\n"
        << "8|#  *  #  #  #  *  #  #  *  #\n"
        << "9|*  #  #  *  O  O  O  O  #  *\n";
        if(game.slowMode) cout << "Taking a few seconds for user to read...\n\n";
        if(game.slowMode) Sleep(17000);
        walls(2);
        cout << "5)A legal shoot here would be [4 3] since it is free (unknown for now), [0 0] would be not since it's already a square that has been targeted in the past.\n"
        << "If a shot is successful, there is a chance it completes a whole sunk ship in which case the game will print a message like :\n"
        << "[4, 0] -> [4, 3] has sunk!\n"
        << "To avoid any confusion a list of the sunk ships will always be provided at each move before the player's guess like this :\n"
        << "Sunk ships :\n"
        << "[0, 0] -> [1, 0]\n"
        << "[5, 1] -> [5, 2]\n"
        << "...\n"
        << "[2, 1] -> [2, 5]\n";
        if(game.slowMode) cout << "Taking a few seconds for user to read...\n\n";
        if(game.slowMode) Sleep(25000);
        walls(2);
        cout << "6)At the end of the game a message will be displayed to say who won in how many moves and how far the other player was from winning.\n"
        << "Further instructions will be provided throughout the game. Enjoy playing!\n\n";
        walls(3);
    }

}

//allocating memory in battlefield
void allocateMemoryBattlefield(Player& player) {
    player.battlefield = new char*[game.battlefieldSize];
    player.revealedByOpponent = new bool*[game.battlefieldSize];

    for (int i = 0; i < game.battlefieldSize; i++) {
        player.battlefield[i] = new char[game.battlefieldSize];
        player.revealedByOpponent[i] = new bool[game.battlefieldSize];

        //initializing battlefield
        for (int j = 0; j < game.battlefieldSize; j++) {
            player.battlefield[i][j] = symbolForNothing;
            player.revealedByOpponent[i][j] = false;
        }
    }
}

//function to clear the memory
void freeMemoryBattlefield(Player& player) {
    for (int i = 0; i < game.battlefieldSize; ++i) {
        delete[] player.battlefield[i];
        delete[] player.revealedByOpponent[i];
    }

    delete[] player.battlefield;
    delete[] player.revealedByOpponent;
}

//allocating memory for ships
void allocateMemoryShips(Player& player) {
    player.ships = new Ship[game.boatsCount + game.submarinesCount + game.destroyersCount + game.carriersCount];
}

//function to clear the memory
void freeMemoryShips(Player& player) {
    delete[] player.ships;
}

//inputting grid size and ship count
void setSize() {
    if(game.defaultSettings) {
        game.battlefieldSize = defaultGridSize;
    } else {
        cout<<"Now how big do you want the battlefield to be? Enter a value between " << minGridSize << "-" << maxGridSize << "!\n";

        int battlefieldSize;
        cin>>battlefieldSize;
        while(battlefieldSize<minGridSize || battlefieldSize>maxGridSize) {
            cout << "Please enter a value between " << minGridSize << "-" << maxGridSize <<"!\n";
            cin >> battlefieldSize;
        }
        cout<<"\n";
        game.battlefieldSize = battlefieldSize;
    }

    allocateMemoryBattlefield(player1);
    allocateMemoryBattlefield(player2);

}

//setting how many ships of each kind are in game
void enterShipCount() {
    cout << "Remember, there must be at least one of each type!\n\n";
    if(game.slowMode) Sleep(1500);
    if(game.battlefieldSize != 5) {
        cout << "Also the total area of those ships can't be more than \n"
        << game.battlefieldSize*game.battlefieldSize*4/10 << "\n - 40% of the total battlefield area ("
        << game.battlefieldSize*game.battlefieldSize << ").\n\n";
    }
    if(game.slowMode) Sleep(3000);
    cout << "How many boats? (1x2 ships) : ";
    cin >> game.boatsCount;
    cout << "How many submarines? (1x3 ships) : ";
    cin >> game.submarinesCount;
    cout << "How many destroyers? (1x4 ships) : ";
    cin >> game.destroyersCount;
    cout << "How many carriers? (1x5 ships) : ";
    cin >> game.carriersCount;
    game.shipsCount = game.boatsCount + game.submarinesCount + game.destroyersCount + game.carriersCount;

}

void setShipCount() {
    if(game.defaultSettings) {
        game.boatsCount = defaultBoats;
        game.submarinesCount = defaultSubmarines;
        game.destroyersCount = defaultDestroyers;
        game.carriersCount = defaultCarriers;
    } else {
        cout << "Now set the amount of each type of ship that each player will have on their battlefield.\n";
        enterShipCount();
        if(game.slowMode) Sleep(1500);
        int shipsArea = game.boatsCount*2 + game.submarinesCount*3 + game.destroyersCount*4 + game.carriersCount*5;
        cout << "Total area = " << shipsArea << "\n";
        walls(2);
        if(game.slowMode) Sleep(1500);

        while(shipsArea > game.battlefieldSize*game.battlefieldSize*4/10 || game.boatsCount<1 || game.submarinesCount<1 || game.destroyersCount<1 || game.carriersCount<1) {
            if(game.battlefieldSize == 5 && game.boatsCount == 1 && game.submarinesCount == 1 && game.destroyersCount == 1 && game.carriersCount == 1) break; //passes
            cout<<"\nUnfortunately that input doesn't meet the requirements. Please, try again. Total area = " << shipsArea << "\n";
            enterShipCount();
            shipsArea = game.boatsCount*2 + game.submarinesCount*3 + game.destroyersCount*4 + game.carriersCount*5;
        }
        player1.remainingShipSquares = player2.remainingShipSquares = shipsArea;
    }

    game.shipsCount = game.boatsCount + game.submarinesCount + game.destroyersCount + game.carriersCount;
    player1.remainingShipsTotalCount = game.shipsCount;
    player2.remainingShipsTotalCount = game.shipsCount;
    player1.remainingShipsOfKind[0] = game.boatsCount;
    player2.remainingShipsOfKind[0] = game.boatsCount;
    player1.remainingShipsOfKind[1] = game.submarinesCount;
    player2.remainingShipsOfKind[1] = game.submarinesCount;
    player1.remainingShipsOfKind[2] = game.destroyersCount;
    player2.remainingShipsOfKind[2] = game.destroyersCount;
    player1.remainingShipsOfKind[3] = game.carriersCount;
    player2.remainingShipsOfKind[3] = game.carriersCount;

    allocateMemoryShips(player1);
    allocateMemoryShips(player2);

    cout<<"\n";

}

//choosing between player vs player or player vs computer gamemodes
void chooseGameMode(int& mode, Player& player1, Player& player2) {

    cout << "Now please choose a game mode between 1 and 3 :\n"
        "1 - Player vs. Player\n"
        "2 - Player vs. Computer\n"
        "3 - Computer vs Computer\n\n";
    bool firstEntry = true;

    do {
        if(firstEntry) {
            firstEntry = false;
        } else {
            cout<<"Incorrect input! Please enter the number 1, 2 or 3!\n";
        }
        char input;
        cin >> input;
        cout << "\n";
        mode = validCheckDigit(input);
    } while(mode!=1 && mode!=2 && mode!=3);

    if(mode == 2) cout << "Good luck against the computer! You'll need it!\n\n";
    if(game.slowMode) Sleep(1500);

    if(mode==1 || mode==2) {
        player1.isHuman = true;
        cout << "Player 1 :\n";
        setHumanName(&player1);

    } else {
        player1.isHuman = false;
        player1.name = computerName1;
    }

    if(mode==2 || mode==3) {
        player2.isHuman = false;
        player2.name = computerName2;
    } else {
        player2.isHuman = true;
        cout << "Player 2 :\n";
        setHumanName(&player2);
    }
    walls(3);
    if(game.slowMode) Sleep(2000);

}

void slowOrNot() {
    char yesOrNo;
    cout << "Would you like for there to be pauses between moves so it's easier to follow? (Y/N)\n";
    cin >> yesOrNo;

    while(yesOrNo!='Y' && yesOrNo!='y' && yesOrNo!='N' && yesOrNo!='n') {
        cout << "Incorrect input! Please enter Y for slower mode or N for no pauses\n";
        cin >> yesOrNo;
    }

    if(yesOrNo=='Y' || yesOrNo=='y') game.slowMode = true;
}

//who goes first
void setMoveOrder() {

    cout << "OK. Now who goes first? Type :\n1 for " << game.player1->name
    << "\n2 for " << game.player2->name << "\n";

    char input;
    cin >> input;
    int first = validCheckDigit(input);
    while(!(first == 1 || first == 2)) {
        cout << "Please enter 1 or 2\n";
        cin >> input;
        first = validCheckDigit(input);
    }

    if(first == 2) game.regularMoveOrder = false;
}

//checking if the placement of the ship is legal
bool legalPlacement(Player* player, int x, int y, char direction, int lengthOfShip) {
    if(direction == 'H' || direction == 'h') {
        if(y + lengthOfShip <= game.battlefieldSize) {
            for(int i = y; i < y + lengthOfShip; i++) {
                if(player->battlefield[x][i]==symbolForShip) {
                    return false;
                }
            }
            return true;
        }
    } else {
        if(x + lengthOfShip <= game.battlefieldSize) {
            for(int i = x; i < x + lengthOfShip; i++) {
                if(player->battlefield[i][y]==symbolForShip) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

//placing ships on battlefield
void placeShipOnBattlefield(char** battlefield, int x, int y, char direction, int lengthOfShip) {
    int index;
    if(direction == 'H' || direction == 'h') index = y;
    else index = x;

    for(int i = index; i < index + lengthOfShip; i++) {
        if(direction == 'H' || direction == 'h') {
            battlefield[x][i] = symbolForShip;
        } else {
            battlefield[i][y] = symbolForShip;
        }
    }

}

//random algorithm to place computer's ships
void generateRandomPlacement(int& x, int& y, int& d) {
    x = rand()%game.battlefieldSize;
    y = rand()%game.battlefieldSize;
    d = rand()%2;
}

//computer placing their ships
void computerPlaceShips(Player* player) {

    printBattlefield(player, true);

    int shipsPlacedIndex = 0;

    for(int i = 0; i < 4; i++) {
        int length = i + 2;
        int countOfType; //for our loop
        string typeName[4] = {"boat", "submarine", "destroyer", "carrier"}; //for our loop

        switch (length) {
            case 2 :
                cout << "Let's start with the boats (1x2). You have to place " << game.boatsCount << " of them.\n";
                countOfType = game.boatsCount;
                break;
            case 3 :
                cout << "Now let's do the submarines (1x3). You have to place " << game.submarinesCount << " of them.\n";
                countOfType = game.submarinesCount;
                break;
            case 4 :
                cout << "Let's move on to the destroyers (1x4). You have to place " << game.destroyersCount << " of them.\n";
                countOfType = game.destroyersCount;
                break;
            case 5 :
                cout << "And finally, time to place the carriers (1x5). You have to place " << game.carriersCount << " of them.\n";
                countOfType = game.carriersCount;
                break;
        }

        for(int j = 0; j < countOfType; j++) {
            cout << "Entering for " << typeName[i] << " number " << j+1 <<"\n";
            int x, y, dir;
            char direction;
            generateRandomPlacement(x, y, dir);
            if(dir == 1) direction = 'V';
            else direction = 'H';

            bool inputIsOk = (x>=0 && x<=game.battlefieldSize-1 && y>=0 && y<=game.battlefieldSize-1 &&
                     (direction=='H' || direction=='h' || direction=='V' || direction=='v'));

            while(!(inputIsOk && legalPlacement(player, x, y, direction, length))) {
                if(inputIsOk) {
                    cout<< "\nIllegal Placement! Please take in consideration the other already placed ships and the dimensions of the battlefield!\n\n";
                    printBattlefield(player, true);
                    cout<<"Please enter the values again :\n";
                } else {
                    cout << "Incorrect input! Try again!\n";
                    cout << "Please for each ship enter coordinates x and y ranging between 0 and " << game.battlefieldSize-1 <<", followed by a letter :\n";
                    cout << "H - for horizontal or\nV - for vertical\n\n";
                }

                generateRandomPlacement(x, y, dir);
                if(dir == 1) direction = 'V';
                else direction = 'H';

                inputIsOk = (x>=0 && x<=game.battlefieldSize-1 && y>=0 && y<=game.battlefieldSize-1 &&
                     (direction=='H' || direction=='h' || direction=='V' || direction=='v'));
            }

            //adding that ship's information to the player's fleet
            player->ships[shipsPlacedIndex].x = x;
            player->ships[shipsPlacedIndex].y = y;
            if(direction=='H' || direction=='h') player->ships[shipsPlacedIndex].direction = Horizontal;
            else player->ships[shipsPlacedIndex].direction = Vertical;
            player->ships[shipsPlacedIndex].length = length;
            player->ships[shipsPlacedIndex].shipType = static_cast<ShipType> (i); //casting i to ShipType


            placeShipOnBattlefield(player->battlefield, x, y, direction, length);

            cout<<"\n";
            printBattlefield(player, true);

            shipsPlacedIndex++;
        }
    }

    cout<<"Ships placed by " << player->name << "!\n\n";
}

//human placing their ships
void humanPlaceShips(Player* player) {
    cout << "Time for " << player->name << " to place their ships!\n\n";

    printBattlefield(player, true);

    cout << "Please for each ship enter coordinates x and y ranging between 0 and " << game.battlefieldSize-1 << ", followed by a letter :\n"
    << "H - for horizontal or\nV - for vertical\n"
    << "Example input :\n0 0 H\n\n";

    int shipsPlacedIndex = 0;

    for(int i = 0; i < 4; i++) {
        int length = i + 2;
        int countOfType; //for our loop
        string typeName[4] = {"boat", "submarine", "destroyer", "carrier"}; //for our loop

        switch (length) {
            case 2 :
                cout << "Let's start with the boats (1x2). You have to place " << game.boatsCount << " of them.\n";
                countOfType = game.boatsCount;
                break;
            case 3 :
                cout << "Now let's do the submarines (1x3). You have to place " << game.submarinesCount << " of them.\n";
                countOfType = game.submarinesCount;
                break;
            case 4 :
                cout << "Let's move on to the destroyers (1x4). You have to place " << game.destroyersCount << " of them.\n";
                countOfType = game.destroyersCount;
                break;
            case 5 :
                cout << "And finally, time to place the carriers (1x5). You have to place " << game.carriersCount << " of them.\n";
                countOfType = game.carriersCount;
                break;
        }

        for(int j = 0; j < countOfType; j++) {

            int x, y;
            char direction;

            //remove!!!
            //if((length != 2  && length != 3) || !game.defaultSettings) {
            if(true) {
                cout << "Entering for " << typeName[i] << " number " << j+1 <<" :\n";

                cin >> x >> y >> direction;
                cout << "\n";
                bool inputIsOk = (x>=0 && x<=game.battlefieldSize-1 && y>=0 && y<=game.battlefieldSize-1 &&
                         (direction=='H' || direction=='h' || direction=='V' || direction=='v'));

                while(!(inputIsOk && legalPlacement(player, x, y, direction, length))) {
                    if(inputIsOk) {
                        cout<< "\nIllegal Placement! Please take in consideration the other already placed ships and the dimensions of the battlefield!\n\n";
                        printBattlefield(player, true);
                        if(game.slowMode) Sleep(3000);
                        cout<<"Please enter the values again :\n";
                    } else {
                        cout << "Incorrect input! Try again!\n";
                        cout << "Please for each ship enter coordinates x and y ranging between 0 and " << game.battlefieldSize-1 <<", followed by a letter :\n";
                        cout << "H - for horizontal or\nV - for vertical\n\n";
                        if(game.slowMode) Sleep(3000);
                    }
                    cin >> x >> y >> direction;
                    inputIsOk = (x>=0 && x<=game.battlefieldSize-1 && y>=0 && y<=game.battlefieldSize-1 &&
                         (direction=='H' || direction=='h' || direction=='V' || direction=='v'));
                }
            }

            //adding that ship's information to the player's fleet
            player->ships[shipsPlacedIndex].x = x;
            player->ships[shipsPlacedIndex].y = y;
            if(direction=='H' || direction=='h') player->ships[shipsPlacedIndex].direction = Horizontal;
            else player->ships[shipsPlacedIndex].direction = Vertical;
            player->ships[shipsPlacedIndex].length = length;
            player->ships[shipsPlacedIndex].shipType = static_cast<ShipType> (i); //casting i to ShipType


            placeShipOnBattlefield(player->battlefield, x, y, direction, length);

            cout<<"\n";
            printBattlefield(player, true);

            shipsPlacedIndex++;
        }
    }

    cout << "Affirmative!\n\n";
    if(game.slowMode) Sleep(5000);
}

//deciding if the player is a human and placing ships
void placeShips(Player* player) {
    if(player->isHuman) humanPlaceShips(player);
    else computerPlaceShips(player);
}

void copyIntField(int** fieldFrom, int** fieldTo) {
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            fieldTo[i][j] = fieldFrom[i][j];
        }
    }
}

void copyCharField(char** fieldFrom, char** fieldTo) {
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            fieldTo[i][j] = fieldFrom[i][j];
        }
    }
}

void sumMatrices(long long** matrixTo, long long** matrixAdding) {
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            matrixTo[i][j] += matrixAdding[i][j];
        }
    }
}

void nullIntMatrix(int** matrix) {
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            matrix[i][j] = 0;
        }
    }
}

void nullCharMatrix(char** matrix) {
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            matrix[i][j] = '0';
        }
    }
}

void nullLongLongMatrix(long long** matrix) {
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            matrix[i][j] = 0;
        }
    }
}

int** allocateMemoryIntMatrix() {
    int** matrix = new int*[game.battlefieldSize];

    for (int i = 0; i < game.battlefieldSize; i++) {
        matrix[i] = new int[game.battlefieldSize];
    }

    return matrix;
}

void freeMemoryIntMatrix(int** matrix) {
    for (int i = 0; i < game.battlefieldSize; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;
}

char** allocateMemoryCharMatrix() {
    char** matrix = new char*[game.battlefieldSize];

    for (int i = 0; i < game.battlefieldSize; i++) {
        matrix[i] = new char[game.battlefieldSize];
    }

    return matrix;
}

void freeMemoryCharMatrix(char** matrix) {
    for (int i = 0; i < game.battlefieldSize; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;
}

long long** allocateMemoryLongLongMatrix() {
    long long** matrix = new long long*[game.battlefieldSize];

    for (int i = 0; i < game.battlefieldSize; i++) {
        matrix[i] = new long long[game.battlefieldSize];
    }

    return matrix;
}

void freeMemoryLongLongMatrix(long long** matrix) {
    for (int i = 0; i < game.battlefieldSize; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;
}

///functions that are part of the shooting logic

//if in battlefield range
bool legalCoordinates(int x, int y, int lengthOfShip, char dir) {
    if(dir == 'H')  return ((y + lengthOfShip <= game.battlefieldSize) ? true : false);
    else return ((x + lengthOfShip <= game.battlefieldSize) ? true : false);
}

//has to be an unknown square
bool legalShoot(int x, int y, int lengthOfShip, char dir, char** knownField) {
    bool legal = legalCoordinates(x, y, lengthOfShip, dir);
    if(legal) {
        if(dir == 'H') {
            for(int i = y; i < y + lengthOfShip; i++) {
                if(knownField[x][i] == symbolForShip || knownField[x][i] == symbolForNothing || knownField[x][i] == symbolForSunkShip) {
                    return false;
                }
            }
            return true;
        } else {
            for(int i = x; i < x + lengthOfShip; i++) {
                if(knownField[i][y] == symbolForShip || knownField[i][y] == symbolForNothing || knownField[i][y] == symbolForSunkShip) {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}

void convertGuessToScore(char** ourGuess, long long** score) {
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            if(ourGuess[i][j] == symbolForShip) score[i][j] = 1;
            else score[i][j] = 0;
        }
    }
}

bool thereIsKnownNotPlaced(char** ourGuess) {
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            if(ourGuess[i][j] == symbolForKnownShip) return true;
        }
    }
    return false;
}

void printSunkShipsList(Player* otherPlayer) {
    int amountOfSunk = game.shipsCount - otherPlayer->remainingShipsTotalCount;
    if(amountOfSunk == 0) cout << otherPlayer->name << " has no sunk ships yet!\n";
    else cout << otherPlayer->name << " has " << amountOfSunk << " sunk ship(s)!\n";
    if(game.slowMode) Sleep(2000);

    cout << otherPlayer->remainingShipsTotalCount << " remain in the game -> [boats, submarines, destroyers, carriers] = ["
    << otherPlayer->remainingShipsOfKind[0] << ", "
    << otherPlayer->remainingShipsOfKind[1] << ", "
    << otherPlayer->remainingShipsOfKind[2] << ", "
    << otherPlayer->remainingShipsOfKind[3] << "]\n\n";

    if(game.slowMode) Sleep(1000);
    if(amountOfSunk == 0) return ;

    cout << "Here's a list of all the sunk ships :\n";
    for(int i = 0; i < game.shipsCount; i++) {
        if(otherPlayer->ships[i].sunk) {
            int x = otherPlayer->ships[i].x;
            int y = otherPlayer->ships[i].y;
            int length = otherPlayer->ships[i].length;
            Direction direction = otherPlayer->ships[i].direction;

            cout << "[" << x << ", " << y << "] -> ["
            << ((direction == Horizontal) ? (x) : (x + length - 1)) << ", "
            << ((direction == Horizontal) ? (y + length - 1) : (y)) << "]\n";
        }
    }
    cout << "\n";
    if(game.slowMode) Sleep(1000);
}

void makeAGuess(Player* player, int guessX, int guessY) {
    Player* otherPlayer = otherPlayerFinder(player);
    otherPlayer->revealedByOpponent[guessX][guessY] = true;

    if(otherPlayer->battlefield[guessX][guessY] == symbolForShip) {
        cout << "Great success! " << player->name << " hit " << otherPlayer->name << "'s ship on " << guessX << ", " << guessY << "!\n";
        otherPlayer->remainingShipSquares--;
        Ship* ship = findShipByCoordinates(otherPlayer, guessX, guessY);
        bool sunk = true;
        if(ship->direction == Horizontal) {
            for(int i = ship->y; i < ship->y + ship->length; i++) {
                if(!otherPlayer->revealedByOpponent[ship->x][i]) {
                    sunk = false;
                    break;
                }
            }
        } else {
            for(int i = ship->x; i < ship->x + ship->length; i++) {
                if(!otherPlayer->revealedByOpponent[i][ship->y]) {
                    sunk = false;
                    break;
                }
            }
        }

        if(sunk) {
            ship->sunk = true;
            cout << otherPlayer->name << "'s " << ship->length << "x1 ship which was on squares ";
            if(ship->direction == Horizontal) {
                cout << "[" << ship->x << "," << ship->y << "] -> [" << ship->x << "," << ship->y + ship->length - 1 << "] has sunk!\n\n";
            } else {
                cout << "[" << ship->x << "," << ship->y << "] -> [" << ship->x + ship->length - 1 << "," << ship->y << "] has sunk!\n\n";
            }
            otherPlayer->remainingShipsTotalCount--;
            otherPlayer->remainingShipsOfKind[ship->length-2]--;
        }
    } else {
        cout << "Miss! " << player->name << "'s attempt on " << otherPlayer->name << " at " << guessX << ", " << guessY << " was innacurate!\n";
    }
}

//complex algorithm we only use when there is a small enough amount of ships left to place since it's slow otherwise
long long** computerPlaceComplexAlgorithm(Player* otherPlayer, int shipIndex, char** field) {
    long long** result = allocateMemoryLongLongMatrix();
    nullLongLongMatrix(result);

    int totalShips = game.shipsCount;

    //if all ships have been placed
    if(shipIndex==0) {
        if(!thereIsKnownNotPlaced(field)) convertGuessToScore(field, result);
        return result;
    }

    //skipping sunk ships
    Ship* ship = &(otherPlayer->ships[totalShips - shipIndex]);
    if(ship->sunk) {
        return computerPlaceComplexAlgorithm(otherPlayer, shipIndex-1, field);
    }

    //calculating ship length
    int shipLength;
    if(shipIndex >= totalShips - game.boatsCount + 1) {
        shipLength = 2;
    } else {
        if(shipIndex >= totalShips - game.boatsCount - game.submarinesCount + 1) {
            shipLength = 3;
        } else {
            if(shipIndex >= totalShips - game.boatsCount - game.submarinesCount - game.destroyersCount + 1) shipLength = 4;
            else shipLength = 5;
        }
    }

    //adding all the options for placing the next ship to our calculation
    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            for(int k = 0; k < 2; k++) {
                char dir = 'H';
                if(k==1) dir = 'V';

                char** ourGuess = allocateMemoryCharMatrix();
                nullCharMatrix(ourGuess);

                if(legalShoot(i, j, shipLength, dir, field)) {
                    copyCharField(field, ourGuess);
                    placeShipOnBattlefield(ourGuess, i, j, dir, shipLength);
                    long long** add = computerPlaceComplexAlgorithm(otherPlayer, shipIndex - 1, ourGuess);
                    sumMatrices(result, add);
                    freeMemoryLongLongMatrix(add);
                }

                freeMemoryCharMatrix(ourGuess);
            }
        }
    }

    return result;
}

//check if square is valid
bool legitSquare(int i, int j, char** field, char symbol) {
    return (i >= 0 && i < game.battlefieldSize && j >= 0 && j < game.battlefieldSize && field[i][j] == symbol);
}

//simpler algorithm to place ships based on human logic
long long** computerPlaceSimpleAlgorithm(char** field) {
    long long** result = allocateMemoryLongLongMatrix();
    nullLongLongMatrix(result);

    int foundKnownShips = 0;
    int firstEdgeX, firstEdgeY, secondEdgeX, secondEdgeY;

    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            if(field[i][j] == symbolForKnownShip) {
                foundKnownShips++;
                if(foundKnownShips == 1) {
                    firstEdgeX = i;
                    firstEdgeY = j;
                }
            }
        }
    }

    bool deadEnd = true;
    if(foundKnownShips >= 2) {
        int newCoordinate;
        if(legitSquare(firstEdgeX, firstEdgeY+1, field, symbolForKnownShip)) {
            //cout << "Horizontal revealed\n";
            //horizontal
            newCoordinate = firstEdgeY+2;
            while(legitSquare(firstEdgeX, newCoordinate, field, symbolForKnownShip)) {
                newCoordinate++;
            }
            newCoordinate--;
            secondEdgeX = firstEdgeX;
            secondEdgeY = newCoordinate;
            //cout << "First Edge - " << firstEdgeX << ", " << firstEdgeY
            //<< "\nSecond Edge - " << secondEdgeX << ", " << secondEdgeY << "\n";

            for(newCoordinate = firstEdgeY - 1; legitSquare(firstEdgeX, newCoordinate, field, symbolForHidden); newCoordinate--) {
                //cout << "Going left\n";
                result[firstEdgeX][firstEdgeY-1]++;
                deadEnd = false;
            }

            for(newCoordinate = secondEdgeY + 1; legitSquare(firstEdgeX, newCoordinate, field, symbolForHidden); newCoordinate++) {
                //cout << "Going right\n";
                result[secondEdgeX][secondEdgeY+1]++;
                deadEnd = false;
            }

        } else {
            //vertical
            //cout << "Vertical revealed\n";
            newCoordinate = firstEdgeX+2;
            while(legitSquare(newCoordinate, firstEdgeY, field, symbolForKnownShip)) {
                newCoordinate++;
            }
            newCoordinate--;
            secondEdgeX = newCoordinate;
            secondEdgeY = firstEdgeY;

            for(newCoordinate = firstEdgeX - 1; legitSquare(newCoordinate, firstEdgeY, field, symbolForHidden); newCoordinate--) {
                //cout << "Going up\n";
                result[firstEdgeX-1][firstEdgeY]++;
                deadEnd = false;
            }

            for(newCoordinate = secondEdgeX + 1; legitSquare(newCoordinate, firstEdgeY, field, symbolForHidden); newCoordinate++) {
                //cout << "Going down\n";
                result[secondEdgeX+1][secondEdgeY]++;
                deadEnd = false;
            }
        }
        //if(deadEnd) cout << "Dead End!!!\n";
    }

    if(foundKnownShips == 1 || (foundKnownShips >= 2 && deadEnd)) {
        for(int i = 0; i < game.battlefieldSize; i++) {
            for(int j = 0; j < game.battlefieldSize; j++) {
                if(field[i][j] == symbolForKnownShip) {
                    for(int p = 0; p < 4; p++) {
                        int depth = 0;
                        bool legit;
                        int newI = i, newJ = j;
                        if(p==0) {
                            legit = legitSquare(i, j-1, field, symbolForHidden);
                            newJ--;
                        }
                        if(p==1) {
                            legit = legitSquare(i, j+1, field, symbolForHidden);
                            newJ++;
                        }
                        if(p==2) {
                            legit = legitSquare(i-1, j, field, symbolForHidden);
                            newI--;
                        }
                        if(p==3) {
                            legit = legitSquare(i+1, j, field, symbolForHidden);
                            newI++;
                        }
                        if(legit && legitSquare(newI, newJ, field, symbolForHidden)) {
                            for(int k = newJ - 1; legitSquare(newI, k, field, symbolForHidden) ; k--) depth++; //left
                            for(int k = newJ + 1; legitSquare(newI, k, field, symbolForHidden); k++) depth++; //right
                            for(int k = newI - 1; legitSquare(k, newJ, field, symbolForHidden); k--) depth++; //up
                            for(int k = newI + 1; legitSquare(k, newJ, field, symbolForHidden); k++) depth++; //down
                            result[newI][newJ] = depth;
                        }
                    }
                }
            }
        }
    }
    if(foundKnownShips == 0) {
        for(int i = 0; i < game.battlefieldSize; i++) {
            for(int j = 0; j < game.battlefieldSize; j++) {
                if(legitSquare(i, j, field, symbolForHidden)) {
                    int minDistanceToWall = game.battlefieldSize;
                    int distanceCount;
                    int depth = 0;

                    distanceCount = 0;
                    for(int k = j - 1; legitSquare(i, k, field, symbolForHidden) ; k--) {
                        depth++;
                        distanceCount++;
                    }
                    if(distanceCount < minDistanceToWall) minDistanceToWall = distanceCount;

                    distanceCount = 0;
                    for(int k = j + 1; legitSquare(i, k, field, symbolForHidden); k++) {
                        depth++;
                        distanceCount++;
                    }
                    if(distanceCount < minDistanceToWall) minDistanceToWall = distanceCount;

                    distanceCount = 0;
                    for(int k = i - 1; legitSquare(k, j, field, symbolForHidden); k--) {
                        depth++;
                        distanceCount++;
                    }
                    if(distanceCount < minDistanceToWall) minDistanceToWall = distanceCount;

                    distanceCount = 0;
                    for(int k = i + 1; legitSquare(k, j, field, symbolForHidden); k++) {
                        depth++;
                        distanceCount++;
                    }
                    if(distanceCount < minDistanceToWall) minDistanceToWall = distanceCount;

                    result[i][j] = depth + minDistanceToWall;
                }
            }
        }
    }

    return result;
}

char** maskedBattlefield(Player* player) {
    char** maskedField = allocateMemoryCharMatrix(); //freed memory in void computerShoot later

    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            if(player->revealedByOpponent[i][j]) {
                if(player->battlefield[i][j] == symbolForShip){
                    Ship* ship = findShipByCoordinates(player, i, j);
                    if(ship->sunk) maskedField[i][j] = symbolForSunkShip;
                    else maskedField[i][j] = symbolForKnownShip;
                }
                else maskedField[i][j] = symbolForNothing; //symbolForNothing
            } else {
                maskedField[i][j] = symbolForHidden;
            }
        }
    }

    return maskedField;
}

bool isComputerComplexAlgorithmFastEnough (int remainingShips) {
    long long multiplier = game.battlefieldSize*game.battlefieldSize*2;
    long long speed = 1;
    long long threshold = 10000000; //10M

    while(remainingShips > 0) {
        speed*=multiplier;
        //threshold for too much time
        if(speed > threshold) {
            cout << "Complex algorithm too slow for now. Proceeding with simple algorithm.\n";
            cout << "Maybe if " << remainingShips << " more ships were sunk we could use complex algorithm...\n\n";
            if(game.slowMode) Sleep(3000);
            return false;
        }
        remainingShips--;
    }

    cout << "Shouldn't be a problem for complex algorithm!\n\n";
    if(game.slowMode) Sleep(2000);
    return true;
}

void computerShoot(Player* player) {
    cout << "Beginning of move number " << game.moveCount << " for " << player->name << "\n";
    if(game.slowMode) Sleep(1000);

    Player* otherPlayer = otherPlayerFinder(player);
    printBattlefield(otherPlayer, false);

    printSunkShipsList(otherPlayer);

    char** maskedField = maskedBattlefield(otherPlayer);

    long long** result;
    int remainingShips = otherPlayer->remainingShipsTotalCount;

    if(isComputerComplexAlgorithmFastEnough(remainingShips)) {
        //execute complex algorithm
        int totalShips = game.boatsCount + game.submarinesCount + game.destroyersCount + game.carriersCount;

        result = computerPlaceComplexAlgorithm(otherPlayer, totalShips, maskedField);

    } else {
        //execute simple algorithm
        result = computerPlaceSimpleAlgorithm(maskedField);
    }

    freeMemoryCharMatrix(maskedField);

    //evaluation print
    cout << "Here is a sneak peek of the evaluation behind all squares on the grid by the computer:\n\n";
    printLongLongMatrix(result);
    if(game.slowMode) Sleep(1000);

    int guessX = 0, guessY = 0;
    long long maxValue = 0;

    for(int i = 0; i < game.battlefieldSize; i++) {
        for(int j = 0; j < game.battlefieldSize; j++) {
            if((!otherPlayer->revealedByOpponent[i][j]) && result[i][j] > maxValue) {
                maxValue = result[i][j];
                guessX = i;
                guessY = j;
            }
        }
    }

    cout << player->name << "'s guess is " << guessX << ", " << guessY << "\n\n";
    makeAGuess(player, guessX, guessY);

    freeMemoryLongLongMatrix(result);

    cout << "Let's see how it's looking after shot!\n\n";
    printBattlefield(otherPlayer, false);
    if(game.slowMode) Sleep(1000);
    cout << otherPlayer->remainingShipSquares << " / " << game.boatsCount*2 + game.submarinesCount*3 + game.destroyersCount*4 + game.carriersCount*5
    << " ship squares remain to be hit! (";
    cout << player->remainingShipSquares << " / " << game.boatsCount*2 + game.submarinesCount*3 + game.destroyersCount*4 + game.carriersCount*5
    << " for " << otherPlayer->name << " to hit)\n";
    cout << "End of move number " << game.moveCount << " for " << player->name << "\n";
    walls(3);
    if(game.slowMode) Sleep(1000);
}

//for shooting by human
void humanShoot(Player* player) {
    cout << "Beginning of move number " << game.moveCount << " for " << player->name << "\n";
    if(game.slowMode) Sleep(1000);

    Player* otherPlayer = otherPlayerFinder(player);

    printBattlefield(otherPlayer, false);
    printSunkShipsList(otherPlayer);

    cout << "Pick a square : \nEnter coordinates x and y to shoot!\n";

    //check for incorrect input
    int guessX = -666, guessY = -666;
    //validating guess conditions
    while(!(guessX >= 0 && guessX < game.battlefieldSize && guessY >= 0 && guessY < game.battlefieldSize && otherPlayer->revealedByOpponent[guessX][guessY] == false)) {
        if(!(guessX == -666 && guessY == -666)) cout << "Invalid input! Please make sure your guess is between 0 and " << game.battlefieldSize-1 << "!\n\n";;
        if(guessX >= 0 && guessX < game.battlefieldSize && guessY >= 0 && guessY < game.battlefieldSize) {
            cout << "You already guessed that square! Please try again!\n\n";
        }
        cin >> guessX >> guessY;
    }
    cout << "\n";
    if(game.slowMode) Sleep(1000);
    makeAGuess(player, guessX, guessY);

    cout << "Let's see how it's looking after shot!\n\n";
    printBattlefield(otherPlayer, false);
    if(game.slowMode) Sleep(3000);
    cout << otherPlayer->remainingShipSquares << " / " << game.boatsCount*2 + game.submarinesCount*3 + game.destroyersCount*4 + game.carriersCount*5
    << " ship squares remain to be hit! (";
    cout << player->remainingShipSquares << " / " << game.boatsCount*2 + game.submarinesCount*3 + game.destroyersCount*4 + game.carriersCount*5
    << " for " << otherPlayer->name << " to hit)\n";
    cout << "End of move number " << game.moveCount << " for " << player->name << "\n";
    walls(3);
    if(game.slowMode) Sleep(4000);
}

//distributing to other functions depending on level of humanity
void shoot(Player* player) {
    if(!player->isHuman) {
        computerShoot(player);
        if(game.slowMode) Sleep(2000);
    } else {
        humanShoot(player);
    }
}

int main() {
    //for random
    srand(time(0));

    addPlayersToGame();

    slowOrNot();
    //intro
    pasteIntro();

    //rules
    rules();
    walls(2);

    //Game settings
    setSize();
    walls(2);
    setShipCount();
    walls(2);

    //choosing between 3 gamemodes
    int mode;
    chooseGameMode(mode, player1, player2);

    setMoveOrder();

    if(game.regularMoveOrder) {
        placeShips(&player1);
        lookAway();
    }
    placeShips(&player2);
    lookAway();
    if(!game.regularMoveOrder) {
        placeShips(&player1);
        lookAway();
    }

    while(player1.remainingShipsTotalCount > 0) {
        //regular move order case
        if(game.regularMoveOrder) {
            shoot(&player1);
            if(player2.remainingShipsTotalCount == 0) {
                cout << player1.name << " wins! All of " << player2.name << "'s ships have sunk in " << game.moveCount+1 << " moves!\n";
                cout << player2.name << " lost! They had " << player1.remainingShipsTotalCount << " ship(s) left to sink and "
                << player1.remainingShipSquares << " square(s) left to shoot!\n\n";
                break;
            }
        }

        shoot(&player2);
        game.moveCount++;

        //if move order is reversed
        if(!game.regularMoveOrder) {
            shoot(&player1);
            if((player2.remainingShipsTotalCount == 0)) {
                cout << player1.name << " wins! All of " << player2.name << "'s ships have sunk in " << game.moveCount+1 << " moves!\n";
                cout << player2.name << " lost! They had " << player1.remainingShipsTotalCount << " ship(s) left to sink and "
                << player1.remainingShipSquares << " square(s) left to shoot!\n\n";
                break;
            }
        }
    }

    if(player1.remainingShipsTotalCount == 0 ) {
        cout << player2.name << " wins! All of " << player1.name << "'s ships have sunk in " << game.moveCount << " moves!\n";
        cout << player1.name << " lost! They had " << player2.remainingShipsTotalCount << " ship(s) left to sink and "
        << player2.remainingShipSquares << " square(s) left to shoot!\n\n";
    }

    freeMemoryBattlefield(player1);
    freeMemoryBattlefield(player2);
    freeMemoryShips(player1);
    freeMemoryShips(player2);

    return 0;
}
