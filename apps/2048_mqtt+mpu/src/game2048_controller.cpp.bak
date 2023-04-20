#include "game2048_controller.h"
#include <Arduino.h>

int GAME2048::addRandom() {
    int rand;
    while (1) {
        rand = random(3200) % 16;
        if (this->board[rand / 4][rand % 4] == 0) {
            this->board[rand / 4][rand % 4] = 2;
            break;
        }
    }
    return rand;
}
/*
*   record original location, if there is number, 
*   then type in the number according to the direction with A B C D
*   direction  1.u 2.d 3.l 4.r
*/
void GAME2048::recordLocation(int direction) {
    for (int i = 0; i < SIZE; i++){
    for (int j = 0; j < SIZE; j++) {
        //empty if there is no number 
        if (board[i][j] == 0) {
            Location[i][j] = "";
        } else {
            //if there are numbers,fill in ABCD according to the direction
            switch (direction) {
            case 1://U
            case 2://D
                switch (i) {
                case 0: Location[i][j] = "A"; break;
                case 1: Location[i][j] = "B"; break;
                case 2: Location[i][j] = "C"; break;
                case 3: Location[i][j] = "D"; break;
                }
                break;
            case 3://L
            case 4://R
                switch (j) {
                case 0: Location[i][j] = "A"; break;
                case 1: Location[i][j] = "B"; break;
                case 2: Location[i][j] = "C"; break;
                case 3: Location[i][j] = "D"; break;
                }
                break;

            }
        }
    }}
}

/*
*analysis the change of the location, and record the move distance then combine the location.
*the number 1 means up, 2 means down, 3 means left, 4 means right 
*large than 4 then it has combination and -8 is the move value.
*smaller than 4 it will be the move value it self 
*/
void GAME2048::countMoveRecord(int direction) {

    //clear the game
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            moveRecord[i][j] = 0;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            switch (direction) {
            case 1:
            case 2:
                //move detection
                if (Location[i][j].find("A") != -1) {
                    moveRecord[0][j] += i;
                }
                if (Location[i][j].find("B") != -1) {
                    moveRecord[1][j] += i - 1;
                }
                if (Location[i][j].find("C") != -1) {
                    moveRecord[2][j] += i - 2;
                }
                if (Location[i][j].find("D") != -1) {
                    moveRecord[3][j] += i - 3;
                }
                break;
            case 3:
            case 4:
                //move detection
                if (Location[i][j].find("A") != -1) {
                    moveRecord[i][0] += j;
                }
                if (Location[i][j].find("B") != -1) {
                    moveRecord[i][1] += j - 1;
                }
                if (Location[i][j].find("C") != -1) {
                    moveRecord[i][2] += j - 2;
                }
                if (Location[i][j].find("D") != -1) {
                    moveRecord[i][3] += j - 3;
                }
                break;
            }
            //combine
            if (Location[i][j].length() == 2) {
                moveRecord[i][j] += 8;
            }
        }
    }
}

void GAME2048::moveUp() {
    recordLocation(1);   //record location
    recordBoard();      //record number 
    //move two times 
    for (int x = 0; x < 2; x++) {
        for (int i = 0; i < SIZE - 1; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = board[i + 1][j];
                    board[i + 1][j] = 0;
                    // animation path record
                    Location[i][j] = Location[i+1][j];
                    Location[i + 1][j] = "";
                }
            }
        }
    }

    //add up
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == board[i + 1][j]) {
                board[i][j] *= 2;
                board[i + 1][j] = 0;
                //animation path record
                Location[i][j].append(Location[i+1][j]);
                Location[i + 1][j] = "";
            }
        }
    }

    //move
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                board[i][j] = board[i + 1][j];
                board[i + 1][j] = 0;
                //record the animation path
                Location[i][j] = Location[i + 1][j];
                Location[i + 1][j] = "";
            }
        }
    }
    countMoveRecord(1);
}


