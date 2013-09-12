#ifndef _STRUCTURE_H
#define _STRUCTURE_H
#include "material.h"
#include "vec.h"
namespace cfdtd{
template<typename float_t,int dim>
class structure{
public:
	structure(){};
	virtual material* get_material(const vec<float_t,dim>& v)=0;
};
}
#endif
