#ifndef _GRID_H_
#define _GRID_H_

#include <vector>
#include <string>

class Cell;

typedef std::vector<Cell *> Row;
typedef std::vector<Cell *> Column;

class Grid
{
public:
	Grid(int w, int h);
	Grid(std::vector<std::string> pattern);
	~Grid();
	Cell& getCell(int x, int y);
	Row& getRow(const int r);
	Column& getColumn(const int c);
	int getWidth() const;
	int getHeight() const;
	std::string writeRow(const int r);
	std::vector<int> encode(const std::vector<Cell *> unit);
protected:
	int width, height;
	std::vector<Cell> cells;
};


#endif
