#include <stdio.h>

#ifndef ROWS
#define ROWS 10
#endif

#ifndef COLS
#define COLS 4
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

/* Game Logic / State Check */
int isColumnFull(char[][COLS], int, int, int);
int isBoardFull(char[][COLS], int, int);
int isInBounds(int, int, int, int);
int getColumnHeight(char[][COLS], int, int);
int checkIfPossibleToPutInAColumn(char board[][COLS], int rows, int cols, int col);
int checkIfNumSequenceForPlayer(char playerToken, char board[][COLS], int rows, int cols, int inSequenceNum);
int checkIfNumSequenceForPlayerBecauseOfLastMove(char playerToken, char board[][COLS], int rows, int cols, int inSequenceNum, int lastMoveRow, int lastMoveCol);

/* Player Input / Type */
int getPlayerType(int);
int requestHumanInput(char board[][COLS], int rows, int cols);
int playPlayerQueary(char board[][COLS], int rows, int cols, int IndexChoiseArray[COLS], int numPlayer, int playerType);

/* Board Management */
void initBoard(char[][COLS], int, int);
void printBoard(char[][COLS], int, int);
int insertToken(char [][COLS], int, int, char, int);
int uninsertToken(char [][COLS], int, int, int);

/* Computer AI */
void setIndexMap(int array[COLS], int cols);
int checkPlayerForPossibleSequence(char [][COLS], int, int, char, int, int[COLS]);
int generateComputerPlayerMove(char [][COLS], int, int, char, char, int[COLS]);

/* Main Execution */
void runConnectFour(char[][COLS], int, int, int, int);
int main();


int main() {
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}

void initBoard(char board[][COLS] , int rows, int cols){
    for (int row = 0; row < rows; row++)
        for (int col = 0;  col < cols; col++)
            //SET BORAD POSITION AS EMPTY
            board[row][col] = EMPTY;
}

int checkIfNumSequenceForPlayer(char playerToken, char board[][COLS], int rows, int cols, int inSequenceNum){
 /**
     * @brief Checks if a specific player has achieved a winning sequence on the board.
     * * @param playerToken The character token of the player to check (TOKEN_P1 or TOKEN_P2).
     * @param board The current state of the game board.
     * @param rows The number of rows in the board.
     * @param cols The number of columns in the board.
     * @param inSequenceNum The required length of the sequence for a win (e.g., 4 for Connect Four).
     * @return int Returns 1 if the player has 'inSequenceNum' tokens consecutively 
     * (horizontally, vertically, or diagonally). Returns 0 otherwise.
     */

    if (inSequenceNum < 2)
        return 0; //No Sequence if Below 2
        
    // Start of the function body
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            //if not this player skip to the next position
            if (board[row][col] != playerToken)
                continue;
            
            //check east (row stays the same colum changes)
            if (cols - col > inSequenceNum - 1){
                int flag = 1;
                for (int shift = 1; shift < inSequenceNum; shift++){
                    if (board[row][col + shift] != playerToken){
                        flag = 0;
                        break;
                    }
                }
                if (flag){
                    return 1;
                }
            }
            //check south east
            if ((cols - col > inSequenceNum - 1) && (rows - row > inSequenceNum - 1)){
                int flag = 1;
                for (int shift = 1; shift < inSequenceNum; shift++){
                    if (board[row + shift][col + shift] != playerToken){
                        flag = 0;
                        break;
                    }
                }
                if (flag){
                    return 1;
                }
            }
            //check south
            if (rows - row > inSequenceNum - 1){
                int flag = 1;
                for (int shift = 1; shift < inSequenceNum ; shift++){
                    if (board[row + shift][col] != playerToken){
                        flag = 0;
                        break;
                    }
                }
                if (flag){
                    return 1;
                }
            }

            //check south west
            if ((col >= inSequenceNum - 1) && (rows - row > inSequenceNum - 1)){
                int flag = 1;
                for (int shift = 1; shift < inSequenceNum; shift++){
                    if (board[row + shift][col - shift] != playerToken){
                        flag = 0;
                        break;
                    }
                }
                if (flag){
                    return 1;
                }
            }
        }
    }
    
    // End of function body - must return a value if no sequence is found
    return 0;
}


