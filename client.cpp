#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
#include "piece.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

int main(int argc, char* argv[])
{
	struct sockaddr_in peer;
	int s;
	int rc;
    STATUS term;
	peer.sin_family = AF_INET;
    if(argc <=1 || argc > 3)
    {
        printf("usage: process [ <ip> ] <port>\n");
        return 1;
    }
    else if(argc == 2)
    {
        peer.sin_port = htons( atoi(argv[1]) );
        peer.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    }
    else 
    {
        peer.sin_port = htons( atoi(argv[2]) );
        peer.sin_addr.s_addr = inet_addr( argv[1] );
    }

	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( s < 0 )
	{
		perror( "socket call failed" );
		return 1;
	}

	rc = connect( s, ( struct sockaddr * )&peer, sizeof( peer ) );
	if ( rc )
	{
		perror( "connect call failed" );
		return 1;
	}
    // sync max size
    Board board;
    board.board_size = 10;
    board.goal_size = 5;
    Init(&board);
    print(server_sync_now);

    rc = recv( s, &board, sizeof(Board), 0 );
    if ( rc <= 0 )
    {
        perror( "recv call failed" );
        goto END;
    }
    else
        printf( "recv board size:%d, goal size: %d\n", board.board_size, board.goal_size);

    Init(&board);
    CCP ccp;
    memset(&ccp, 0, sizeof(ccp));

    while(1)
    {
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

            /* draw piece */
            print(term);

            if(my_term_now == term)
            {
                continue; 
            }
            else if(op_term_now == term)
            {
                /* send piece */
                rc = send( s, &ccp, sizeof(ccp), 0 );
                if ( rc <= 0 )
                {
                    perror( "send call failed" );
                    goto END;
                }
                break;    
            }
            else if(i_win_now == term)
            {
                rc = send( s, &ccp, sizeof(ccp), 0 );
                if ( rc <= 0 )
                {
                    perror( "send call failed" );
                    goto END;;
                }
                goto END;;
            }
            else if(op_win_now == term)
            {
                goto END;;
            }
        }
        while(1)
        {
            /* recv piece */
            rc = recv( s, &ccp, sizeof(ccp), 0 );
            if ( rc <= 0 )
            {
                perror( "recv call failed" );
                goto END;; 
            }
            else
                printf( "recv %d %d\n", ccp.x, ccp.y );
            /* draw piece */
            term = op_term(ccp.x, ccp.y);
            print(term);

            if(my_term_now == term)
            {
                break; 
            }
            else if(op_term_now == term)
            {
                cout<<"error\n";
                goto END;;
            }
            else if(i_win_now == term || op_win_now == term)
            {
                goto END;;
            }
            else
            {
                cout<<"error\n";
                goto END;
            }
        }
    }
END:
    close(s);
    return 0;
}
