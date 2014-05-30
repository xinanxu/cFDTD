#pragma once
#include <iostream>
#include <map>

namespace cfdtd{
namespace io{

enum colormap_t{
	BLUE_RED,		// (0, 0, 255) - (255, 255, 255) - (255, 0, 0)
	WHITE_BLACK,	// (255, 255, 255) - (0, 0, 0)
	CUSTOMIZED
};

struct color{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	color(unsigned char r, unsigned char g, unsigned char b):r(r),g(g),b(b){};
	color():r(0),g(0),b(0){};
};

class colormap{
	public:
		colormap(colormap_t cm_t_in):cm_t(cm_t_in){
			switch(cm_t){
				case BLUE_RED:
					color_nodes[0.f] = color(0, 0, 255);
					color_nodes[0.5f] = color(255, 255, 255);
					color_nodes[1.f] = color(255, 0, 0);
					valid = true;
					break;
				case WHITE_BLACK:
					color_nodes[0.f] = color(255, 255, 255);
					color_nodes[1.f] = color(0, 0, 0);
					valid = true;
					break;
				case CUSTOMIZED:
					valid = false;
					break;
			}
		}
		bool insert(float val, unsigned char r, unsigned char g, unsigned char b){
			if(val > 1.f || val < 0.f)
				return false;
			if(color_nodes.find(val) == color_nodes.end())
				return false;
			color_nodes[val] = color(r,g,b);
			if(!valid && color_nodes.find(0.f) != color_nodes.end() && color_nodes.find(1.f) != color_nodes.end())
				valid = true;
			return true;
		}
		int get_rgb(float val, char &r, char &g, char &b){
			return get_rgb(val, 0., 1., r, g, b);
		}
		int get_rgb(float val, float min, float max, char &r, char &g, char &b){
			return 0;	
		}

		colormap_t cm_t;
		std::map<float, color> color_nodes;
		bool valid;
};

}}
