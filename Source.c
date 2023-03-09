#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>

// Define const
#define lenght 30
#define ifInull if (i == 0)
#define ifJnull if (j == 0)
#define ifInine if ( i == 29)
#define ifJnine if (j == 29)

// Display struct to send instance from SDL struct
typedef struct Display {

    SDL_Window* fenetre;
    SDL_Renderer* renderer;
    SDL_Rect cases[lenght][lenght];

} Display;

// State struct bollean using 
// except zero pos that store X,Y zero pos, they wait to be discover in auto discover function 
typedef struct State {
    int isBomb;
    int isFlag;
    int isSafe;
    int isDiscover;
    int zeroPosX;
    int zeroPosY;
} State;


// Game struct, hide board store data and we store them in game board when we want show them
struct Game {
    char game[lenght][lenght];
    char hide[lenght][lenght];
};


// Init function
//void showBoard(char board[lenght][lenght]);
void SDLShowBoard(char board[lenght][lenght], Display* display);
void randPos(int pRow, int pCol, int* randRow, int* randCol);
void placeBomb(char bomb[lenght][lenght], State state[lenght][lenght]);
int aroundCheck(char board[lenght][lenght], int i, int j, char charCheck);
int aroundBomb(char board[lenght][lenght], int i, int j);
int wherePlay(int* rowInput, int* colInput, int* beginReveal);
void aroundDiscover(int i, int j, State[lenght][lenght], char game[lenght][lenght], char hide[lenght][lenght], Display* display);
void autoDiscover(char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], Display* display);
int boxDiscover(int rowInput, int colInput, char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], Display* display);
int playing(char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], int* beginReveal, Display* display);
void setCase(int posRow, int posCol, struct State state[lenght][lenght], char game[lenght][lenght], char hide[lenght][lenght]);
void appendFunc(struct State state[lenght][lenght], int posRow, int posCol);
void firstDiscover(int posI, int posJ, char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], int* beginReveal);
int playing(char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], int* beginReveal,Display* display);
//void placeFlag(State state[lenght][lenght], char game[lenght][lenght]);
void revealAtBomb(char game[lenght][lenght], char hide[lenght][lenght],Display *display);



void SDLShowOneCase(int i, int j, char board[lenght][lenght], Display* display) {

    // show one case

    SDL_Surface* bmp = SDL_LoadBMP("./image.bmp");

    switch (board[i][j]) {
    case '0':
        bmp = SDL_LoadBMP("./LEMUR.bmp");
        break;
    case '1':
        bmp = SDL_LoadBMP("./LE1.bmp");
        break;
    case '2':
        bmp = SDL_LoadBMP("./LE2.bmp");
        break;
    case '3':
        bmp = SDL_LoadBMP("./LE3.bmp");
        break;
    case '4':
        bmp = SDL_LoadBMP("./LE4.bmp");
        break;
    case 'o':
        bmp = SDL_LoadBMP("./LEBOMB.bmp");
        break;
    case 'F':
        bmp = SDL_LoadBMP("./LEFLAG.bmp");
        break;
    }

    SDL_Texture* image = SDL_CreateTextureFromSurface(display->renderer, bmp);
    SDL_RenderCopy(display->renderer, image, NULL, &display->cases[i][j]);
    SDL_RenderPresent(display->renderer);

}



