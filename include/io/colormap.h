#pragma once
#include <iostream>
#include <map>

namespace cfdtd{

enum colormap_t{
	BLUE_RED,		// (0, 0, 255) - (255, 255, 255) - (255, 0, 0)
	WHITE_BLACK,	// (255, 255, 255) - (0, 0, 0)
	CUSTOMIZED
};
struct color{
	char r;
	char g;
	char b;
	color(char r,char g,char b):r(r),g(g),b(b){}
};
class colormap{
	public:
		colormap(colormap_t cm_t);
		bool insert(float val, char r, char g, char b);
		int get_rgb(float val, char &r, char &g, char &b);
		int get_rgb(float val, float min, float max, char &r, char &g, char &b);

		colormap_t cm_t;
		std::map<float, color> colors;
		bool valid;
};

}