int checkIfNumSequenceForPlayerBecauseOfLastMove(char playerToken, char board[][COLS], int rows, int cols, int inSequenceNum, int lastMoveRow, int lastMoveCol){
 /**
     * @brief Checks if a specific player has achieved a winning sequence on the board.
     * * @param playerToken The character token of the player to check (TOKEN_P1 or TOKEN_P2).
     * @param board The current state of the game board.
     * @param rows The number of rows in the board.
     * @param cols The number of columns in the board.
     * @param inSequenceNum The required length of the sequence (e.g., 4 for Connect Four).
     * @return int Returns 1 if the player has 'inSequenceNum' tokens consecutively 
     * (horizontally, vertically, or diagonally). Returns 0 otherwise.
     */

    if (inSequenceNum < 2)
        return 0; //No Sequence if Below 2
    

    // Start of the function body
    for (int row = 0; row < rows; row++) {
        for (int col = cols - 1; col >= 0; col--) {
            //if not this player skip to the next position
            int counter = 0;

            if (board[row][col] != playerToken)
                continue;

            if ((row == lastMoveRow) && (col == lastMoveCol))
                counter++;
            
          
            //check south east
            if ((cols - col > inSequenceNum - 1) && (rows - row > inSequenceNum - 1)){
                int flag = 1;
                int innerCount = 0;
                for (int shift = 1; shift < inSequenceNum; shift++){
                    if ((row + shift == lastMoveRow) && (col + shift == lastMoveCol))
                        innerCount++;
                    if (board[row + shift][col + shift] != playerToken){
                        flag = 0;
                        break;
                    }
                }
                if (flag && ((counter + innerCount) == 1)){
                    return 1;
                }
            }
            //check south
            if (rows - row > inSequenceNum - 1){
                int flag = 1;
                int innerCount = 0;
                for (int shift = 1; shift < inSequenceNum ; shift++){
                    if ((row + shift == lastMoveRow) && (col == lastMoveCol))
                        innerCount++;
                    if (board[row + shift][col] != playerToken){
                        flag = 0;
                        break;
                    }
                }
                if (flag && ((counter + innerCount) == 1)){
                    return 1;
                }
            }

            //check south west
            if ((col >= inSequenceNum - 1) && (rows - row > inSequenceNum - 1)){
                int flag = 1;
                int innerCount = 0;
                for (int shift = 1; shift < inSequenceNum; shift++){
                    if ((row + shift == lastMoveRow) && (col - shift == lastMoveCol))
                        innerCount++;

                    if (board[row + shift][col - shift] != playerToken){
                        flag = 0;
                        break;
                    }
                }
                if (flag && ((counter + innerCount) == 1)){
                    return 1;
                }
            }

            //check east (row stays the same colum changes)
            if (cols - col > inSequenceNum - 1){
                int flag = 1;
                int innerCount = 0;
                for (int shift = 1; shift < inSequenceNum; shift++){
                    if ((row == lastMoveRow) && (col + shift == lastMoveCol))
                        innerCount++;
                    if (board[row][col + shift] != playerToken){
                        flag = 0;
                        break;
                    }
                }
                if (flag && ((counter + innerCount) == 1)){
                    return 1;
                }
            }
        }
    }
    
    // End of function body - must return a value if no sequence is found
    return 0;
}

int insertToken(char board[][COLS], int rows, int cols, char playerToken, int insertCol){
    //The functions inserts a playerToken to a selected column, if player inserted an invalid column (insertCol)
    //then return 0
    //if column is full,  the action cannot happen, returns 0
    //if action is successfull return 1

    if ((insertCol < 0) || (insertCol > cols - 1))
        //invalid insertCol Value
        return 0;
    
    //check if the column is full
    //if the row full return 0
    if (board[0][insertCol] != EMPTY)
        return 0;

    //find the first avaliable place for the token
    int row;
    for (row = rows - 1; row > -1; row--){
        if (board[row][insertCol] == EMPTY)
            break;
    }
    
    //insert token
    board[row][insertCol] = playerToken;

    //return 1 to insert the token
    return 1;

}

int uninsertToken(char board[][COLS], int rows, int cols, int insertCol){
    //get the last token inserted in a column
    //cehck if the column is valid if not return 0
    //if the column is empty return 0, action failed
    //if the action succeeded return 1
    if ((insertCol < 0) || (insertCol > cols - 1))
        //invalid insertCol Value
        return 0;

    //check if the column is full
    //if the row full return 0
    const int lastRowIndex = rows - 1;
    if (board[lastRowIndex][insertCol] == EMPTY)
        return 0;
    
    //now remove the last token in a column
    int row = 0;
    do {
        if (board[row][insertCol] != EMPTY)
            break;
        row++;
    }while (row < rows);

    board[row][insertCol] = EMPTY;

    return 1;
}