void SDLShowBoard(char board[lenght][lenght], Display* display) {
    SDL_RenderClear(display->renderer);

    int i = 0;
    int j = 0;
    int posY = 0;
    int caseSize = 800 / lenght;

    for (i = 0; i < lenght; i++) {
        for (j = 0; j < lenght; j++) {
            display->cases[i][j].x = (j == 0) ? 0 : display->cases[i][j - 1].x + caseSize;
            display->cases[i][j].y = posY;
            display->cases[i][j].w = display->cases[i][j].h = caseSize;
            int modulo_i = (i % 2);
            int modulo_j = (j + modulo_i) % 2;
            if (modulo_j == 0) {
                SDL_SetRenderDrawColor(display->renderer, 27, 105, 206, 255);
            }
            else {
                SDL_SetRenderDrawColor(display->renderer, 27, 173, 209, 255);
                
            }

            SDL_Surface* bmp = SDL_LoadBMP("./image.bmp");
            switch (board[i][j]) {
                case '0':
                    bmp = SDL_LoadBMP("./LEMUR.bmp");
                    break;
                case '1':
                    bmp = SDL_LoadBMP("./LE1.bmp");
                    break;
                case '2':
                    bmp = SDL_LoadBMP("./LE2.bmp");
                    break;
                case '3':
                    bmp = SDL_LoadBMP("./LE3.bmp");
                    break;
                case '4':
                    bmp = SDL_LoadBMP("./LE4.bmp");
                    break;
                case 'o':
                    bmp = SDL_LoadBMP("./LEBOMB.bmp");
                    break;
                case 'F':
                    bmp = SDL_LoadBMP("./LEFLAG.bmp");
                    break;
            }
            SDL_RenderFillRect(display->renderer, &display->cases[i][j]);
            SDL_Texture* image = SDL_CreateTextureFromSurface(display->renderer, bmp);
            SDL_RenderCopy(display->renderer, image, NULL, &display->cases[i][j]);

        }
        posY += caseSize;
    }
    SDL_RenderPresent(display->renderer);

}

//void showBoard(char board[lenght][lenght]) {
//    printf("%s", "    ");
//    int i = 0;
//    int j = 0;
//    for (int k = 0; k < lenght; k++) {
//        if (k >= 10 && k < 20) {
//            printf("|%d|", k - 10);
//        }
//        else if (k >= 20) {
//            printf("|%d|", k - 20);
//        }
//        else {
//            printf("|%d|", k);
//        }
//    }
//    printf("\n\n");
//    for (i = 0; i < lenght; i++) {
//        if (i >= 10 && i < 20) {
//            printf("|%d| ", i - 10);
//        }
//        else if (i >= 20) {
//            printf("|%d| ", i - 20);
//        }
//        else {
//            printf("|%d| ", i);
//        }
//        for (j = 0; j < lenght; j++) {
//            if (board[i][j] == '0') {
//                printf("\033[0;15m");
//                printf("|%c|", ' ');
//            }
//            else if (board[i][j] == 'o') {
//                printf("\033[0;35m");
//                printf("|%c|", 'o');
//            }
//            else if (board[i][j] == '1') {
//                printf("\033[0;32m");
//                printf("|%c|", '1');
//            }
//            else if (board[i][j] == '2') {
//                printf("\033[0;33m");
//                printf("|%c|", '2');
//            }
//            else if (board[i][j] == '3') {
//                printf("\033[0;31m");
//                printf("|%c|", '3');
//            }
//            else if (board[i][j] == '4') {
//                printf("\033[0;31m");
//                printf("|%c|", '4');
//            }
//            else if (board[i][j] == '?') {
//                printf("\033[0;15m");
//                printf("|%c|", '?');
//            }
//            else {
//                printf("\033[0;34m");
//                printf("|%c|", board[i][j]);
//            }
//        }
//        printf("\n");
//    }
//}

// generate random pos
void randPos(int pRow, int pCol, int* randRow, int* randCol) {
    *randRow = rand() % pRow;
    *randCol = rand() % pCol;
}

// for each exec of the func -> generate and set value to bomb pos if not safe zone -> optimized
void placeBomb(char bomb[lenght][lenght], State state[lenght][lenght]) {
    int pRow = lenght;
    int pCol = lenght;
    int randRow = 0;
    int randCol = 0;

    randPos(pRow, pCol, &randRow, &randCol);

    if (bomb[randRow][randCol] == 'o' || state[randRow][randCol].isSafe) {
        randPos(pRow, pCol, &randRow, &randCol);
    }
    else {
        state[randRow][randCol].isBomb = 1;
        bomb[randRow][randCol] = 'o';
    }
}

// set number of bomb around the case and not exeed the zone
int aroundCheck(char board[lenght][lenght], int i, int j, char charCheck) {
    int nbrChar = 0;
    if (board[i - 1][j - 1] == charCheck && i - 1 >= 0 && j - 1 >= 0) {
        nbrChar += 1;
    }
    if (board[i + 1][j + 1] == charCheck && i + 1 != lenght && j + 1 != lenght) {
        nbrChar += 1;
    }
    if (board[i - 1][j + 1] == charCheck && i - 1 >= 0 && j + 1 != lenght) {
        nbrChar += 1;
    }
    if (board[i + 1][j - 1] == charCheck && i + 1 != lenght && j - 1 >= 0) {
        nbrChar += 1;
    }
    if (board[i + 1][j] == charCheck && i + 1 != lenght) {
        nbrChar += 1;
    }
    if (board[i - 1][j] == charCheck && i - 1 >= 0) {
        nbrChar += 1;
    }
    if (board[i][j + 1] == charCheck && j + 1 != lenght) {
        nbrChar += 1;
    }
    if (board[i][j - 1] == charCheck && j - 1 >= 0) {
        nbrChar += 1;
    }
    return nbrChar;
}


