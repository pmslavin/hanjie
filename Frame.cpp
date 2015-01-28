#include "Frame.h"
#include "Grid.h"
#include "Cell.h"
#include "Preview.h"

#include <sstream>


Frame::Frame(int w, int h, Grid *g, bool prev) :	width(w), height(h),
							rows(15), cols(20),
							code_l(width/7), code_t(height/7),
							cellsz(28),
							grid(g),
							hasPreview(prev),
							hideCells(false),
							preview(nullptr),
							mouseDown(false),
							lastc_x(-1), lastc_y(-1)
{
	if(grid){
		setGridGeometry();
		orig = new Grid(*grid);
	}

	window = SDL_CreateWindow("Hanjie", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	TTF_Init();
	font = TTF_OpenFont("OpenSans-Regular.ttf", 12);
	if(hasPreview)
		preview = new Preview(cols*cellsz/4, rows*cellsz/4, grid);
}


void Frame::setGridGeometry()
{
	rows = grid->getHeight();
	cols = grid->getWidth();
}


Frame::~Frame()
{
	TTF_CloseFont(font);
	TTF_Quit();

	if(preview)
		delete preview;

	if(orig)
		delete orig;
}


void Frame::drawGrid()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	int header	= height/6;
//	int footer	= height/7;
	int margin_l	= width/11;
//	int margin_r	= width/9;

	int gridw	= cellsz * cols;
	int gridh	= cellsz * rows;
	gridx_l		= margin_l + code_l;	/* Grid start x left */
	gridy_t		= header + code_t;	/* Grid start y top */

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

}


void Frame::flipCell(int x, int y)
{
	Cell &c = grid->getCell(x, y);
	State curState = c.getState();

	if(curState == State::Clear){
		c.setState(State::Filled);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	}else if(curState == State::Filled){
		c.setState(State::Clear);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	}

	SDL_Rect czRect;

	czRect.w	= cellsz;
	czRect.h	= cellsz;
	czRect.x	= gridx_l + x*cellsz;
	czRect.y	= gridy_t + y*cellsz;

	SDL_RenderFillRect(renderer, &czRect);
}


void Frame::setCell(int x, int y)
{
	Cell &c = grid->getCell(x, y);
	c.setState(State::Filled);

	SDL_Rect czRect;

	czRect.w	= cellsz;
	czRect.h	= cellsz;
	czRect.x	= gridx_l + x*cellsz;
	czRect.y	= gridy_t + y*cellsz;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &czRect);
}


void Frame::clearCell(int x, int y)
{
	Cell &c = grid->getCell(x, y);
	c.setState(State::Clear);

	SDL_Rect czRect;

	czRect.w	= cellsz;
	czRect.h	= cellsz;
	czRect.x	= gridx_l + x*cellsz;
	czRect.y	= gridy_t + y*cellsz;

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &czRect);
}


void Frame::setGrid(Grid *g)
{
	grid = g;

	if(orig)
		delete orig;

	orig = new Grid(*grid);
}


void Frame::updateGrid()
{
	std::vector<int> code;
	SDL_Surface *code_num;
	SDL_Color textcolour = { 0x00, 0x00, 0x00, 0xFF };
	SDL_Rect code_rect;
	int cidx = 0;

	/* Set visual to match grid, encode each row,
	 * then render code numbers to appropriate rect. */
	for(int r=0; r<grid->getHeight(); ++r){
		Row row = grid->getRow(r);
		for(auto &c: row){
			if(c->getState() == State::Filled && !hideCells)
				setCell(cidx, r);

			++cidx;		/* "Cell index" */
		}
		cidx = 0;

		code = grid->encode(row);
		std::ostringstream oss;
		for(auto c: code){
			oss << c << "  ";
			++cidx;		/* "Code index" */
		}
		std::string numchar = oss.str();
		numchar.erase(numchar.size()-2, 2);
		code_num = TTF_RenderText_Blended(font, numchar.c_str(), textcolour);

		code_rect.x = gridx_l - code_num->w - 6;
		code_rect.y = gridy_t + (cellsz-code_num->h)/2 + r*cellsz;
		code_rect.w = code_num->w;
		code_rect.h = code_num->h;

		SDL_Texture *codetex = SDL_CreateTextureFromSurface(renderer, code_num);
		SDL_FreeSurface(code_num);
		SDL_RenderCopy(renderer, codetex, NULL, &code_rect);
		SDL_DestroyTexture(codetex);
		cidx = 0;
	}

	/* Encode each column and render codes. */
	for(int c=0; c<grid->getWidth(); ++c){
		Column col = grid->getColumn(c);
		code = grid->encode(col);
		cidx = 0;
		for(auto it=code.rbegin(); it!=code.rend(); ++it){
			std::ostringstream oss;
			oss << *it;
			++cidx;		/* "Code index" */
			std::string numchar = oss.str();
			code_num = TTF_RenderText_Blended(font, numchar.c_str(), textcolour);

			code_rect.x = gridx_l + (cellsz-code_num->w)/2 + c*cellsz;
			code_rect.y = gridy_t - cidx*(code_num->h+1);
			code_rect.w = code_num->w;
			code_rect.h = code_num->h;

			SDL_Texture *codetex = SDL_CreateTextureFromSurface(renderer, code_num);
			SDL_FreeSurface(code_num);
			SDL_RenderCopy(renderer, codetex, NULL, &code_rect);
			SDL_DestroyTexture(codetex);
		}
	}
}


