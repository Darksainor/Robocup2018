#ifndef __COORD_H_
#define __COORD_H_
#ifndef ARDUINO
#include <stdint.h>
#endif

class Coord {
public:
	Coord();
	Coord(uint8_t x, uint8_t y);
	uint8_t getX() const;
	uint8_t getY() const;
	void setX(uint8_t x);
	void setY(uint8_t y);
	bool operator==(const Coord& c);
private:
	uint8_t x, y;
};
#endif
