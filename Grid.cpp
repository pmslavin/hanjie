#include "Grid.h"
#include "Cell.h"

#include <stdexcept>
#include <sstream>


Grid::Grid(int w, int h) : width(w),
			   height(h)
{
	cells.reserve(w*h);
	for(int r=0; r<h; ++r){
		for(int c=0; c<w; ++c){
			cells.push_back(Cell(c, r));
		}
	}

}


Grid::~Grid()
{
//	for(auto& c: cells)
//		delete c;

	cells.clear();
}


Cell& Grid::getCell(int x, int y)
{
	if(x < 0 || x >= width || y < 0 || y >= height){
		std::ostringstream ess;
		ess << "Invalid Cell: getCell(" << x
		    << "," << y << ")" << std::endl;
		throw std::out_of_range(ess.str());

	}
	return cells[y*width+x];
}


Row& Grid::getRow(const int r)
{
	if(r < 0 || r >= height){
		std::ostringstream ess;
		ess << "Invalid Row: getRow(" << r
		    << ")" << std::endl;
		throw std::out_of_range(ess.str());
	}

	Row *row = new Row();
	row->reserve(width);

//	std::cout << "getRow() from " << r*width
//		  << " to " << (r+1)*width << std::endl;
	for(int i=r*width; i<(r+1)*width; ++i){
		std::cout << i << " ";
		row->push_back(&cells[i]);
	}
	std::cout << std::endl;

	return *row;
}


Column& Grid::getColumn(const int c)
{
	if(c < 0 || c >= height){
		std::ostringstream ess;
		ess << "Invalid Column: getColumn(" << c
		    << ")" << std::endl;
		throw std::out_of_range(ess.str());
	}

	Column *col = new Column();
	col->reserve(height);

	for(int i=c; i<width*height; i+=width){
//		std::cout << i << " (" << &cells[i] << ") ";
		col->push_back(&cells[i]);
	}
	std::cout << std::endl;

	return *col;
}


int Grid::getWidth() const
{
	return width;
}


int Grid::getHeight() const
{
	return height;
}
