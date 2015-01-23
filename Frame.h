#ifndef _FRAME_H_
#define _FRAME_H_

#include "SDL2/SDL.h"


class Frame
{
public:
	Frame(int w, int h);
	~Frame();
	void draw();
protected:
	int width, height;
	SDL_Window *window;
	SDL_Renderer *renderer;
};

#endif
