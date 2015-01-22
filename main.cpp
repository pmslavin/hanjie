#include "Cell.h"
#include "Grid.h"

int main()
{
	Cell c;
	c.showState();
	c.setState(State::Filled);
	c.showState();

	Grid g(20, 15);

	Row r = g.getRow(0);
	Column col = g.getColumn(7);

	return 0;
}
