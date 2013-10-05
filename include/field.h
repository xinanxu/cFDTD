#ifndef _FIELD_H
#define _FIELD_H
#include "structure.h"
#include <type_traits>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "boundary.h"
#include "source.h"
#include "io/io.h"
#include <tuple>
#include <ctime>
#include <cstdio>

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

template<typename float_t,int dim>
class field{
static_assert(dim<=3,"Dimension too large");
static_assert(dim>=1,"Dimension too small");
};
template<typename float_t>
class field<float_t,1>{
static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	structure<float_t,1>* s;
	boundary_layer<float_t,1> b;
	float_t* data[4*3];
	float_t* mat[4];
	float_t CD;
	field(structure<float_t,1>* s_in,boundary_layer<float_t,1> b_in){
		s=s_in;                                                                                            
        b=b_in;                                                                                            
        material_init();                                                                                   
        for(int i=0;i<3;++i){                                                                            
                data[Ex+4*i]=new float_t[s->grids_total]();                                              
                data[Hx+4*i]=new float_t[s->grids_total]();                                              
                data[Dx+4*i]=NULL;                                                                       
                data[Bx+4*i]=NULL;                                                                       
        }                                                                                                  
        if(s->is_simple())                                                                                 
                return;     
	};
	void step();
	void stepto(float_t t);
private:
	void material_init();
};
enum polarization_mode{TM,TE,TMTE};
template<typename float_t>
class field<float_t,2>{
static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	structure<float_t,2>* s;
	boundary_layer<float_t,2> b;
	float_t* data[4*3];
	float_t* mat[4];
	float_t CD;
	field(structure<float_t,2>* s_in,boundary_layer<float_t,2> b_in,polarization_mode p_mode_in=TMTE){
		s=s_in;                                                                                            
        b=b_in;                                                                                            
		p_mode=p_mode_in;	
        material_init();                                                                                   
		switch(p_mode){
			case TMTE:
	        for(int i=0;i<3;++i){                                                                            
   		             data[Ex+4*i]=new float_t[s->grids_total]();                                              
   		             data[Hx+4*i]=new float_t[s->grids_total]();                                              
        	        data[Dx+4*i]=NULL;                                                                       
        	        data[Bx+4*i]=NULL;                                                                       
	        }                                                                                                  
			break;
			case TM:
	        for(int i=0;i<3;++i){                                                                            
        	        data[Dx+4*i]=NULL;                                                                       
        	        data[Bx+4*i]=NULL;                                                                       
	        }                                                                                                  
   		    data[Hz]=NULL;
   		    data[Ex]=NULL;
   		    data[Ey]=NULL;
   		    data[Ez]=new float_t[s->grids_total]();                                              
   		    data[Hx]=new float_t[s->grids_total]();                                              
   		    data[Hy]=new float_t[s->grids_total]();                                              
			break;
			case TE:
	        for(int i=0;i<3;++i){                                                                            
        	        data[Dx+4*i]=NULL;                                                                       
        	        data[Bx+4*i]=NULL;                                                                       
	        }                                                                                                  
   		    data[Hz]=new float_t[s->grids_total]();                                              
   		    data[Ex]=new float_t[s->grids_total]();                                              
   		    data[Ey]=new float_t[s->grids_total]();                                              
   		    data[Ez]=NULL;
   		    data[Hx]=NULL;
   		    data[Hy]=NULL;
			break;
		}
        if(s->is_simple())                                                                                 
                return;     
	};
	void step();
	void step_TE();
	void step_TM();
	void step_source();
	void stepto(float_t t);
	std::vector<std::tuple<vec<float_t,2>,source<float_t>*,float_t> > source_list;
	int output(mat_component mat_c,char* filename,io::io_type io_t=io::IO_IMAGE_BMP,io::color_map cm=io::WHITE_BLACK){
		if(mat_c==EPS){
			float_t *eps=new float_t[s->grids_total]();
			for(size_t i=0;i<s->grids_total;++i)
				eps[i]=1./mat[CB][i];
			int result=io::output(filename,eps,io_t,s->grids[X],s->grids[Y],cm);
			delete [] eps;
			return result;
		}
		return io::output(filename,mat[mat_c],io_t,s->grids[X],s->grids[Y],cm);
	};
	int output(field_component field_c,char* filename,io::io_type io_t=io::IO_IMAGE_BMP,io::color_map cm=io::BLUE_RED){
		return io::output(filename,data[field_c],io_t,s->grids[X],s->grids[Y],cm);
	};
	void push_source(vec<float_t,2> pos,source<float_t>* s,float_t begin_time=0.){
		source_list.push_back(std::make_tuple(pos,s,begin_time));
	};
private:
	float_t time;
	float_t dt;
	void material_init();
	polarization_mode p_mode;
	void pop_source(){
		if(!source_list.empty())
			source_list.pop_back();
	};
};
template<typename float_t>
class field<float_t,3>{
static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	structure<float_t,3>* s;
	boundary_layer<float_t,3> b;
	float_t* data[4*3];
	float_t* mat[4];
	float_t CD;
	field(structure<float_t,3>* s_in,boundary_layer<float_t,3> b_in){
		s=s_in;                                                                                            
        b=b_in;                                                                                            
        material_init();                                                                                   
        for(int i=0;i<3;++i){                                                                            
                data[Ex+4*i]=new float_t[s->grids_total]();                                              
                data[Hx+4*i]=new float_t[s->grids_total]();                                              
                data[Dx+4*i]=NULL;                                                                       
                data[Bx+4*i]=NULL;                                                                       
        }                                                                                                  
        if(s->is_simple())                                                                                 
                return;     
	};
	void step();
	void stepto(float_t t);
private:
	void material_init();
};
////////////////////////////////////////////////////
//Initialization for the field
///////////////////////////////////////////////////
template<typename float_t>
void field<float_t,1>::material_init(){
   		   if(s->is_simple()){                                                                                
   	    	   mat[CA]=NULL;                                                                              
   	    	   mat[DA]=NULL;                                                                              
   	    	   mat[DB]=NULL;                                                                              
   	    	   mat[CB]=new float_t[s->grids_total]();                                                     
   	    	   for(unsigned i=0;i < s->grids[X];++i){                                     
   	    	       vec<float_t,1> position((i+0.5)*s->interval);
   	    	       dielectric<float_t>* mat_in=static_cast<dielectric<float_t>*>(s->get_material(position));
   	    	       mat[CB][i]=1./mat_in->eps;
   	    	   }
   	    	   float_t* max_eps_reverse=std::max_element(mat[CB],mat[CB]+s->grids_total);
   	    	   CD=std::sqrt(*max_eps_reverse)/std::sqrt(1);
    		 }
}
template<typename float_t>
void field<float_t,2>::material_init(){
		       if(s->is_simple()){                                                                                
                mat[CA]=NULL;                                                                              
                mat[DA]=NULL;                                                                              
                mat[DB]=NULL;                                                                              
                mat[CB]=new float_t[s->grids_total]();                                                     
                                for(unsigned j=0;j < s->grids[Y];++j)
                                for(unsigned i=0;i < s->grids[X];++i){
                                        vec<float_t,2> position((i+0.5)*s->interval,(j+0.5)*s->interval);
                                        dielectric<float_t>* mat_in=static_cast<dielectric<float_t>*>(s->get_material(position));
                                        mat[CB][i+j*s->grids[X]]=1./mat_in->eps;
                                }
                float_t* max_eps_reverse=std::max_element(mat[CB],mat[CB]+s->grids_total);
                CD=std::sqrt(*max_eps_reverse)/std::sqrt(2);
                dt=CD*s->interval;
			   }
}
template<typename float_t>
void field<float_t,3>::material_init(){
		       if(s->is_simple()){                                                                                
                mat[CA]=NULL;                                                                              
                mat[DA]=NULL;                                                                              
                mat[DB]=NULL;                                                                              
                mat[CB]=new float_t[s->grids_total]();                                                     
                                for(unsigned k=0;k < s->grids[Z];++k)
                                for(unsigned j=0;j < s->grids[Y];++j)
                                for(unsigned i=0;i < s->grids[X];++i){
                                        vec<float_t,3> position((i+0.5)*s->interval,(j+0.5)*s->interval,(k+0.5)*s->interval);
                                        dielectric<float_t>* mat_in=static_cast<dielectric<float_t>*>(s->get_material(position));
                                        mat[CB][i+(j+k*s->grids[Y])*s->grids[X]]=1./mat_in->eps;
                                }
                float_t* max_eps_reverse=std::max_element(mat[CB],mat[CB]+s->grids_total);
                CD=std::sqrt(*max_eps_reverse)/std::sqrt(3);
			   }
}
/////////////////////////////////////////////////////
//real step function, simplest implementation
////////////////////////////////////////////////////
template<typename float_t>
void field<float_t,2>::step_source(){
	for(typename std::vector<std::tuple<vec<float_t,2>,source<float_t>*,float_t> >::iterator it=source_list.begin();it!=source_list.end();++it){
		source<float_t>* &src=std::get<1>(*it);
		vec<float_t,2> &p=std::get<0>(*it);
		if(time>=std::get<2>(*it)&&time<=std::get<2>(*it)+src->duration){
			if (data[src->component]!=NULL){
				data[src->component][s->get_index(p.x)+s->get_index(p.y)*s->grids[X]]+=src->get_intensity(time-std::get<2>(*it))-src->get_intensity(time-std::get<2>(*it)-s->interval);
			}
		}
	}
}
template<typename float_t>
void field<float_t,2>::step_TM(){
	switch(b.b_type){
		case ZERO:
			for(size_t j=1;j<s->grids[Y]-1;++j)
			for(size_t i=1;i<s->grids[X]-1;++i){
				data[Hx][i+j*s->grids[X]]+=CD*(data[Ez][i+j*s->grids[X]]-data[Ez][i+(j+1)*s->grids[X]]);
				data[Hy][i+j*s->grids[X]]+=CD*(data[Ez][i+1+j*s->grids[X]]-data[Ez][i+j*s->grids[X]]);
				data[Ez][i+j*s->grids[X]]+=mat[CB][i+j*s->grids[X]]*(data[Hy][i+j*s->grids[X]]-data[Hy][i-1+j*s->grids[X]]+data[Hx][i+(j-1)*s->grids[X]]-data[Hx][i+j*s->grids[X]]);
			}
			break;
	}
}
template<typename float_t>
void field<float_t,2>::step_TE(){
	switch(b.b_type){
		case ZERO:
			for(size_t j=1;j<s->grids[Y]-1;++j)
			for(size_t i=1;i<s->grids[X]-1;++i){
				data[Hz][i+j*s->grids[X]]+=CD*(data[Ex][i+(j+1)*s->grids[X]]-data[Ex][i+j*s->grids[X]]+data[Ey][i+j*s->grids[X]]-data[Ey][i+1+j*s->grids[X]]);
				data[Ex][i+j*s->grids[X]]+=CD*0.5*(mat[CB][i+j*s->grids[X]]+mat[CB][i+1+j*s->grids[X]])*(data[Hz][i+j*s->grids[X]]-data[Hz][i+(j-1)*s->grids[X]]);
				data[Ey][i+j*s->grids[X]]+=CD*0.5*(mat[CB][i+j*s->grids[X]]+mat[CB][i+(j+1)*s->grids[X]])*(data[Hz][i-1+j*s->grids[X]]-data[Hz][i+j*s->grids[X]]);
			}
			break;
	}
}
template<typename float_t>
void field<float_t,2>::step(){
	if(s->is_simple()){
		switch(p_mode){
			case TMTE:
				step_source();
				step_TM();
				step_TE();
				break;
			case TM:
				step_source();
				step_TM();
				break;
			case TE:
				step_source();
				step_TE();
				break;
		}
	
	}
	time+=dt;
}
template<typename float_t>
void field<float_t,2>::stepto(float_t t){
	clock_t begin=clock();
	int count=0;
	while(t>time)
	{
		step();
		++count;
		float_t time_elapsed=(clock()-begin)/CLOCKS_PER_SEC;
		if(time_elapsed>=3){
			printf("[CFDTD] Stepping to time: %8.2f | speed %10.6f sec/step | %8.2f secs to finish...\n",time,time_elapsed/count,(t-time)/dt*time_elapsed/count);
			begin=clock();
			count=0;
		}
	}
}

}
#endif
