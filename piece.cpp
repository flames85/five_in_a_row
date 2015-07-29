#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "piece.h"
#include <termio.h>
#include <stdio.h>

using namespace std;

#define MAX_SIZE 100


int A[MAX_SIZE+1][MAX_SIZE+1];
int num;
int last_x = 1; // op or my last x
int last_y = 1; // op or my last y

int cur_x = 1; // my cur x
int cur_y = 1; // my cur y

int my_last_cur_x = 1; // my last cur x
int my_last_cur_y = 1; // my last cur y

Board _board;


void Init(Board* board)
{
    for(int i=1;i<=MAX_SIZE;i++)
    {
        for(int j=1;j<=MAX_SIZE;j++)
            A[i][j] = PLAYER_NONE;
    }
    // set board
    _board.board_size = board->board_size;
    _board.goal_size = board->goal_size;

    my_last_cur_x = _board.board_size/2;
    my_last_cur_y = _board.board_size/2;

    cur_x = _board.board_size/2;
    cur_y = _board.board_size/2;
}

void printHead(SET_ITEM set_item)
{
    switch(set_item)
    {
        case SET_BOARD:
            printf("\n \033[40;32mboard :\033[0m" // board
                    " \033[41;37m%d\033[0m"        // %d
                    "\033[40;32mx\033[0m"         // x
                    "\033[41;37m%d\033[0m\n"      // %d
                    " \033[40;32mgoal  :\033[0m"  // goal
                    " %d\n",    // %d
                    _board.board_size, 
                    _board.board_size, 
                    _board.goal_size);
            break;
        case SET_GOAL: 
            printf("\n \033[40;32mboard :\033[0m" // board
                    " %d"        // %d
                    "\033[40;32mx\033[0m"         // x
                    "%d\n"      // %d
                    " \033[40;32mgoal  :\033[0m"  // goal
                    " \033[41;37m%d\033[0m\n",    // %d
                    _board.board_size, 
                    _board.board_size, 
                    _board.goal_size);
            break;
        default:
            printf("\n \033[40;32mboard :\033[0m" // board
                    " %d"        // %d
                    "\033[40;32mx\033[0m"         // x
                    "%d\n"      // %d
                    " \033[40;32mgoal  :\033[0m"  // goal
                    " %d\n",    // %d
                    _board.board_size, 
                    _board.board_size, 
                    _board.goal_size);
            break;
    }
}
void print(STATUS status, SET_ITEM item)
{
    system("clear");

    for(int i=1;i <= (4+3*_board.board_size) ;i++)
        cout << "-";

    printHead(item);

    for(int i=1;i <= (4+3*_board.board_size) ;i++)
        cout << "-";
    printf("\n");
        
    cout << "  ";
    for(int i=1;i <= _board.board_size ;i++)
    {
       printf("\033[40;32m %2d\033[0m", i);
    }
    cout <<endl;

    for(int i=1;i <= _board.board_size;i++)
    {
        printf("\033[40;32m%2d\033[0m", i);
        for(int j=1;j <= _board.board_size;j++)
        {
            if(i == last_x && j == last_y)
            {
#ifdef CUR_MODE
                if(i == cur_x && j == cur_y)
                {
                    // curses
                    printf("  ");
                    printf("\033[44;32m%c\033[0m", A[i][j]);
                }
                else
                {
                    printf("\033[40;32m %2c\033[0m", A[i][j]);
                }
#else
                printf("\033[40;32m %2c\033[0m", A[i][j]);
#endif
            }
            else
            {
#ifdef CUR_MODE
                if(i == cur_x && j == cur_y)
                {
                    // curses
                    printf("  ");
                    printf("\033[44;37m%c\033[0m", A[i][j]);
                }
                else
                {
                    printf("\033[40;37m %2c\033[0m", A[i][j]);
                }
#else
                printf("\033[40;37m %2c\033[0m", A[i][j]);
#endif
            }
        }
        cout<<endl;
    }
    for(int i=1;i <= (4+3*_board.board_size) ;i++)
        cout << "-";

    printf("\n");
    switch(status)
    {
        case my_term_now:
            printf("\033[40;32m my term ... \033[0m");
            break;
        case op_term_now:
            printf("\033[40;31m op term ... \033[0m");
            break;
        case i_win_now:
            printf("\033[42;31m You win ! \033[0m");
            break;
        case op_win_now:
            printf("\033[41;37m You lost ! \033[0m");
            break;
        case server_set_now:
            printf("\033[40;32m seting ... \033[0m");
            break;
        case server_sync_now:
            printf("\033[40;31m sync ... \033[0m");
            break;
        case board_too_large:
            printf("\033[41;37m board too large ! \033[0m");
            break;
        case board_too_small:
            printf("\033[41;37m board too small ! \033[0m");
            break;
        case goal_too_large:
            printf("\033[41;37m goal too large ! \033[0m");
            break;
        case goal_too_small:
            printf("\033[41;37m goal too small ! \033[0m");
            break;
        default:
            break;
    }
    printf("\n");

    for(int i=1;i <= (4+3*_board.board_size) ;i++)
        cout << "-";
    cout << endl;
}
bool win(int x,int y,int c)
{
    int i = x,j = y;
    // 0 Degree
    num=0;
    while(i>=1&&A[i][j]==c)
    {
        num++;
        i--;
    }
    i=x+1;
    while(i<=MAX_SIZE&&A[i][j]==c)
    {
        num++;
        i++;
    }
    if(num >= _board.goal_size)return true;

    // 90 Degree
    num=0;
    i=x,j=y;
    while(j>=1&&A[i][j]==c)
    {
        num++;
        j--;
    }
    j=y+1;
    while(j<=MAX_SIZE&&A[i][j]==c)
    {
        num++;
        j++;
    }
    if(num >= _board.goal_size)return true;

    // 45 Degree
    num=0;
    i=x;j=y;
    while(i>=1&&j>=1&&A[i][j]==c)
    {
        num++;
        i--;
        j--;
    }
    i=x+1,j=y+1;
    while(i<=MAX_SIZE && j<=MAX_SIZE&&A[i][j]==c)
    {
        num++;
        i++;
        j++;
    }
    if(num >= _board.goal_size)return true;

    // 135 Degree
    num=0;
    i=x;j=y;
    while(i<=MAX_SIZE && j>=1&&A[i][j]==c)
    {
        num++;
        i++;
        j--;
    }
    i=x-1,j=y+1;
    while(i>=1&&j<=MAX_SIZE &&A[i][j]==c)
    {
        num++;
        i--;
        j++;
    }
    if(num >= _board.goal_size)return true;
    return false;
}

