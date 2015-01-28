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
	Grid(const std::vector<std::string> pattern, const std::string t="");
	~Grid();
	Cell& getCell(const int x, const int y);
	Row getRow(const int r);
	Column getColumn(const int c);
	int getWidth() const;
	int getHeight() const;
	std::string writeRow(const int r);
	std::vector<int> encode(const std::vector<Cell *> unit);
	void setTitle(const std::string t);
	std::string getTitle() const;
protected:
	int width, height;
	std::vector<Cell> cells;
	std::string title;
};


#endif
