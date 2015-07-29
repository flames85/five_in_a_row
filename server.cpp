#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include "piece.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

int main(int argc, char* argv[])
{
	struct sockaddr_in local;
	int s;
	int s1;
	int rc;
    if(argc != 2)
    {
        printf("usage: process <port>\n");
        return 1;
    }

	local.sin_family = AF_INET;
	local.sin_port = htons( atoi(argv[1]) );
	local.sin_addr.s_addr = htonl( INADDR_ANY );
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( s < 0 )
	{
		perror( "socket call failed" );
		return 1;
	}
	rc = bind( s, ( struct sockaddr * )&local, sizeof( local ) );
	if ( rc < 0 )
	{
		perror( "bind call failure" );
		return 1;
	}
	rc = listen( s, 5 );
	if ( rc )
	{
		perror( "listen call failed" );
		return 1;
    }
    // set board
    Board board;
#ifdef CUR_MODE 
    curSetBoard(&board);
#else
    //
    int board_size = 10;
    int goal_size = 5;
    printf("set (board size, goal size):");
    scanf("%d %d", &board_size, &goal_size);
    setbuf(stdin, NULL); /*À¶¶õ?ÑÕ¶è*/
    board.board_size = board_size;
    board.goal_size = goal_size;
#endif

	s1 = accept( s, NULL, NULL );
	if ( s1 < 0 )
	{
		perror( "accept call failed" );
		return 1;
	}

    STATUS term = op_term_now;
    //int x = -1;
    //int y = -1;
    CCP ccp;
    memset(&ccp, 0, sizeof(ccp));

    rc = send( s1, &board, sizeof(Board), 0 );
    if ( rc <= 0 )
    {
        perror( "send call failed" );
        goto END;
    }

    Init(&board);

    while(1)
    {
        while(1)
        {
            print(op_term_now);
            rc = recv( s1, &ccp, sizeof(ccp), 0 );
            if ( rc <= 0 )
            {
                //perror( "recv call failed" );
                goto END;
            }
            // set piece 
            term = op_term(ccp.x, ccp.y);

            /* draw piece */
            print(term);
            if(my_term_now == term)
            {
                //x = ccp.x;
                //y = ccp.y;
                break; 
            }
            else if(op_term_now == term)
            {
                printf("error!\n");
                continue;    
            }
            else if(i_win_now == term || op_win_now == term)
            {
                goto END;
            }
            else
            {
                printf("error!\n");
                goto END;
            }
        }
        while(1)
        {
            print(my_term_now);
#ifdef CUR_MODE
            curSetCCP(&ccp);
#else
            printf("Please enter your x,y:");
            setbuf(stdin, NULL); /*À¶¶õ?ÑÕ¶è*/
            scanf("%d %d", &ccp.x, &ccp.y);
#endif

            // set piece 
            term = my_term(ccp.x, ccp.y);
            print(term);

            /* draw piece */
            if(my_term_now == term)
            {   
                continue; 
            }   
            else if(op_term_now == term)
            {   
                /* send piece */
                rc = send( s1, &ccp, sizeof(ccp), 0 );
                if ( rc <= 0 ) 
                {   
                    perror( "send call failed" );
                    goto END;                
                }   
                break;    
            }   
            else if(i_win_now == term)
            {   
                rc = send( s1, &ccp, sizeof(ccp), 0 );
                if ( rc <= 0 )
                {
                    perror( "send call failed" );
                    goto END;
                }
                goto END;
            }   
            else if(op_win_now == term)
            {   
                goto END;
            }
        }
    }
END:
    close(s1);
    return 0;
}