// set and return corresponding number for each case to know number of bomb around
int aroundBomb(char board[lenght][lenght], int i, int j) {
    int nbrBomb = 0;
    char charCheck = 'o';
    if (board[i][j] != 'o') {
        nbrBomb = aroundCheck(board, i, j, charCheck);
        char charNbrBomb = nbrBomb + '0';
        return charNbrBomb;
    }
    return 'a';
}



int main() {

    struct Game structGame;
    struct State state[lenght][lenght];
    Display display;
    int windowLenght = 800;
    int windowWitdh = 800;
    int i, j = 0;
    srand(time(NULL));

    for (i = 0; i < lenght; i++) {
        for (j = 0; j < lenght; j++) {
            state[i][j].isBomb = 0;
            state[i][j].isFlag = 0;
            state[i][j].isDiscover = 0;
            state[i][j].isSafe = 0;
            structGame.game[i][j] = '?';
            structGame.hide[i][j] = '0';
        }
    }

    if (SDL_VideoInit(NULL) < 0)
    {
        printf("Erreur d'initialisation de la SDL : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    display.fenetre = SDL_CreateWindow("Une fenetre SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowLenght, windowWitdh, 0);
    if (display.fenetre == NULL)
    {
        printf("Erreur lors de la creation d'une fenetre : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    display.renderer = SDL_CreateRenderer(display.fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (display.renderer == NULL)
    {
        printf("Erreur lors de la creation d'un renderer : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_SetRenderDrawColor(display.renderer, 255, 255, 255, 255);
    SDL_RenderClear(display.renderer);
    SDLShowBoard(structGame.game, &display);
    SDL_RenderPresent(display.renderer);


    // game loop -> when game over or win freeze the playing but the event can continue

    int gameOver = 0;
    int beginReveal = 0;
    do {
        if (gameOver != 1) {
            gameOver = playing(structGame.game, structGame.hide, state, &beginReveal, &display);
            beginReveal = 1;
        }
    } while (1);
    return 0;
}


// wait player choice between flag and case
int wherePlay(int* clickMouseX, int* clickMouseY, int* beginReveal) {
    int clickState = 0;
    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    clickState = 1;
                    *clickMouseX = event.button.x;
                    *clickMouseY = event.button.y;
                    return clickState;
                }
                if (*beginReveal == 1) {
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        clickState = 2;
                        *clickMouseX = event.button.x;
                        *clickMouseY = event.button.y;
                        return clickState;
                    }
                }
            }
        }
    }

    /*int error = 0;
    do {
        printf("Sur quel ligne voulez-vous jouer ?\n");
        error = scanf_s("%d", rowInput);
        if (error != 0 && *rowInput < lenght) {
            printf("%s %d\n", "Vous avez choisi de jouer a la ligne", *rowInput);
        }
        char c = getchar();
        while (c != '\n') {
            c = getchar();
        };
    } while (error == 0 || *rowInput > lenght);
    error = 0;
    do {
        printf("Sur quel colonne voulez-vous jouer ?\n");
        error = scanf_s("%d", colInput);
        if (error != 0 && *colInput < lenght) {
            printf("%s %d\n", "Vous avez choisi de jouer a la colonne", *colInput);
        }
        char c = getchar();
        while (c != '\n') {
            c = getchar();
        };
    } while (error == 0 || *colInput > lenght);*/
}

// append array that contain the future zero to discover in loop, with an execution order
void appendFunc(struct State state[lenght][lenght], int posRow, int posCol) { 
    for (int i = 0; i < lenght; i++) {
        for (int j = 0; j < lenght; j++) {
            if (state[i][j].zeroPosX == 10000) {
                state[i][j].zeroPosX = posRow;
                state[i][j].zeroPosY = posCol;
                return;
            }
        }
    }
}

