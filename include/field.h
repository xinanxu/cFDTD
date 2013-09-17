#ifndef _FIELD_H
#define _FIELD_H
#include "structure.h"
#include <type_traits>
#include <cassert>
#include "boundary.h"

///Cache optimization
#ifdef LEVEL3_CACHE_SIZE
#define CACHE_SIZE LEVEL2_CACHE_SIZE
#elif defined LEVEL2_CACHE_SIZE
#define CACHE_SIZE (LEVEL2_CACHE_SIZE/_NPROCESSORS_ONLN
#elif defined LEVEL1_CACHE_SIZE
#define CACHE_SIZE LEVEL1_CACHE_SIZE
#elif !defined CACHE_SIZE
#define CACHE_SIZE 65536
#endif

namespace cfdtd{
enum field_component{Ex,Hx,Dx,Bx,Ey,Hy,Dy,By,Ez,Hz,Dz,Bz};
enum mat_component{CA,CB,DA,DB};
template<typename float_t,int dim>
class field{
static_assert(dim<=3,"Dimension too large");
static_assert(dim>=1,"Dimension too small");
static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	structure<float_t,dim>* s;
	boundary_layer<float_t,dim> b;
	float_t* data[4*dim];
	float_t* mat[4];
	float_t CD;
	field(structure<float_t,dim>* s_in,boundary_layer<float_t,dim> b_in);
private:
	void material_init();
};
}

#endif
