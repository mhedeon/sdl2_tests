#include "test.h"

void init(t_test *test)
{
	test->win = create_win("test", 800, 600);
	test->font = ttf_open_font("./libraries/libmgl/ttf/OSR.ttf", 20);
}

int main()
{
	t_test	*test;
	char *text;
	char *composition;
	Sint32 cursor;
	Sint32 selection_len;
	SDL_bool done = SDL_FALSE;

	if ((test = (t_test*)malloc(sizeof(t_test))) == NULL)
		return (error_log("malloc err"));
	init(test);
	
	SDL_StartTextInput();
	while (!done)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			if (KEY == SDLK_ESCAPE)
				done = SDL_TRUE;
			switch (e.type)
			{
				case SDL_QUIT:
					/* Quit */
					done = SDL_TRUE;
					break;
				case SDL_TEXTINPUT:
					/* Add new text onto the end of our text */
					strcat(text, e.text.text);
					printf("inpt\n");
					break;
				case SDL_TEXTEDITING:
					/*
					Update the composition text.
					Update the cursor position.
					Update the selection length (if any).
					*/
					printf("edt\n");
					composition = e.edit.text;
					cursor = e.edit.start;
					selection_len = e.edit.length;
					
					break;
			}
		}
		printf("text: %s\nlength: %lu\n", text, strlen(text));
		// ttf_render_text(test->win->ren, test->font, &(SDL_Rect) { 50, 50, selection_len, 20 }, text);
		upd_win(test->win);
	}
	

	return (0);
}