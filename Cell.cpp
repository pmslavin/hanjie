#include "Cell.h"
#include <iostream>
#include <type_traits>
#include <map>


std::map<State, const std::string> Cell::stateMap = {	{State::Undef, "Undefined"},
							{State::Clear, "Clear"},
							{State::Filled, "Filled"}
						    };

Cell::Cell(int x, int y, State s) : x(x),
				    y(y),
				    state(s)
{
}


Cell::~Cell()
{

}

int Cell::getX() const
{
	return x;
}


int Cell::getY() const
{
	return y;
}


State Cell::getState() const
{
	return state;
}


void Cell::setState(State s)
{
	state = s;
}


void Cell::showState() const
{
//	std::cout << static_cast<std::underlying_type<State>::type>(state) << std::endl;
	std::cout << Cell::stateMap[state] << std::endl;
}