// discover a box in hide game array to show it after, this func is used for auto discover system 
void setCase(int posRow, int posCol, struct State state[lenght][lenght], char game[lenght][lenght], char hide[lenght][lenght]) {
    if (state[posRow][posCol].isBomb != 1 && state[posRow][posCol].isDiscover != 1) {
        if (hide[posRow][posCol] == '0') {
            appendFunc(state, posRow, posCol);
        }
        state[posRow][posCol].isDiscover = 1;
        game[posRow][posCol] = hide[posRow][posCol];
    }
}


// discover the 8 box around a case, this func is used for auto discover system 
void aroundDiscover(int i, int j, struct State state[lenght][lenght], char game[lenght][lenght], char hide[lenght][lenght], Display* display) {

    int ij = 0;
    int posRow = 0;
    int posCol = 0;

    ifInull{
      posRow = i;
    }
    else {
    posRow = i - 1;
        }
    ifJnull{
        posCol = j;
    }
    else {
    posCol = j - 1;
    }
    setCase(posRow, posCol, state, game, hide);
    SDLShowOneCase(posRow, posCol, game, display);

    ifInine{
        posRow = i;
    }
    else {
    posRow = i + 1;
    }
    ifJnine{
        posCol = j;
    }
    else {
    posCol = j + 1;
    }
    setCase(posRow, posCol, state, game, hide);
    SDLShowOneCase(posRow, posCol, game, display);

    ifInull{
    posRow = i;
    }
    else {
        posRow = i - 1;
    }
    ifJnine{
    posCol = j;
    }
    else {
        posCol = j + 1;
    }
    setCase(posRow, posCol, state, game, hide);
    SDLShowOneCase(posRow, posCol, game, display);

    ifInine{
    posRow = i;
    }
    else {
        posRow = i + 1;
    }
    ifJnull{
    posCol = j;
    }
    else {
        posCol = j - 1;
    }
    ij = (posRow) * 10 + (posCol);
    setCase(posRow, posCol, state, game, hide);
    SDLShowOneCase(posRow, posCol, game, display);

    posRow = i;
    ifJnull{
    posCol = j;
    }
    else {
        posCol = j - 1;
    }
    setCase(posRow, posCol, state, game, hide);
    SDLShowOneCase(posRow, posCol, game, display);

    posRow = i;
    ifJnine{
    posCol = j;
    }
    else {
        posCol = j + 1;
    }
    setCase(posRow, posCol, state, game, hide);
    SDLShowOneCase(posRow, posCol, game, display);

    ifInull{
    posRow = i;
    }
    else {
        posRow = i - 1;
    }
    posCol = j;
    setCase(posRow, posCol, state, game, hide);
    SDLShowOneCase(posRow, posCol, game, display);

    ifInine{
    posRow = i;
    }
    else {
        posRow = i + 1;
    }
    posCol = j;
    setCase(posRow, posCol, state, game, hide);
    SDLShowOneCase(posRow, posCol, game, display);
}


// auto discover loop that execute in order the array of zero and set it no effective after discover
void autoDiscover(char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], Display* display) {
    for (int k = 0; k < lenght; k++) {
        for (int l = 0; l < lenght; l++) {
            if (state[k][l].zeroPosX != 10000 && state[k][l].zeroPosX != 20000) {
                int intPosX = state[k][l].zeroPosX;
                int intPosY = state[k][l].zeroPosY;
                aroundDiscover(intPosX, intPosY, state, game, hide, display);
                state[k][l].zeroPosX = 20000;
                autoDiscover(game, hide, state, display);
            }
        }
    }

}

// check the box that we try to discover after a click 
int boxDiscover(int rowInput, int colInput, char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], Display* display) {

    int i = 0;
    int j = 0;
    for (i = 0; i < lenght; i++) {
        for (j = 0; j < lenght; j++) {
            state[i][j].zeroPosX = 10000;
        }
    }
    char playerChoice = hide[rowInput][colInput];
    if (playerChoice == '0') {
        state[rowInput][colInput].isDiscover = 1;
        game[rowInput][colInput] = playerChoice;
        SDLShowOneCase(rowInput,colInput,game, display);
        state[rowInput][colInput].zeroPosX = rowInput;
        state[rowInput][colInput].zeroPosY = colInput;
        autoDiscover(game, hide, state, display);
    }
    else if (playerChoice == 'o') {
        return 1;
    }
    else {
        state[rowInput][colInput].isDiscover = 1;
        game[rowInput][colInput] = playerChoice;
    }
    return 0;
}

