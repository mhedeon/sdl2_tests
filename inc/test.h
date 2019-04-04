#ifndef TEST_H
# define TEST_H

# include "libmgl.h"
# include "string.h"

# include "SDL_net.h"

# define KEY e.type == SDL_KEYDOWN && e.key.keysym.sym

//-----------------------------------------------------------------------------
# define MAX_PACKET 0xFF
# define MAX_SOCKETS 0x10

//-----------------------------------------------------------------------------
# define WOOD_WAIT_TIME 5000

//-----------------------------------------------------------------------------
# define FLAG_QUIT 0x0000
# define FLAG_WOOD_UPDATE 0x0010

typedef struct	s_test
{
	t_window	*win;
	TTF_Font	*font;
}				t_test;

typedef struct {
	int in_use;
	int questing;
	uint8_t amt_wood;
	uint32_t timer_wood;
} Client;

int next_ind = 0;
TCPsocket server_socket;
Client clients[MAX_SOCKETS];
SDLNet_SocketSet socket_set;
TCPsocket sockets[MAX_SOCKETS];

#endif
