#include<stdio.h>
#include<assert.h>

typedef char board_t[3][3];
typedef char player_t;

void init_board(board_t board){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            board[i][j] = '.';
        }
    }
}

int has_won(board_t board, player_t player){
    for(int i=0;i<3;i++){
        if((board[i][0]==player) && (board[i][1]==player) && (board[i][2]==player)) return 1;
        if((board[0][i]==player) && (board[1][i]==player) && (board[2][i]==player)) return 1;
    }
    if((board[0][0]==player) && (board[1][1]==player) && (board[2][2]==player)) return 1;
    return 0;
}

void print_board(board_t board){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            printf("%c ",board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int is_full(board_t board){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(board[i][j]=='.') return 0;
        }
    }
    return 1;
}

player_t other_player(player_t player){
    switch(player){
        case 'X' : return 'O';
        case 'O' : return 'X';
        default : assert(0);
    }
}

typedef struct {
    int row;
    int col;
    int score; // +1 for win, 0 for draw, -1 for loss
} move_t; // Think of this as a tuple from python

move_t best_move(board_t board, player_t player ){
    move_t candidate;
    move_t response;
    int no_candidate = 1;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(board[i][j]=='.'){
                board[i][j] = player;
                if(has_won(board, player)){
                    board[i][j] = '.';
                    return (move_t){
                        .row = i,
                        .col = j,
                        .score = 1
                    };
                }
                board[i][j] = '.';
            }
        }
    }

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(board[i][j]=='.'){
                board[i][j] = player;
                response = best_move(board, other_player(player));
                board[i][j] = '.';
                if(response.score == -1){ // This move leads to the other player losing, so, return this as this is an optimal move
                    return (move_t){
                        .row = i,
                        .col = j,
                        .score = 1
                    };
                }
                else if(response.score = 0){
                    candidate = (move_t){ // No if here as even if we get a condidate by response.score = 1(opponent wins), still we need to do this move as here, response.score = 0
                        .row = i,
                        .col = j,
                        .score = 0
                    };
                    no_candidate = 0;
                }
                else{ // response.score = 1, opponent wins here
                    if(no_candidate){
                        candidate = (move_t){
                            .row = i,
                            .col = j,
                            .score = -1
                        };
                        no_candidate = 0;
                    }
                }
            }
        }
    }
    return candidate;
}

int main(){
    int move, row, col;
    board_t board;
    player_t current = 'X';
    move_t response;

    init_board(board);
    print_board(board);
    while(1){
        
        if(current == 'X'){
            printf("0 1 2\n3 4 5\n6 7 8\n");
            printf("Enter your move: ");
            scanf("%d",&move);
            assert(board[move/3][move%3]=='.');
            board[move/3][move%3] = 'X';
            printf("User:\n");
            print_board(board);
        }
        else{
            response = best_move(board,current);
            board[response.row][response.col] = current;
            printf("Computer:\n");
            print_board(board);
        }

        if(has_won(board, current)){
            print_board(board);
            printf("Player %c won the game!",current);
            break;
        }
        else if(is_full(board)){
            print_board(board);
            printf("The game is a Draw!");
            break;
        }
        current = other_player(current);
    }
}
