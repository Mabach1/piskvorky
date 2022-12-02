#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define player_1 'X'
#define player_2 'O'

#define X 7
#define Y 7
#define WIN_LEN 4

#define SIZE (Y*X)

#define CHAR_LEN 16

#define BLANK ' '

#define ERROR   1
#define OK      0


char board[SIZE];

void fill(void) {
    for (int i = 0; i < SIZE; i++) {
        board[i] = BLANK;
    }
}
void draw_board(void) {
    int k = 0;

    printf(" ");

    for (int i = 'A'; i < (X + 'A'); i++) {
        printf(" %c ", i);
    }
    
    printf("\n");

    for (int i = 0; i < Y; i++) {
        printf("%d", i);
        for (int j = 0; j < X; j++) {
            printf("[%c]", board[k++]);
        }
        printf("\n");
    }
}
int pick(int position, char player) {
    if (board[position] != BLANK) return 1;
    
    board[position] = player;
    
    return 0;
}
int vertical_win(char player, int position) {
    int connected = 1;

    for (int i =(position + X); i <= Y; i += X) {
        if (board[i] != player) break; 
        ++connected;
    }

    for (int i = (position - X); i >= 0; i -= X) {
        if (board[i] != player) break; 
        ++connected;
    }

    if (connected == WIN_LEN) return 1;

    return 0;
}
int horizontal_win(char player, int position) {
    int connected = 0;

    for (int i = position; i <= X; i++) {
        if (board[i] != player) break; 
        
        connected++;
    }

    for (int i = (position - 1); i >= 0; i--) {
        if (board[i] != player) break; 

        connected++;
    }

    if (connected == WIN_LEN) return 1;

    return 0;
}
int diagonal_win_left(char player, int position) {
    int connected = 1;

    for (int i = position+(X+1); i < SIZE; i += (X+1)) {
        if (board[i] != player) break;
        connected++;
    }

    for (int i = position-(X+1); i >= 0; i -= (X+1)) {
        if (board[i] != player) break; 
        connected++;
    }

    if (connected == WIN_LEN) return 1;

    return 0;
}
int diagonal_win_right(char player, int position) {
    int connected = 1;

    for (int i = position+(X-1); i < SIZE; i += (X-1)) {
        if (board[i] != player) break; 
        connected++;
    }

    for (int i = position-(X-1); i >= 0; i -= (X-1)) {
        if (board[i] != player) break; 
        connected++;
    }

    if (connected == WIN_LEN) return 1;

    return 0;
}
int diagonal_win(char player, int position) {
    if (diagonal_win_left(player, position)) return 1;
    if (diagonal_win_right(player, position)) return 1;

    return 0;
}
int is_winner(char player, int current_position) {
    if (vertical_win(player, current_position)) return 1;
    if (horizontal_win(player, current_position)) return 1;
    if (diagonal_win(player, current_position)) return 1;

    return 0;
}
char inter_toupper(char text[]) {
    text[0] = toupper(text[0]);
}
int translate_position(char souradnice[]) {
    char souradnice_copy[CHAR_LEN];

    strcpy(souradnice_copy, souradnice);

    if (souradnice_copy[0] >= 'a' && souradnice_copy[0] <= 'z') inter_toupper(souradnice_copy); 

    char *sloupec = strtok(souradnice_copy, " ");
    char *radek = strtok(NULL, " ");

    return (atoi(radek) * X) + (souradnice_copy[0] - 'A');
}
int chceck_souradnice(char souradnice[]) {
    char souradnice_copy[CHAR_LEN];

    strcpy(souradnice_copy, souradnice);

    char *sloupec = strtok(souradnice_copy, " ");
    char *radek = strtok(NULL, " ");

    if (atoi(radek) < 0 || atoi(radek) > Y) return ERROR;
    if (souradnice_copy[0] < 'A' || souradnice_copy[0] > (X +'A')) return ERROR;

    if (board[translate_position(souradnice)] != ' ') return ERROR;

    return OK;
}
int check_characters(char souradnice[]) {
    char souradnice_copy[CHAR_LEN];

    inter_toupper(souradnice_copy);

    if (souradnice_copy[0] < 'A' || souradnice_copy[0] > 'Z') return ERROR;

    return OK;
}
void clear_screen(void) {
    system("cls");   
}
void error_message(int error_id) {
    switch (error_id) {
        case 0: 
            printf("\nSptane zadane souradnice, zkuste to znovu.\n");
            break;
        case 1:
            printf("\nNedostatecna velikost pro hraci desku.\n");
            break;
        case 2:
            printf("\nNedostatecna velikost vyherni delky\n");
            break;
    }
    
    getchar();
    clear_screen();
}  
void board_setup(void) {
    clear_screen();
    fill();
}
int board_check(void) {
    if (X > 2  && Y > 2 && WIN_LEN > 2) return 0;

    if (X < 3 || Y < 3) {
        return 1;
    }

    if (WIN_LEN < 3 || WIN_LEN > X || WIN_LEN > Y) {
        return 2;
    }
}

int main() {
    if ((board_check())) {
        error_message(board_check());
        return 0;
    }

    board_setup();

    int pocet_tahu = 0;

    char current_player = player_1;
    
    while (pocet_tahu < SIZE) {
        pocet_tahu++;
        
        draw_board();
        
        char souradnice[CHAR_LEN];

        printf("\nZadej souradnice: ");
        gets(souradnice);

        /* kdyz uzivatel zada souradnice, ktere nejsou mozne */
        // if (chceck_souradnice(souradnice)) {
        //     error_message(0);
        //     continue;
        // }

        clear_screen();

        pick(translate_position(souradnice), current_player);
        
        if (is_winner(current_player, translate_position(souradnice))) break;

        if (current_player == player_1) {
            current_player = player_2;
            continue;
        }
        
        current_player = player_1;
    }

    /* final(winning) board */
    draw_board();

    if (SIZE == pocet_tahu) {
        printf("Remiza, doslo misto!");
        return 0;
    }

    printf("\nVitezem je '%c'!\n", current_player);

    return 0;
}