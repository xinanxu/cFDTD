#include <io/colormap.h>
namespace cfdtd{
colormap::colormap(colormap_t cm_t):cm_t(cm_t){
	switch(cm_t){
	case BLUE_RED:
		colors.emplace(0.f, color(0, 0, 255));
		colors.emplace(0.5f, color(255, 255, 255));
		colors.emplace(1.f, color(255, 0, 0));
		valid = true;
		break;
	case WHITE_BLACK:
		colors.emplace(0.f, color(255, 255, 255));
		colors.emplace(1.f, color(0, 0, 0));
		valid = true;
		break;
	case CUSTOMIZED:
		valid = false;
		break;
	}
}

bool colormap::insert(float val,char r,char g,char b){
	if(val > 1.f || val < 0.f)
		return false;
	if(colors.find(val) == colors.end())
		return false;
	colors.emplace(val,color(r,g,b));
	if(!valid && colors.find(0.f) != colors.end() && colors.find(1.f) != colors.end())
		valid = true;
	return true;
}

int colormap::get_rgb(float val, float min, float max, char &r, char &g, char &b){
	if (max <= min)
		return false;
	return get_rgb((val-min)/(max-min), r, g, b);
}
int colormap::get_rgb(float val, char &r, char &g, char &b){
	return false;//WIP
}
}
