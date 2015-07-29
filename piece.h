#ifndef __piece_h__
#define __picce_h__

#define PLAYER_CLIENT 'O'
#define PLAYER_SERVER 'X'
#define PLAYER_NONE '+'

enum STATUS{
    my_term_now = 0,
    op_term_now,
    i_win_now,
    op_win_now,
    server_set_now,
    server_sync_now,
    board_too_small,
    board_too_large,
    goal_too_small,
    goal_too_large,
};

enum SET_ITEM
{
    SET_NONE = 0,
    SET_BOARD,
    SET_GOAL,
};

typedef struct _CCP{
    int x;
    int y;
}CCP;

typedef struct _Board{
    int board_size;
    int goal_size;
}Board;

void Init(Board *board);
void print(STATUS status, SET_ITEM item = SET_NONE);
bool win(int x,int y,int c);
STATUS my_term(int x, int y);
STATUS op_term(int x, int y);
int curSetCCP(CCP *ccp);
int curSetBoard(Board* board);


#endif