int checkPlayerForPossibleSequence(char board [][COLS], int rows, int cols, char playerToken, int sequenceNum, int indexMap[COLS]){
    //in each column we will insert the next move, 
    //and for each updated position we will check wether the player has a sequence of sequenceNum.
    //if he has the sequence return the move that created the sequence
    //else return -1, no move will create a sequence of sequenceNum
    int isInserted, isSequence;
    for (int col = 0; col < cols; col++){
        int colToCheckFirst = indexMap[col];
        isInserted = insertToken(board, rows, cols, playerToken, colToCheckFirst);

        if (!isInserted){
            //sequence is not possible at this column
            continue;
        }
        
        //get row index of the inserted item
        int columnHeight = getColumnHeight(board, rows, colToCheckFirst);
        int rowIndexInsert = rows - columnHeight;

        //if we succeeded making the move check for a sequence of 3
        isSequence = checkIfNumSequenceForPlayerBecauseOfLastMove(playerToken, board, rows, cols, sequenceNum, rowIndexInsert, colToCheckFirst);
        uninsertToken(board, rows, cols, colToCheckFirst);
        if (isSequence){
            return colToCheckFirst;
        }

    }
    return -1;
}

int generateComputerPlayerMove(char board [][COLS], int rows, int cols, char playerToken, char opposingPlayerToken, int whatColsToCheckFirst[COLS]){

    //WE ASSUME THE BOARD IS NOT FULL WHEN USING THIS FUNCTION!!!!

    // Priority order
    // 1. Winning move - if it is possible to win on the next move - choose the column that produces the win.
    // 2. Blocking the opponent - if the opponent can win on their next move choose the column that prevents this.
    // 3. Creating a sequence of three - if it is possible to create a sequence of three tokens do so.
    // 4. Blocking the opponent’s sequence of three.
    // 5. Choosing according to the arbitrary ordering rule.

    // Ordering rule for columns
    // When several options have the same priority level, choose a column according to the following rules:
    // 1. Prefer the column whose distance from the center column is minimal.
    // 2. If the distance is equal, choose the left column among the two.

    //1. if it is possible to win on the next move - choose the column that produces the win.
    int winningMove = checkPlayerForPossibleSequence(board, rows, cols, playerToken, CONNECT_N, whatColsToCheckFirst);
    if (winningMove != -1){
        return winningMove;
    }
    //2. if the opponent can win on their next move choose the column that prevents this.
    int opponentWinningMove = checkPlayerForPossibleSequence(board, rows, cols, opposingPlayerToken, CONNECT_N, whatColsToCheckFirst);
    if (opponentWinningMove != -1){
        return opponentWinningMove;
    }

    //3. if it is possible to create a sequence of three tokens do so.
    int moveForASequenceOf3 = checkPlayerForPossibleSequence(board, rows, cols, playerToken, 3, whatColsToCheckFirst);
    if (moveForASequenceOf3 != -1){
        return moveForASequenceOf3;
    }

    //4. Blocking the opponent’s sequence of three
    int OpponentmoveForASequenceOf3 = checkPlayerForPossibleSequence(board, rows, cols, opposingPlayerToken, 3, whatColsToCheckFirst);
    if (OpponentmoveForASequenceOf3 != -1){
        return OpponentmoveForASequenceOf3;
    }

    // Ordering rule for columns
    // When several options have the same priority level, choose a column according to the following rules:
    // 1. Prefer the column whose distance from the center column is minimal.
    // 2. If the distance is equal, choose the left column among the two.

    for (int i = 0; i < cols; i++){
        int column = whatColsToCheckFirst[i];
        if(checkIfPossibleToPutInAColumn(board, rows, cols, column)){
            return column;
        }
    }

    //never gets here but the compiler cries...

    return -1;
}

int getColumnHeight(char board[][COLS], int rows, int col) {
    if (board[0][col] != EMPTY)
        return rows;

    for (int i = 0; i < rows; i++){
        if (board[i][col] != EMPTY){
            return rows - i;
        }
    }
    return 0;

}

int isBoardFull(char board[][COLS], int rows, int cols){
    for (int col = 0; col < cols; col++)
        for (int row = 0; row < rows; row++)
            if (board[row][col] == EMPTY)
                return 0;
    
    return 1;
}

