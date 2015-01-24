#ifndef _FRAME_H_
#define _FRAME_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Grid;
enum class State;


class Frame
{
public:
	Frame(int w, int h, Grid *g=nullptr);
	~Frame();
	void draw();
	void setGrid(Grid *g);
	void updateGrid();
	void setGridGeometry();
protected:
	int width, height;
	int rows, cols;
	int gridx_l, gridy_t;
	int cellsz;
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font;
	Grid *grid;
	void flipCell(int x, int y);
};

#endif
