#ifndef _BOUNDARY_H
#define _BOUNDARY_H

namespace cfdtd{
enum boundary_layer_type{NOTHING=0};
template<typename float_t,int dim>
class boundary_layer{
public:
	float_t[2*dim];
	boundary_layer_type b_type;
	boundary_layer(){};
	boundary_layer(float_t thickness,boundary_layer_type b_type_in){
		for(int i=0;i<2*dim;++i)
			float_t[i]=thickness;
		b_type=b_type_in;	
	};
};

}

#endif
