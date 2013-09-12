#ifndef _MATERIAL_H
#define _MATERIAL_H
#include <type_traits>
namespace cfdtd{
class material{
public:
	material(){};
	virtual bool is_simple()=0;
	virtual ~material(){};
};

///Simple material class, supporting any float types of dielectricity
template<typename float_t>
class dielectric:public material{
	static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	float_t eps;
	dielectric():material(),eps(1){};
	dielectric(float_t eps_in):material(),eps(eps_in){};
	virtual bool is_simple(){return true;};
	virtual ~dielectric(){};
};
}
#endif
