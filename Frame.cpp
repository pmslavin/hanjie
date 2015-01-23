#include "Frame.h"


Frame::Frame(int w, int h) : width(w), height(h)
{
	window = SDL_CreateWindow("Hanjie", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	draw();
	SDL_Delay(5000);
}


Frame::~Frame()
{

}


void Frame::draw()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	int rows	= 15;
	int cols	= 20;
	int header	= height/7;
//	int footer	= height/7;
	int margin_l	= width/9;
//	int margin_r	= width/9;
	int code_l	= width/7;		/* Code space left */
	int code_t	= height/7;		/* Code space top */
	int cellsz	= 32;

	int gridx_l	= margin_l + code_l;	/* Grid start x left */
	int gridy_t	= header + code_t;	/* Grid start y top */
	int gridw	= cellsz * cols;
	int gridh	= cellsz * rows;

	int r, c;

	/* Grid rows and cols */
	for(r=0; r<=rows; ++r)
		SDL_RenderDrawLine(renderer, gridx_l, gridy_t+r*cellsz, gridx_l+gridw, gridy_t+r*cellsz);

	for(c=0; c<=cols; ++c)
		SDL_RenderDrawLine(renderer, gridx_l+c*cellsz, gridy_t, gridx_l+c*cellsz, gridy_t+gridh);

	/* Emphasize every fifth */
	for(r=0; r<=rows; r+=5)
		SDL_RenderDrawLine(renderer, gridx_l, gridy_t+r*cellsz+1, gridx_l+gridw, gridy_t+r*cellsz+1);
	
	for(c=0; c<=cols; c+=5)
		SDL_RenderDrawLine(renderer, gridx_l+c*cellsz+1, gridy_t, gridx_l+c*cellsz+1, gridy_t+gridh);
	
	/* Code space rows */
	for(r=0; r<=rows; ++r)
		SDL_RenderDrawLine(renderer, gridx_l-code_l, gridy_t+r*cellsz, gridx_l, gridy_t+r*cellsz);

	/* Code space cols */
	for(c=0; c<=cols; ++c)
		SDL_RenderDrawLine(renderer, gridx_l+c*cellsz, gridy_t-code_t, gridx_l+c*cellsz, gridy_t+gridh);

	/* Emphasize every fifth */
	for(r=0; r<=rows; r+=5)
		SDL_RenderDrawLine(renderer, gridx_l-code_l, gridy_t+r*cellsz+1, gridx_l, gridy_t+r*cellsz+1);

	for(c=0; c<=cols; c+=5)
		SDL_RenderDrawLine(renderer, gridx_l+c*cellsz+1, gridy_t-code_t, gridx_l+c*cellsz+1, gridy_t+gridh);


	SDL_RenderPresent(renderer);
}
