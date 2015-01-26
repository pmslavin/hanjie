#include "Cell.h"
#include "Grid.h"
#include "Frame.h"
#include "SDL2/SDL.h"


std::vector<std::string>
puppy = { 	"00000011111111111110",
		"00111110000000110011",
		"01110001110000011001",
		"11110111111000001101",
		"11101100011000000101",
		"11101101011001000101",
		"11100110100000000110",
		"11100011101110000100",
		"11100000001110000100",
		"00011000000100001100",
		"00001100000100011000",
		"01111110001010111110",
		"11000111111111100011",
		"10000011100111000001",
		"10101011000011010101"
	};

std::vector<std::string>
whale = { 	"00111011000000000000",
		"01111111100000000000",
		"01100100100000000000",
		"01000100000000000000",
		"00000100000000000000",
		"00000000000000000000",
		"00000000000000000000",
		"00000000000000000000",
		"00000000000000000000",
		"00000000000000000000",
		"00000000000000000000",
		"00000000000000000000",
		"00000000000000000000",
		"00000000000000000000",
		"00000000000000000000"
	};

std::vector<std::string>
cross = {	"0001000",
		"0001000",
		"0001000",
		"1111111",
		"0001000",
		"0001000",
		"0001000"
	};

std::vector<std::string>
emma =	{	"0111110",
		"0100000",
		"0111110",
		"0100000",
		"0111110"
	};


int main()
{
/*	Cell c(7,5);
	c.showState();
	c.setState(State::Filled);
	c.showState();
	std::cout << c;

	Grid g(20, 15);

	Row row = g.getRow(3);
	Column col = g.getColumn(7);

	for(auto& e: row)
		std::cout << *e;

	for(auto& e: col)
		std::cout << *e;
*/
	bool active = true;
	const int delay = 100;
	SDL_Event event;

	Grid pg(cross);
//	Grid pg(20, 15);
	Frame f(1200, 900, &pg);

	std::cout << pg.getWidth() << ","
		  << pg.getHeight() << std::endl;
	
/*	for(int r=0; r<pg.getHeight(); ++r){
		for(auto& e: pg.getRow(r))
			std::cout << *e;
	}
*/
	for(int r=0; r<pg.getHeight(); ++r){
		std::cout << pg.writeRow(r) << std::endl;
	}

	std::vector<int> code;
	for(int r=0; r<pg.getHeight(); ++r){
		code = pg.encode(pg.getRow(r));
		for(auto i: code)
			std::cout << i << " ";

		std::cout << std::endl;
	}

	code.clear();
	std::cout << std::endl;

	for(int c=0; c<pg.getWidth(); ++c){
		code = pg.encode(pg.getColumn(c));
		for(auto i: code)
			std::cout << i << " ";

		std::cout << std::endl;
	}

	int mx, my;
	f.drawGrid();
	f.updateGrid();
	f.refresh();
	while(active){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:	
						active = false;
						break;
				case SDL_KEYDOWN:
						if(event.key.keysym.sym == 'q')
							active = false;
						break;
				case SDL_MOUSEBUTTONDOWN:
						SDL_GetMouseState(&mx, &my);
						f.clickAt(mx, my);
						break;

			}	
		}
		f.refresh();
		SDL_Delay(delay);
	}


	return 0;
}
