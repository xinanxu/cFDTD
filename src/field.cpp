#include "field.h"
#include "stdlib.h"
#include <algorithm>
#include <cmath>
using namespace std;
namespace cfdtd{

template<typename float_t,int dim>
field<float_t,dim>::field(structure<float_t,dim>* s_in,boundary_layer<float_t,dim> b_in){
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
}
template<typename float_t,int dim>
void field<float_t,dim>::material_init(){
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
		float_t* max_eps_reverse=max_element(mat[CB],mat[CB]+s->grids_total);
		CD=sqrt(*max_eps_reverse)/sqrt(dim);
	}
}
}
