#include "Grid.h"
#include "Cell.h"

#include <stdexcept>
#include <sstream>
#include <algorithm>


Grid::Grid(int w, int h) : width(w),
			   height(h)
{
	cells.reserve(w*h);
	for(int r=0; r<h; ++r){
		for(int c=0; c<w; ++c){
			cells.push_back(Cell(c, r, State::Clear));
		}
	}

}


Grid::Grid(const std::vector<std::string> pattern, const std::string t) : title(t)
{
	width = pattern[0].size();
	height = pattern.size();

	cells.reserve(width*height);

	State s;
	for(int r=0; r<height; ++r){
		for(int c=0; c<width; ++c){
			s = (pattern[r][c] == '1') ? State::Filled : State::Clear;
			cells.push_back(Cell(c, r, s));
		}
	}
}


Grid::~Grid()
{
	cells.clear();
}


Cell& Grid::getCell(const int x, const int y)
{
	if(x < 0 || x >= width || y < 0 || y >= height){
		std::ostringstream ess;
		ess << "Invalid Cell: getCell(" << x
		    << "," << y << ")" << std::endl;
		throw std::out_of_range(ess.str());

	}
	return cells[y*width+x];
}


Row Grid::getRow(const int r)
{
	if(r < 0 || r >= height){
		std::ostringstream ess;
		ess << "Invalid Row: getRow(" << r
		    << ")" << std::endl;
		throw std::out_of_range(ess.str());
	}

	Row row;
	row.reserve(width);

//	std::cout << "getRow() from " << r*width
//		  << " to " << (r+1)*width << std::endl;
	for(int i=r*width; i<(r+1)*width; ++i){
//		std::cout << i << " ";
		row.push_back(&cells[i]);
	}
//	std::cout << std::endl;

	return row;
}


Column Grid::getColumn(const int c)
{
	if(c < 0 || c >= width){
		std::ostringstream ess;
		ess << "Invalid Column: getColumn(" << c
		    << ")" << std::endl;
		throw std::out_of_range(ess.str());
	}

	Column col;
	col.reserve(height);

	for(int i=c; i<width*height; i+=width){
//		std::cout << i << " (" << &cells[i] << ") ";
		col.push_back(&cells[i]);
	}
//	std::cout << std::endl;

	return col;
}


int Grid::getWidth() const
{
	return width;
}


int Grid::getHeight() const
{
	return height;
}


std::string Grid::writeRow(const int r)
{
	Row row = getRow(r);

	std::ostringstream oss;

	for(auto& e: row)
		oss << ((e->getState() == State::Filled) ? '#' : ' ');
	
	return oss.str();
}


std::vector<int> Grid::encode(std::vector<Cell *> unit)
{
	std::vector<int> code;
	int run=0;

	for(auto it=unit.cbegin(); it != unit.cend(); ++it){
		if((*it)->getState() == State::Filled){
			++run;
		}else if(run){
			code.push_back(run);
			run = 0;
		}
	}

	if(run || code.empty())
		code.push_back(run);

	return code;
}


void Grid::setTitle(const std::string t)
{
	title = t;
}


std::string Grid::getTitle() const
{
	return title;
}
