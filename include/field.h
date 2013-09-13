#ifndef _FIELD_H
#define _FIELD_H
#include "structure.h"
#include <type_traits>
#include <cassert>

///Cache optimization
#ifdef LEVEL3_CACHE_SIZE
#define CACHE_SIZE LEVEL2_CACHE_SIZE
#else
#ifdef LEVEL2_CACHE_SIZE
#define CACHE_SIZE (LEVEL2_CACHE_SIZE/_NPROCESSORS_ONLN
#else
#ifdef LEVEL1_CACHE_SIZE
#define CACHE_SIZE LEVEL1_CACHE_SIZE
#endif

namespace cfdtd{
template<typename float_t,int dim>
class field{
static_assert(dim<=3,"Dimension too large");
static_assert(dim>=1,"Dimension too small");
static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	structure<float_t,dim>* s;
	boundary_layer b;
	float_t* data;
	field(structure<float_t,dim>* s_in,boundary_layer b_in);
	
};
}

#endif
