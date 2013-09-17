#ifndef _FIELD_H
#define _FIELD_H
#include "structure.h"
#include <type_traits>
#include <cassert>
#include <cmath>
#include <algorithm>
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
	field(structure<float_t,dim>* s_in,boundary_layer<float_t,dim> b_in){
		s=s_in;                                                                                            
        b=b_in;                                                                                            
        material_init();                                                                                   
        for(int i=0;i<dim;++i){                                                                            
                data[Ex+4*dim]=new float_t[s->grids_total]();                                              
                data[Hx+4*dim]=new float_t[s->grids_total]();                                              
                data[Dx+4*dim]=NULL;                                                                       
                data[Bx+4*dim]=NULL;                                                                       
        }                                                                                                  
        if(s->is_simple())                                                                                 
                return;     
	};
private:
	void material_init(){
		       if(s->is_simple()){                                                                                
                mat[CA]=NULL;                                                                              
                mat[DA]=NULL;                                                                              
                mat[DB]=NULL;                                                                              
                mat[CB]=new float_t[s->grids_total]();                                                     
                switch(dim){                                                                               
                        case 1:                                                                            
                                for(unsigned i=0;i < s->grids[X];++i){                                     
                                        vec<float_t,1> position((i+0.5)*s->interval);
                                        dielectric<float_t>* mat_in=s->get_material(position);
                                        mat[CB][i]=1./mat_in->eps;
                                }
                                break;
                        case 2:
                                for(unsigned j=0;j < s->grids[Y];++j)
                                for(unsigned i=0;i < s->grids[X];++i){
                                        vec<float_t,2> position((i+0.5)*s->interval,(j+0.5)*s->interval);
                                        dielectric<float_t>* mat_in=s->get_material(position);
                                        mat[CB][i+j*s->grids[X]]=1./mat_in->eps;
                                }
                                break;
                        case 3:
                                for(unsigned k=0;k < s->grids[Z];++k)
                                for(unsigned j=0;j < s->grids[Y];++j)
                                for(unsigned i=0;i < s->grids[X];++i){
                                        vec<float_t,3> position((i+0.5)*s->interval,(j+0.5)*s->interval,(k+0.5)*s->interval);
                                        dielectric<float_t>* mat_in=s->get_material(position);
                                        mat[CB][i+(j+k*s->grids[Y])*s->grids[X]]=1./mat_in->eps;
                                }
                                break;
                        default:
                                assert(true);
                }
                float_t* max_eps_reverse=std::max_element(mat[CB],mat[CB]+s->grids_total);
                CD=std::sqrt(*max_eps_reverse)/std::sqrt(dim);
        }
	};
};
}

#endif
