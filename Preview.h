#ifndef _PREVIEW_H_
#define _PREVIEW_H_

#include "SDL2/SDL.h"

class Grid;
class Cell;

class Preview
{
public:
	Preview(int w, int h, Grid *g);
	~Preview();
	SDL_Surface *draw();
	int getWidth() const;
	int getHeight() const;
protected:
	int width, height;
	Grid *grid;
	SDL_Surface *surf;
	int border;
	int pxsz_x, pxsz_y;
	SDL_Rect pxRect;
};



#endif
