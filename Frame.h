#ifndef _FRAME_H_
#define _FRAME_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Grid;
class Cell;
enum class State;


class Frame
{
public:
	Frame(int w, int h, Grid *g=nullptr);
	~Frame();
	void drawGrid();
	void setGrid(Grid *g);
	void updateGrid();
	void setGridGeometry();
	void setCell(int x, int y);
	void clearCell(int x, int y);
	void clickAt(int mx, int my);
	void refresh();
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
	Cell& xy2Cell(int mx, int my);
	bool isXYinGrid(int mx, int my) const;
};

#endif
