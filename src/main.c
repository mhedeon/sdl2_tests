#include "test.h"

void init(t_test *test)
{
	test->win = create_win("test", 2000, 600);
	test->font = ttf_open_font("./libraries/libmgl/ttf/OpenSans-Regular.ttf", 30);
}

void			ttf_render_solid(SDL_Renderer *ren, TTF_Font *font, SDL_Rect *r,
															char *text)
{
	SDL_Surface	*sur;
	SDL_Texture	*msg;

	sur = TTF_RenderText_Solid(font, text, (SDL_Color) { 255, 255, 255, 0 });
	if (sur == NULL)
	{
		error_log("Text error");
		return ;
	}
	msg = SDL_CreateTextureFromSurface(ren, sur);
	if (msg == NULL)
	{
		SDL_FreeSurface(sur);
		error_log("Text error");
		return ;
	}
	SDL_RenderCopy(ren, msg, NULL, r);
	SDL_FreeSurface(sur);
	SDL_DestroyTexture(msg);
}

void			ttf_render_blended(SDL_Renderer *ren, TTF_Font *font, SDL_Rect *r,
															char *text)
{
	SDL_Surface	*sur;
	SDL_Texture	*msg;

	sur = TTF_RenderText_Blended(font, text, (SDL_Color) { 255, 255, 255, 0 });
	if (sur == NULL)
	{
		error_log("Text error");
		return ;
	}
	msg = SDL_CreateTextureFromSurface(ren, sur);
	if (msg == NULL)
	{
		SDL_FreeSurface(sur);
		error_log("Text error");
		return ;
	}
	SDL_RenderCopy(ren, msg, NULL, r);
	SDL_FreeSurface(sur);
	SDL_DestroyTexture(msg);
}

void			ttf_render_shaded(SDL_Renderer *ren, TTF_Font *font, SDL_Rect *r,
															char *text)
{
	SDL_Surface	*sur;
	SDL_Texture	*msg;

	sur = TTF_RenderText_Shaded(font, text, (SDL_Color) { 255, 255, 255, 0 }, (SDL_Color) { 255, 0, 0, 0 });
	if (sur == NULL)
	{
		error_log("Text error");
		return ;
	}
	msg = SDL_CreateTextureFromSurface(ren, sur);
	if (msg == NULL)
	{
		SDL_FreeSurface(sur);
		error_log("Text error");
		return ;
	}
	SDL_RenderCopy(ren, msg, NULL, r);
	SDL_FreeSurface(sur);
	SDL_DestroyTexture(msg);
}

int main()
{
	t_test	*test;
	// char text[32];
	char *text = (char*)malloc(sizeof(char));
							text[0] = '\0';
	int len = 1;
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
			if (KEY == SDLK_BACKSPACE)
				text[strlen(text) - 1] = '\0';
			if (KEY == SDLK_ESCAPE)
				done = SDL_TRUE;
			if (KEY == SDLK_RETURN)
			{
				text = realloc(text, --len);
				strncat(text, "\n", len - strlen(text) - 1);
			}
			switch (e.type)
			{
				case SDL_QUIT:
					/* Quit */
					done = SDL_TRUE;
					break;
				case SDL_TEXTINPUT:
					/* Add new text onto the end of our text */
					text = realloc(text, ++len);
					strncat(text, e.text.text, len - strlen(text) - 1);
					// strncat(text, e.text.text, 32 - strlen(text) - 1);
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
		// printf("text: %s\n\n", text);

		SDL_UpdateTexture(test->win->tex, NULL, test->win->buff,
			test->win->w * sizeof(Uint32));
		SDL_RenderClear(test->win->ren);
		SDL_RenderCopy(test->win->ren, test->win->tex, NULL, NULL);

		int w, h;
		TTF_SizeText(test->font, text, &w, &h);
		ttf_render_solid(test->win->ren, test->font, &(SDL_Rect) { 50, 50, w, h }, text);
		ttf_render_blended(test->win->ren, test->font, &(SDL_Rect) { 50, 150, w, h }, text);
		ttf_render_shaded(test->win->ren, test->font, &(SDL_Rect) { 50, 250, w, h }, text);

		SDL_RenderPresent(test->win->ren);
	}

	// system("leaks test");
	return (0);
}