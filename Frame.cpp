#include "Frame.h"
#include "Grid.h"
#include "Cell.h"

#include <sstream>


Frame::Frame(int w, int h, Grid *g) :	width(w), height(h),
					rows(15), cols(20),
					cellsz(32),
					grid(g)
{
	if(grid)
		setGridGeometry();

	window = SDL_CreateWindow("Hanjie", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	TTF_Init();
	font = TTF_OpenFont("OpenSans-Regular.ttf", 12);
}


void Frame::setGridGeometry()
{
	rows = grid->getHeight();
	cols = grid->getWidth();
}


Frame::~Frame()
{

}


void Frame::drawGrid()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	int header	= height/7;
//	int footer	= height/7;
	int margin_l	= width/9;
//	int margin_r	= width/9;
	int code_l	= width/7;		/* Code space left */
	int code_t	= height/7;		/* Code space top */

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
}


void Frame::updateGrid()
{
	std::vector<int> code;
	SDL_Surface *code_num;
	SDL_Color textcolour = { 0x00, 0x00, 0x00, 0x00 };
	SDL_Rect code_rect;
	int cidx = 0;

	/* Set visual to match grid, encode each row,
	 * then render code numbers to appropriate rect. */
	for(int r=0; r<grid->getHeight(); ++r){
		Row row = grid->getRow(r);
		for(auto &c: row){
			if(c->getState() == State::Filled)
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
	SDL_RenderPresent(renderer);
}
