#include "Preview.h"
#include "Grid.h"
#include "Cell.h"


Preview::Preview(int w, int h, Grid *g) :	width(w),
						height(h),
						grid(g),
						border(1),
						pxsz_x(width/grid->getWidth()),
						pxsz_y(height/grid->getHeight())
{
	surf = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
}


Preview::~Preview()
{

}


SDL_Surface *Preview::draw()
{
	Row row;
	SDL_Rect pxRect = { 0, 0, pxsz_x, pxsz_y };
	int cidx = 0;

/*	SDL_Rect interior = { border, border, width-2*border, height-2*border };
	SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, 0x00, 0x00, 0x00));
	SDL_FillRect(surf, &interior, SDL_MapRGB(surf->format, 0xFF, 0xFF, 0xFF));
*/
	SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, 0xFF, 0xFF, 0xFF));

	for(int r=0; r<grid->getHeight(); ++r){
		row = grid->getRow(r);
		pxRect.y = r*pxsz_y;
		for(auto& c: row){
			if(c->getState() == State::Filled){
				pxRect.x = cidx*pxsz_x;
				SDL_FillRect(surf, &pxRect, SDL_MapRGB(surf->format, 0x00, 0x00, 0x00));
			}
			++cidx;
		}
		cidx = 0;
	}
	return surf;
}


int Preview::getWidth() const
{
	return width;
}


int Preview::getHeight() const
{
	return height;
}