// first discover -> this func is used to init the safe zone 
void firstDiscover(int posI, int posJ, char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], int* beginReveal) {
    if (posI < 0 || posI >= lenght || posJ < 0 || posJ >= lenght) {
        return;
    }
    state[posI][posJ].isSafe = 1;
    game[posI][posJ] = hide[posI][posJ];
}

void placeFlag(State state[lenght][lenght], char game[lenght][lenght]) {
    //    int yesOrNo = 0;
    //    int rowInput = 0;
    //    int colInput = 0;
    //
    //    printf("%s\n", "Voulez-vous placer un flag ? (0 = non, 1 = oui)");
    //    scanf_s("%d", &yesOrNo);
    //
    //    if (yesOrNo == 1) {
    //        int error = 0;
    //        do {
    //            printf("%s\n", "Sur quel ligne voulez-vous placer le flag ?");
    //            error = scanf_s("%d", &rowInput);
    //            if (error != 0 && rowInput < lenght) {
    //                printf("%s %d\n", "Vous avez choisi de jouer a la ligne", rowInput);
    //            }
    //            char c = getchar();
    //            while (c != '\n') {
    //                c = getchar();
    //            };
    //        } while (error == 0 || rowInput > lenght);
    //        error = 0;
    //        do {
    //            printf("%s\n", "Sur quel colonne voulez-vous placer le flag ?");
    //            error = scanf_s("%d", &colInput);
    //            if (error != 0 && colInput < lenght) {
    //                printf("%s %d\n", "Vous avez choisi de jouer a la colonne", colInput);
    //            }
    //            char c = getchar();
    //            while (c != '\n') {
    //                c = getchar();
    //            };
    //        } while (error == 0 || colInput > lenght);
    //        if (game[rowInput][colInput] == '?') {
    //            game[rowInput][colInput] = 'F';
    //            state[rowInput][colInput].isFlag = 1;
    //            showBoard(game);
    //        }
    //        else {
    //            printf("%s\n", "il y a quelque chose ici !");
    //            placeFlag(state, game);
    //        }
    //    }
    //    yesOrNo = 0;
    //    printf("%s\n", "Voulez-vous retirer un flag ? (0 = non, 1 = oui)");
    //    scanf_s("%d", &yesOrNo);
    //    if (yesOrNo == 1) {
    //        int error = 0;
    //        do {
    //            printf("%s\n", "Sur quel ligne voulez-vous retirer le flag ?");
    //            error = scanf_s("%d", &rowInput);
    //            if (error != 0 && rowInput < lenght) {
    //                printf("%s %d\n", "Vous avez choisi de jouer a la ligne", rowInput);
    //            }
    //            char c = getchar();
    //            while (c != '\n') {
    //                c = getchar();
    //            };
    //        } while (error == 0 || rowInput > lenght);
    //        error = 0;
    //        do {
    //            printf("%s\n", "Sur quel colonne voulez-vous retirer le flag ?");
    //            error = scanf_s("%d", &colInput);
    //            if (error != 0 && colInput < lenght) {
    //                printf("%s %d\n", "Vous avez choisi de jouer a la colonne", colInput);
    //            }
    //            char c = getchar();
    //            while (c != '\n') {
    //                c = getchar();
    //            };
    //        } while (error == 0 || colInput > lenght);
    //
    //
    //        if (game[rowInput][colInput] == 'F') {
    //            game[rowInput][colInput] = '?';
    //            state[rowInput][colInput].isFlag = 0;
    //            showBoard(game);
    //        }
    //        else {
    //            printf("%s\n", "il y a deja un flag ou quelque chose ici");
    //            placeFlag(state, game);
    //        }
    //    }
}


// reveal bomb


// reveal all bomb with anim when game over
void revealAtBomb(char game[lenght][lenght], char hide[lenght][lenght],Display* display) {
    for (int i = 0; i < lenght; i++) {
        for (int j = 0; j < lenght; j++) {
            if (hide[i][j] == 'o') {
                game[i][j] = hide[i][j];
                SDLShowOneCase(i, j, game, display);
            }
        }
    }

}

