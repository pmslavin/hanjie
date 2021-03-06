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
		"00111111100001110111",
		"01111111110011111110",
		"01111111111010001100",
		"11111111111100100110",
		"11111110011110000011",
		"11111111011111100111",
		"01010111111111111110",
		"01010101111001111110",
		"01110101011100111000",
		"00011111111111110010"
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

	Grid pg(whale, "Whale");
//	Grid pg(20, 15);
	Frame f(1200, 900, &pg);

	std::cout << pg.getWidth() << ","
		  << pg.getHeight() << std::endl;
	
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

	std::cout << std::endl;
	code.clear();

	for(int c=0; c<pg.getWidth(); ++c){
		code = pg.encode(pg.getColumn(c));
		for(auto i: code)
			std::cout << i << " ";

		std::cout << std::endl;
	}

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
						if(event.key.keysym.sym == 'b')
							f.writeBMP("output.bmp");
						if(event.key.keysym.sym == 'c')
							f.clearAllCells();
						if(event.key.keysym.sym == 'h')
							f.toggleHide();
						if(event.key.keysym.sym == 'i')
							f.invertGrid();
						if(event.key.keysym.sym == 'r')
							f.revert();
						if(event.key.keysym.sym == 's')
							f.scale();
						break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEMOTION:
						f.mouseAction(&event);
						break;

			}	
		}
		f.refresh();
		SDL_Delay(delay);
	}


	return 0;
}