int checkIfPossibleToPutInAColumn(char board[][COLS], int rows, int cols, int col){
    //we assume the column is valid
    const char dummyPlayerToken = TOKEN_P1;
    if (insertToken(board, rows, cols, dummyPlayerToken, col)){
        uninsertToken(board, rows, cols, col);
        return 1;
    }
    return 0;
}


int requestHumanInput(char board[][COLS], int rows, int cols) {
    int userInput;
    int validInput = 0;
    int isReadSuccessful;
    int column;

    // Use a while loop that continues until valid input is received
    while (!validInput) {
        printf("Enter column (1-%d). \n", COLS);

        isReadSuccessful = scanf(" %d", &userInput);

        if (!isReadSuccessful) {
            printf("Invalid value. Enter a number.");
            while (getchar() != '\n'); //clearBuffer
        } 
        else {
            column = userInput - 1;
            if (!(column > - 1 && column < cols)){
                printf("Invalid column. try again.");
                continue;
            }
            int columnNotFull = checkIfPossibleToPutInAColumn(board, rows, cols, column);
            if (!columnNotFull){
                printf("Column %d is full. Choose another column", column);
                continue;
            }
            validInput = 1;
        }  
    }
    return column; 
}


void setIndexMap(int array[COLS], int cols){
    int indexCounter = 0;

    //if there is a only one column 
    if (cols == 1){
        array[indexCounter] = 0;
        return;
    }

    //if the number of columns is odd
    if (cols % 2 == 1){
        int middleColIndex = cols / 2; // middle row index
        array[indexCounter] = middleColIndex;
        indexCounter++;

        for (int shift = 1; middleColIndex + shift < cols; shift++){
            int rightColumnIndex = middleColIndex + shift;
            int leftColumnIndex = middleColIndex - shift;

            array[indexCounter] = leftColumnIndex;
            indexCounter++;
            
            array[indexCounter] = rightColumnIndex;
            indexCounter++;
        }
    }
    else{
        //if the number of columns is even
        for (int shift = 0; shift < cols / 2 ; shift++){
            int rightColumnIndex = (cols / 2) + shift;
            int leftColumnIndex = (cols / 2) - shift - 1;

            array[indexCounter] = leftColumnIndex;
            indexCounter++;
            
            array[indexCounter] = rightColumnIndex;
            indexCounter++;
        }
    }
}

int playPlayerQueary(char board[][COLS], int rows, int cols, int IndexChoiseArray[COLS], int numPlayer, int playerType){
    //player one, check human or computer
    int playerMove;
    printf("Player %d (%c) turn. \n", numPlayer, numPlayer == 1 ? TOKEN_P1 : TOKEN_P2);
    if (playerType == HUMAN){
        playerMove = requestHumanInput(board, rows, cols);
    }
    else{
        playerMove = generateComputerPlayerMove(board, rows, cols, TOKEN_P1, TOKEN_P2, IndexChoiseArray);
        printf("Computer chose column %d", playerMove + 1);
    }
    return playerMove;

}

void runConnectFour(char board[][COLS], int rows, int cols, int player1Type, int player2Type){
    int IndexChoiseArray[COLS];
    int player1Won = 0, player2Won = 0;

    //initialize preIndexChoiseArray
    setIndexMap(IndexChoiseArray, cols);
    do {

        //player 1
        int movePlayer1 = playPlayerQueary(board, rows, cols, IndexChoiseArray, 1, player1Type);
        insertToken(board, rows, cols, TOKEN_P1, movePlayer1);
        printBoard(board, rows, cols);
        if (checkIfNumSequenceForPlayer(TOKEN_P1, board, rows, cols, CONNECT_N)){
            player1Won = 1;
            break;
        }
        if (isBoardFull(board, rows, cols)){
            break;
        }

        //player 2
        int movePlayer2 = playPlayerQueary(board, rows, cols, IndexChoiseArray, 2, player2Type);
        //insert move of player 2
        insertToken(board, rows, cols, TOKEN_P2, movePlayer2);
        //cehck if player 2 won
        printBoard(board, rows, cols);
        if (checkIfNumSequenceForPlayer(TOKEN_P2, board, rows, cols, CONNECT_N)){
            player2Won = 1;
            break;
        }
    } 
    while (!isBoardFull(board, rows, cols));
    //if draw no one won;
    if (player1Won){
        printf("Player 1 (%c) wins!", TOKEN_P1);
    }
    else if (player2Won)
    {
        printf("Player 2 (%c) wins!", TOKEN_P2);
    }
    else{
        printf("Board full and no winner. It's a tie!");
    }
}