STATUS my_term(int x, int y)
{
    if(x <= 0 || y <= 0 || x > _board.board_size || y > _board.board_size|| A[x][y]==(int)PLAYER_CLIENT|| A[x][y] == (int)PLAYER_SERVER)
    {
        cerr << "piece exists" << endl;
        return my_term_now;
    }
    A[x][y] = PLAYER_CLIENT;
    last_x = x;
    last_y = y;

    if(win(x,y,PLAYER_CLIENT))
    {
        return i_win_now;
    }
    return op_term_now;
}

STATUS op_term(int x, int y)
{
    if(x <= 0 || y <= 0 || x > _board.board_size || y > _board.board_size || A[x][y] == PLAYER_CLIENT || A[x][y] == PLAYER_SERVER)
    {
        cerr << "piece exists" << endl;
        return op_term_now;
    }
    A[x][y] = PLAYER_SERVER;
    last_x = x;
    last_y = y;
    if(win(x,y,PLAYER_SERVER))
    {
        return op_win_now;
    }
    return my_term_now;
}
int scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);

    in = getchar();

    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}
int curSetCCP(CCP *ccp)
{
    int c = 0;
    cur_x = my_last_cur_x;
    cur_y = my_last_cur_y;
    if(cur_x <= 0 || cur_y <= 0 || cur_x > _board.board_size || cur_y > _board.board_size)
    {
        cur_x = _board.board_size/2;
        cur_y = _board.board_size/2;
    }
    while(1)
    {
        c = scanKeyboard();
        switch(c)
        {
        case 65:
            cur_x--;
            break;
        case 66:
            cur_x++;
            break;
        case 67:
            cur_y++;
            break;
        case 68:
            cur_y--;
            break;
        case 10:
            ccp->x = cur_x;
            ccp->y = cur_y;
            my_last_cur_x = cur_x;
            my_last_cur_y = cur_y;
            return 0;
        }
        if(cur_x <= 0 || cur_y <= 0 || cur_x > _board.board_size || cur_y > _board.board_size)
        {
            cur_x = _board.board_size/2;
            cur_y = _board.board_size/2;
        }
        print(my_term_now);
    }
}

int curSetBoard(Board* board)
{
    _board.board_size = 10;
    _board.goal_size = 5;
    int c = 0;
    SET_ITEM item = SET_BOARD;
    STATUS status = server_set_now;
    while(1)
    {
        Init(&_board);
        print(status, item);
        c = scanKeyboard();
        switch(c)
        {
            case 65:
                if(goal_too_large == status || goal_too_small == status)
                {
                    break;
                }
                item = SET_BOARD;
                break;
            case 66:
                if(board_too_large == status || board_too_small == status)
                {
                    break;
                }
                item = SET_GOAL;
                break;
            case 67:
                switch(item)
                {
                    case SET_BOARD:
                        if(board_too_large == status)
                        {
                            break;
                        }
                        if(++_board.board_size > 30)
                        {
                            status = board_too_large;
                        }
                        else
                        {
                            status = server_set_now;
                        }
                        break;
                    case SET_GOAL:
                        if(goal_too_large == status)
                        {
                            break;
                        }
                        if(++_board.goal_size > _board.board_size)
                        {
                            status = goal_too_large;
                        }
                        else
                        {
                            status = server_set_now;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case 68:
                switch(item)
                {
                    case SET_BOARD:
                        if(board_too_small == status)
                        {
                            break;
                        }
                        _board.board_size--;
                        if( _board.board_size < _board.goal_size)
                        {
                            status = board_too_small;
                        }
                        else
                        {
                            status = server_set_now;
                        }
                        break;
                    case SET_GOAL:
                        if(goal_too_small == status)
                        {
                            break;
                        }
                        if(--_board.goal_size < 2)
                        {
                            status = goal_too_small;
                        }
                        else
                        {
                            status = server_set_now;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case 10:
                board->board_size = _board.board_size;
                board->goal_size = _board.goal_size;

                print(server_sync_now, item);
                return 0;
        }
    }
}