bool Frame::isXYinGrid(int mx, int my) const
{
	return (mx > gridx_l && mx < gridx_l+cols*cellsz) && (my > gridy_t && my < gridy_t+rows*cellsz);
}


Cell& Frame::xy2Cell(int mx, int my)
{
	int r, c;

	r = (mx - gridx_l)/cellsz;
	c = (my - gridy_t)/cellsz;

	return grid->getCell(r, c);
}


void Frame::clickAt(int mx, int my)
{
	if(isXYinGrid(mx, my)){
		Cell& c = xy2Cell(mx, my);
		flipCell(c.getX(), c.getY());
	}
}


void Frame::refresh()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	drawGrid();
	updateGrid();
	if(hasPreview){
		SDL_Surface *prevsurf = preview->draw();
		SDL_Texture *prevtext = SDL_CreateTextureFromSurface(renderer, prevsurf);
		SDL_Rect prevRect = { gridx_l+cols*cellsz+(width-(gridx_l+cols*cellsz)-preview->getWidth())/2, gridy_t-2*preview->getHeight(), preview->getWidth(), preview->getHeight() };
		SDL_RenderCopy(renderer, prevtext, NULL, &prevRect);
		prevRect.x -= 1;
		prevRect.y -= 1;
		prevRect.w += 1;
		prevRect.h += 1;
		SDL_RenderDrawRect(renderer, &prevRect);
		prevRect.x -= 2;
		prevRect.y -= 2;
		prevRect.w += 4;
		prevRect.h += 4;
		SDL_RenderDrawRect(renderer, &prevRect);
		SDL_DestroyTexture(prevtext);
	}
	SDL_RenderPresent(renderer);
}


void Frame::writeBMP(const char *filename)
{
	hideCells = true;
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	drawGrid();
	updateGrid();

	SDL_Rect gridRect = {gridx_l-code_l, gridy_t-code_t, code_l+cols*cellsz+2, code_t+rows*cellsz+2};
	SDL_Surface *bmpSurf = SDL_CreateRGBSurface(0, gridRect.w, gridRect.h, 24, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderReadPixels(renderer, &gridRect, SDL_PIXELFORMAT_RGB24, bmpSurf->pixels, bmpSurf->pitch);
	SDL_SaveBMP(bmpSurf, filename);
	SDL_FreeSurface(bmpSurf);

	hideCells = false;
	updateGrid();
}


void Frame::mouseAction(SDL_Event *e)
{
	int mx, my;

	SDL_GetMouseState(&mx, &my);

	if(e->type == SDL_MOUSEBUTTONDOWN)
		mouseDown = true;

	if(e->type == SDL_MOUSEBUTTONUP){
		mouseDown = false;
		lastc_x = lastc_y = -1;
	}

	if(mouseDown){
		if(isXYinGrid(mx, my)){
			Cell& c = xy2Cell(mx, my);
			/* Either (is different cell) or (is new click) */
			if((c.getX() != lastc_x || c.getY() != lastc_y) || lastc_x == -1){
				lastc_x = c.getX();
				lastc_y = c.getY();
				clickAt(mx, my);
			}
		}
	}
	
}



void Frame::clearAllCells()
{
	for(int r=0; r<grid->getHeight(); ++r){
		Row row = grid->getRow(r);
		for(auto& c: row)
			c->setState(State::Clear);
	}
}


void Frame::toggleHide()
{
	hideCells = !hideCells;
}


void Frame::invertGrid()
{
	for(int r=0; r<grid->getHeight(); ++r){
		Row row = grid->getRow(r);
		for(auto& c: row){
			State cState = c->getState();
			if(cState == State::Filled)
				c->setState(State::Clear);
			else if(cState == State::Clear)
				c->setState(State::Filled);
		}
	}
}


void Frame::revert()
{
	*grid = *orig;
	refresh();
}