void GAME2048::moveDown() {
    recordLocation(2);   
    recordBoard();
    for (int x = 0; x < 2; x++) {
        for (int i = SIZE - 1; i > 0; i--) {
            for (int j = SIZE - 1; j >= 0; j--) {
                if (board[i][j] == 0) {
                    board[i][j] = board[i - 1][j];
                    board[i - 1][j] = 0;
                     // animation path record
                    Location[i][j] = Location[i - 1][j];
                    Location[i - 1][j] = "";
                }
            }
        }
    }

    //add up
    for (int i = SIZE - 1; i > 0; i--) {
        for (int j = SIZE - 1; j >= 0; j--) {
            if (board[i][j] == board[i - 1][j]) {
                board[i][j] *= 2;
                board[i - 1][j] = 0;
                //animation path record
                Location[i][j].append(Location[i - 1][j]);
                Location[i - 1][j] = "";
            }
        }
    }

    //move
    for (int i = SIZE - 1; i > 0; i--) {
        for (int j = SIZE - 1; j >= 0; j--) {
            if (board[i][j] == 0) {
                board[i][j] = board[i - 1][j];
                board[i - 1][j] = 0;
                //animation path record
                Location[i][j] = Location[i - 1][j];
                Location[i - 1][j] = "";
            }
        }
    }
    countMoveRecord(2);
}

void GAME2048::moveLeft() {
    recordLocation(3);   //记录位置
    recordBoard();
    //移动
    for (int x = 0; x < 2; x++) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE - 1; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = board[i][j + 1];
                    board[i][j + 1] = 0;
                    //animation path record
                    Location[i][j] = Location[i][j + 1];
                    Location[i][j+1] = "";
                }
            }
        }
    }
    //add up
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE - 1; j++) {
            if (board[i][j] == board[i][j + 1]) {
                board[i][j] *= 2;
                board[i][j + 1] = 0;
                //animation path record
                Location[i][j].append(Location[i][j+1]);
                Location[i][j+1] = "";
            }
        }
    }
    //移动
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE - 1; j++) {
            if (board[i][j] == 0) {
                board[i][j] = board[i][j + 1];
                board[i][j + 1] = 0;
                //animation path record
                Location[i][j] = Location[i][j + 1];
                Location[i][j + 1] = "";
            }
        }
    }
    countMoveRecord(3);
}

void GAME2048::moveRight() {
    recordLocation(4);   
    recordBoard();
    //move two times
    for (int x = 0; x < 2; x++) {
        for (int i = SIZE - 1; i >= 0; i--) {  
            for (int j = SIZE - 1; j > 0; j--) {
                if (board[i][j] == 0) {
                    board[i][j] = board[i][j - 1];
                    board[i][j - 1] = 0;
                     // animation path record
                    Location[i][j] = Location[i][j - 1];
                    Location[i][j - 1] = "";
                }
            }
        }
    }

    //相加
    for (int i = SIZE - 1; i >= 0; i--) {
        for (int j = SIZE - 1; j > 0; j--) {
            if (board[i][j] == board[i][j - 1]) {
                board[i][j] *= 2;
                board[i][j - 1] = 0;
                 // animation path record
                Location[i][j].append(Location[i][j - 1]);
                Location[i][j - 1] = "";
            }
        }
    }

    //move
    for (int i = SIZE - 1; i >= 0; i--) {
        for (int j = SIZE - 1; j > 0; j--) {
            if (board[i][j] == 0) {
                board[i][j] = board[i][j - 1];
                board[i][j - 1] = 0;
              // animation path record
                Location[i][j] = Location[i][j - 1];
                Location[i][j - 1] = "";
            }
        }
    }
    countMoveRecord(4);
}

/*
way of judging the game to make sure moving on
or fail or successed
for return 0: there is empty
return 1: win the game
return 2: fail the game
*/
int GAME2048::judge() {
    //win
    for (int i = 0; i <= SIZE * SIZE; i++) {
        if (board[i / 4][i % 4] >= WIN_SCORE) {
            return 1;       //Win
        }
    }
    //empty
    for (int i = 0; i <= SIZE * SIZE; i++) {
        if (board[i / 4][i % 4] == 0) {
            return 0;
        }
    }

    //check the neighbors are same or not in colounm and row
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if(i<3){
                if (board[i][j] == board[i + 1][j]) {
                    return 0;
                }
            }
            if(j<3){
                if (board[i][j] == board[i][j + 1]) {
                    return 0;
                }
            }
        }
    }

    return 2;   //fail

}