#define IMM2D_WIDTH 300
#define IMM2D_HEIGHT 150
#define IMM2D_IMPLEMENTATION
#include <iostream>
#include "immediate2d.h"
using namespace std;

static constexpr const char astronavepng[] = "immagineastronave.PNG";

 
void run() {
	int x = 145;
	int y = 100;

	
	const Image astronave = LoadImage(astronavepng);

	DrawImage(x, y, astronave);

}
