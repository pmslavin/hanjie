#ifndef _CELL_H_
#define _CELL_H_

#include <iostream>
#include <map>

enum class State
{
	Undef	= 0,
	Clear	= 1,
	Filled	= 2
};


class Cell
{
public:
	Cell(int x=0, int y=0, State s=State::Undef);
	~Cell();
	int getX() const;
	int getY() const;
	State getState() const;
	void setState(State s);
	void showState() const;
	static std::map<State, const std::string> stateMap;
protected:
	int x, y;
	State state;
};


#endif