// game loop call it, it's the source of the input to play
int playing(char game[lenght][lenght], char hide[lenght][lenght], State state[lenght][lenght], int* beginReveal, Display* display) {

    int mouseClickX = 0;
    int mouseClickY = 0;
    int nbrBomb = 150;
    int rowInput;
    int colInput;

    SDLShowBoard(game, display);



    // ask choice
    int choiceState = wherePlay(&mouseClickX, &mouseClickY, beginReveal);


    // check if choice is flag or case 
    if (choiceState == 2) {
        rowInput = (mouseClickY - (mouseClickY % 26)) / 26;
        colInput = (mouseClickX - (mouseClickX % 26)) / 26;
        if (game[rowInput][colInput] == '?') {
            game[rowInput][colInput] = 'F';
            state[rowInput][colInput].isFlag = 1;
            SDLShowBoard(game, display);
            return 0;
        }
        if (game[rowInput][colInput] == 'F') {
            game[rowInput][colInput] = '?';
            state[rowInput][colInput].isFlag = 0;
            SDLShowBoard(game, display);
            return 0;
        }
    }
    else {
        rowInput = (mouseClickY - (mouseClickY % 26)) / 26;
        colInput = (mouseClickX - (mouseClickX % 26)) / 26;
        if (game[rowInput][colInput] != '?') {
            printf("%s\n", "cette case est deja prise");
        }


        // check if this is the first click time 
        if (*beginReveal == 0) { // if first reveal init safe zone 
            int posI = rowInput;
            int posJ = colInput;
            firstDiscover(posI, posJ, game, hide, state, beginReveal); // discover the case and make itself safe to avoid place bomb on 
            posI = rowInput + 1;
            posJ = colInput + 1;
            firstDiscover(posI, posJ, game, hide, state, beginReveal);
            posI = rowInput - 1;
            posJ = colInput - 1;
            firstDiscover(posI, posJ, game, hide, state, beginReveal);
            posI = rowInput + 1;
            posJ = colInput - 1;
            firstDiscover(posI, posJ, game, hide, state, beginReveal);
            posI = rowInput - 1;
            posJ = colInput + 1;
            firstDiscover(posI, posJ, game, hide, state, beginReveal);
            posI = rowInput;
            posJ = colInput - 1;
            firstDiscover(posI, posJ, game, hide, state, beginReveal);
            posI = rowInput;
            posJ = colInput + 1;
            firstDiscover(posI, posJ, game, hide, state, beginReveal);
            posI = rowInput - 1;
            posJ = colInput;
            firstDiscover(posI, posJ, game, hide, state, beginReveal);
            game[posI][posJ] = hide[rowInput + 1][colInput];
            posI = rowInput + 1;
            posJ = colInput;
            firstDiscover(posI, posJ, game, hide, state, beginReveal);


            for (int i = 0; i < nbrBomb; i++) { // place bomb loop
                placeBomb(hide, state);
            }

            for (int i = 0; i < lenght; i++) { // place number loop
                for (int j = 0; j < lenght; j++) {
                    char nbrBomb = aroundBomb(hide, i, j);
                    if (nbrBomb != 'a') {
                        hide[i][j] = nbrBomb;
                    }
                }
            }

            boxDiscover(rowInput, colInput, game, hide, state, display); // discover the main 0 and the auto discover func make the rest
            return 0;
            
        }
        else {  // check win and game over with the great time 
            int isWin = 1;
            if (boxDiscover(rowInput, colInput, game, hide, state, display)) {
                revealAtBomb(game, hide,display);
                printf("\n%s\n", "game over");
                return 1;
            }
            else {
                for (int i = 0; i < lenght; i++) {
                    for (int j = 0; j < lenght; j++) {
                        if (game[i][j] == '?') {
                            isWin = 0;
                            return 0;
                        }
                    }
                }
            }

            if (isWin == 1) {
                for (int i = 0; i < lenght; i++) {
                    for (int j = 0; j < lenght; j++) {
                        game[i][j] = hide[i][j];
                    }
                }
                SDLShowBoard(game, display);
                printf("\n%s\n", "WIN");
                return 1;
            }
            return 0;
        }
    }
}





























