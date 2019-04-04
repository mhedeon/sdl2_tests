#ifndef TEST_H
# define TEST_H

# include "libmgl.h"
# include "string.h"

# include "SDL_net.h"

# define KEY e.type == SDL_KEYDOWN && e.key.keysym.sym

typedef struct	s_test
{
	t_window	*win;
	TTF_Font	*font;
}				t_test;

#endif
