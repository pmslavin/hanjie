#ifndef _GRID_H_
#define _GRID_H_

#include <vector>

class Cell;

typedef std::vector<Cell *> Row;
typedef std::vector<Cell *> Column;

class Grid
{
public:
	Grid(int w, int h);
	~Grid();
	Cell *getCell(int x, int y) const;
	Row getRow(const int r) const;
	Column getColumn(const int c) const;
protected:
	int width, height;
	std::vector<Cell *> cells;
};


#endif
