#pragma once
#include <iostream>
#include <map>

namespace cfdtd{
namespace io{

// For performance, all colormap calculations are in float precision

enum colormap_t{
	BLUE_RED,		// (0, 0, 255) - (255, 255, 255) - (255, 0, 0)
	WHITE_BLACK,	// (255, 255, 255) - (0, 0, 0)
	CUSTOMIZED
}

class colormap{
	public:
		colormap(colormap_t cm_t_in):cm_t(cm_t_in){
			switch(cm_t){
				case BLUE_RED:
					color_nodes.emplace(0.f, )
			}
		};
		int get_rgb(float val, char &r, char &g, char &b){
			return get_rgb(val, 0., 1., r, g, b);
		}
		int get_rgb(float val, float min, float max, char &r, char &g, char &b){
			
		}

		colormap_t cm_t;
		map<float, char[3]> color_nodes;
}

}
