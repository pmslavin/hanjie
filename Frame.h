#ifndef _FRAME_H_
#define _FRAME_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"


class Grid;
class Cell;
enum class State;
class Preview;


class Frame
{
public:
	Frame(int w, int h, Grid *g=nullptr, bool prev=true);
	~Frame();
	void drawGrid();
	void setGrid(Grid *g);
	void updateGrid();
	void setGridGeometry();
	void setCell(int x, int y);
	void clearCell(int x, int y);
	void refresh();
	void writeBMP(const char *filename);
	void mouseAction(SDL_Event *e);
	void clearAllCells();
	void toggleHide();
	void invertGrid();
	void revert();
	void scale();
protected:
	int width, height;
	int rows, cols;
	int code_l, code_t;	/* Code space x (l)eft, y (t)op */
	int gridx_l, gridy_t;	/* Grid start x (l)eft, y (t)op */
	int margin_l, margin_t;
	int cellsz;
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font, *font12, *font8, *fontTitle;
	Grid *grid, *orig;
	void flipCell(int x, int y);
	Cell& xy2Cell(int mx, int my);
	bool isXYinGrid(int mx, int my) const;
	bool hasPreview, hideCells;
	Preview *preview;
	bool mouseDown;
	int lastc_x, lastc_y;
	void clickAt(int mx, int my);
	SDL_Texture *makeTitle();
};

#endif
