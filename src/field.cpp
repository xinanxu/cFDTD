#include "field.h"
#include "stdlib.h"
namespace cfdtd{
field::field(structure<float_t,dim>* s_in,boundary_layer b_in){};
	s=s_in;
	b=b_in;
	int status=posix_memalign((void**)&data,MEM_ALIGNMENT,
}
