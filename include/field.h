#ifndef _FIELD_H
#define _FIELD_H
#include "config.h"
#include "structure.h"
#include <type_traits>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "boundary.h"
#include "source.h"
#include "io/io.h"
#include <tuple>
#include <cstdio>
#include <omp.h>

///Cache optimization
#ifdef LEVEL3_CACHE_SIZE
const unsigned int shared_cache=LEVEL3_CACHE_SIZE;
#define SHARED_CACHE
#ifdef LEVEL2_CACHE_SIZE
const unsigned int seperated_cache=LEVEL2_CACHE_SIZE;
#define SEPERATED_CACHE
#ifdef LEVEL1_DCACHE_SIZE
const unsigned int seperated_cache_inner=LEVEL1_DCACHE_SIZE;
#define SEPERATED_CACHE_INNER
#else
#error "Could not find definition of LEVEL1_DCACHE_SIZE"
#endif
#else
#error "Could not find definition of LEVEL2_CACHE_SIZE"
#endif
#else
#warning "Could not find definition of LEVEL3_CACHE_SIZE, is your machine that old???"
#ifdef  LEVEL2_CACHE_SIZE
const unsigned int shared_cache=LEVEL2_CACHE_SIZE;
#define SHARED_CACHE
#ifdef  LEVEL_1_DCACHE_SIZE
const unsigned int seperated_cache=LEVEL1_DCACHE_SIZE;
#define SEPERATED_CACHE
#else
#error "Could not find definition of LEVEL1_DCACHE_SIZE"
#endif
#else
#error "Could not find definition of LEVEL2_CACHE_SIZE, cFDTD is not able to build on machine that old???"
#endif
#endif

static_assert(shared_cache!=0,"Do not use 0 as CACHE_SIZE");
static_assert(seperated_cache!=0,"Do not use 0 as CACHE_SIZE");
#ifdef SEPERATED_CACHE_INNER
static_assert(seperated_cache_inner!=0,"Do not use 0 as CACHE_SIZE");
#endif
static_assert(shared_cache>seperated_cache,"High level cache should be larger than low level cache");
#ifdef SEPERATED_CACHE_INNER
static_assert(seperated_cache>seperated_cache_inner,"High level cache should be larger than low level cache");
#endif
	
///loop optimization for cache usage
#define DEFINE_TRIPLE_LOOP(DATA_TYPES_NEED) \
	const size_t seperated_loop_size_inner=static_cast<const size_t>(sqrt(seperated_cache_inner/sizeof(float_t)/DATA_TYPES_NEED))-2;\
	static_assert(seperated_loop_size_inner>1,"L1 cache size too small");\
	const size_t seperated_loop_num=(static_cast<const size_t>(sqrt(seperated_cache/sizeof(float_t)/DATA_TYPES_NEED))-2)/seperated_loop_size_inner;\
	const size_t seperated_loop_size=seperated_loop_num*seperated_loop_size_inner;\
	static_assert(seperated_loop_num>1,"L2 cache size too small");\
	const size_t shared_loop_num=(static_cast<const size_t>(sqrt(shared_cache/sizeof(float_t)/DATA_TYPES_NEED))-2)/seperated_loop_size;\
	const size_t shared_loop_size=shared_loop_num*seperated_loop_size;\
	static_assert(shared_loop_num>1,"L3 cache size too small")


#define DEFINE_DUAL_LOOP(DATA_TYPES_NEED) \
	const size_t seperated_loop_size=static_cast<const size_t>(sqrt(seperated_cache/sizeof(float_t)/DATA_TYPES_NEED))-2;\
	static_assert(seperated_loop_size>1,"L1 cache size too small");\
	const size_t shared_loop_num=(static_cast<const size_t>(sqrt(shared_cache/sizeof(float_t)/DATA_TYPES_NEED))-2)/seperated_loop_size;\
	const size_t shared_loop_size=shared_loop_num*seperated_loop_size;\
	static_assert(shared_loop_num>1,"L2 cache size too small")

#define DIVIDE_CEIL(x,y) (x+y-1)/y

#define EXEC_DUAL_LOOP(STATEMENTS) \

#define EXEC_TRIPLE_LOOP(STATEMENTS) \


#define PARALLEL_STEP(x,y) x ## _ ## y()
#define EXEC_STEP(x,y) switch(y){\
		case SERIAL:\
		PARALLEL_STEP(x,SERIAL);\
		break;\
		case OMP:\
		PARALLEL_STEP(x,OMP);\
		break;\
		case OCL:\
		PARALLEL_STEP(x,OCL);\
		break;\
		}
#define DECLARE_STEP(x) \
		void x ## _SERIAL();\
		void x ## _OMP();\
		void x ## _OCL()

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
		time=0.;
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
		time=0.;
        if(s->is_simple())                                                                                 
                return;     
	};
	void step(PARALLEL_TYPE pt=SERIAL);
	DECLARE_STEP(step_TE);
	DECLARE_STEP(step_TM);
	DECLARE_STEP(step_TMTE);
	DECLARE_STEP(step_source);
	void stepto(float_t t,PARALLEL_TYPE pt=SERIAL);
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
		time=0.;
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
//Two Dimension, Serial
////////////////////////////////////////////////////
template<typename float_t>
void field<float_t,2>::step_source_SERIAL(){
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
void field<float_t,2>::step_TM_SERIAL(){
	switch(b.b_type){
		case ZERO:
			for(size_t j=1;j<s->grids[Y]-1;++j)
			for(size_t i=1;i<s->grids[X]-1;++i){
				data[Hx][i+j*s->grids[X]]+=CD*(data[Ez][i+j*s->grids[X]]-data[Ez][i+(j+1)*s->grids[X]]);
				data[Hy][i+j*s->grids[X]]+=CD*(data[Ez][i+1+j*s->grids[X]]-data[Ez][i+j*s->grids[X]]);
			}
			for(size_t j=1;j<s->grids[Y]-1;++j)
			for(size_t i=1;i<s->grids[X]-1;++i){
				data[Ez][i+j*s->grids[X]]+=mat[CB][i+j*s->grids[X]]*CD*(data[Hy][i+j*s->grids[X]]-data[Hy][i-1+j*s->grids[X]]+data[Hx][i+(j-1)*s->grids[X]]-data[Hx][i+j*s->grids[X]]);
			}
			break;
	}
}
template<typename float_t>
void field<float_t,2>::step_TE_SERIAL(){
	switch(b.b_type){
		case ZERO:
			for(size_t j=1;j<s->grids[Y]-1;++j)
			for(size_t i=1;i<s->grids[X]-1;++i){
				data[Hz][i+j*s->grids[X]]+=CD*(data[Ex][i+(j+1)*s->grids[X]]-data[Ex][i+j*s->grids[X]]+data[Ey][i+j*s->grids[X]]-data[Ey][i+1+j*s->grids[X]]);
			}
			for(size_t j=1;j<s->grids[Y]-1;++j)
			for(size_t i=1;i<s->grids[X]-1;++i){
				data[Ex][i+j*s->grids[X]]+=CD*0.5*(mat[CB][i+j*s->grids[X]]+mat[CB][i+1+j*s->grids[X]])*(data[Hz][i+j*s->grids[X]]-data[Hz][i+(j-1)*s->grids[X]]);
				data[Ey][i+j*s->grids[X]]+=CD*0.5*(mat[CB][i+j*s->grids[X]]+mat[CB][i+(j+1)*s->grids[X]])*(data[Hz][i-1+j*s->grids[X]]-data[Hz][i+j*s->grids[X]]);
			}
			break;
	}
}
template<typename float_t>
void field<float_t,2>::step_TMTE_SERIAL(){
	switch(b.b_type){
		case ZERO:
			for(size_t j=1;j<s->grids[Y]-1;++j)
			for(size_t i=1;i<s->grids[X]-1;++i){
				data[Hx][i+j*s->grids[X]]+=CD*(data[Ez][i+j*s->grids[X]]-data[Ez][i+(j+1)*s->grids[X]]);
				data[Hy][i+j*s->grids[X]]+=CD*(data[Ez][i+1+j*s->grids[X]]-data[Ez][i+j*s->grids[X]]);
				data[Hz][i+j*s->grids[X]]+=CD*(data[Ex][i+(j+1)*s->grids[X]]-data[Ex][i+j*s->grids[X]]+data[Ey][i+j*s->grids[X]]-data[Ey][i+1+j*s->grids[X]]);
			}
			for(size_t j=1;j<s->grids[Y]-1;++j)
			for(size_t i=1;i<s->grids[X]-1;++i){
				data[Ex][i+j*s->grids[X]]+=CD*0.5*(mat[CB][i+j*s->grids[X]]+mat[CB][i+1+j*s->grids[X]])*(data[Hz][i+j*s->grids[X]]-data[Hz][i+(j-1)*s->grids[X]]);
				data[Ey][i+j*s->grids[X]]+=CD*0.5*(mat[CB][i+j*s->grids[X]]+mat[CB][i+(j+1)*s->grids[X]])*(data[Hz][i-1+j*s->grids[X]]-data[Hz][i+j*s->grids[X]]);
				data[Ez][i+j*s->grids[X]]+=mat[CB][i+j*s->grids[X]]*CD*(data[Hy][i+j*s->grids[X]]-data[Hy][i-1+j*s->grids[X]]+data[Hx][i+(j-1)*s->grids[X]]-data[Hx][i+j*s->grids[X]]);
			}
			break;
	}
}
/////////////////////////////////////////////////////
//Two Dimension, OMP
////////////////////////////////////////////////////
template<typename float_t>
void field<float_t,2>::step_source_OMP(){
	step_source_SERIAL();
}
template<typename float_t>
void field<float_t,2>::step_TM_OMP(){

#ifdef SEPERATED_CACHE_INNER
///For TM, only 4 fields(CB,Ez,Hx,Hy) will be used
	DEFINE_TRIPLE_LOOP(4);
#else
	DEFINE_DUAL_LOOP(4);
#endif

	switch(b.b_type){
		case ZERO:
			for(size_t L3_index_x=0 ; L3_index_x < DIVIDE_CEIL(s->grids[X]-2,shared_loop_size); L3_index_x++)
			for(size_t L3_index_y=0 ; L3_index_y < DIVIDE_CEIL(s->grids[Y]-2,shared_loop_size); L3_index_y++)
			{

				size_t L3_x_begin=L3_index_x*shared_loop_size+1;
				size_t L3_x_end=(L3_index_x+1)*shared_loop_size+1;
				if (L3_x_end>s->grids[X]-1) L3_x_end=s->grids[X]-1;
				assert(L3_x_end>L3_x_begin);

				size_t L3_y_begin=L3_index_y*shared_loop_size+1;
				size_t L3_y_end=(L3_index_y+1)*shared_loop_size+1;
				if (L3_y_end>s->grids[Y]-1) L3_y_end=s->grids[Y]-1;
				assert(L3_y_end>L3_y_begin);
				
				for(size_t L2_index_x=0 ; L2_index_x < DIVIDE_CEIL(L3_x_end-L3_x_begin,seperated_loop_size) ; L2_index_x++)
				for(size_t L2_index_y=0 ; L2_index_y < DIVIDE_CEIL(L3_y_end-L3_y_begin,seperated_loop_size) ; L2_index_y++)
				{
					size_t L2_x_begin=L2_index_x*seperated_loop_size;
					size_t L2_x_end=(L2_index_x+1)*seperated_loop_size;
					if (L2_x_end>L3_x_end-L3_x_begin) L2_x_end=L3_x_end-L3_x_begin;
					assert(L2_x_end>L2_x_begin);
	
					size_t L2_y_begin=L2_index_y*seperated_loop_size;
					size_t L2_y_end=(L2_index_y+1)*seperated_loop_size;
					if (L2_y_end>L3_y_end-L3_y_begin) L2_y_end=L3_y_end-L3_y_begin;
					assert(L2_y_end>L2_y_begin);

#ifdef SEPERATED_CACHE_INNER
					for(size_t L1_index_x=0 ; L1_index_x < DIVIDE_CEIL(L2_x_end-L2_x_begin,seperated_loop_size_inner) ; L1_index_x++)
					for(size_t L1_index_y=0 ; L1_index_y < DIVIDE_CEIL(L2_y_end-L2_y_begin,seperated_loop_size_inner) ; L1_index_y++)
					{
						size_t L1_x_begin=L1_index_x*seperated_loop_size_inner;
						size_t L1_x_end=(L1_index_x+1)*seperated_loop_size_inner;
						if (L1_x_end>L2_x_end-L2_x_begin) L1_x_end=L2_x_end-L2_x_begin;
						assert(L1_x_end>L1_x_begin);
						
						size_t L1_y_begin=L1_index_y*seperated_loop_size_inner;
						size_t L1_y_end=(L1_index_y+1)*seperated_loop_size_inner;
						if (L1_y_end>L2_y_end-L2_y_begin) L1_y_end=L2_y_end-L2_y_begin;
						assert(L1_y_end>L1_y_begin);

						size_t x0=L3_x_begin+L2_x_begin+L1_x_begin;
						size_t y0=L3_y_begin+L2_y_begin+L1_y_begin;
						size_t entry=x0+y0*s->grids[X];

						for(size_t j=0; j < L1_y_end-L1_y_begin ; j++)
						for(size_t i=0; i < L1_x_end-L1_x_begin ; i++)
						{
//----------------->
							data[Hx][entry+i+j*s->grids[X]]+=CD*(data[Ez][entry+i+j*s->grids[X]]-data[Ez][entry+i+(j+1)*s->grids[X]]);
							data[Hy][entry+i+j*s->grids[X]]+=CD*(data[Ez][entry+i+1+j*s->grids[X]]-data[Ez][entry+i+j*s->grids[X]]);
						}
					}
#else

					size_t x0=L3_x_begin+L2_x_begin;
					size_t y0=L3_y_begin+L2_y_begin;
					size_t entry=x0+y0*s->grids[X];
					for(size_t j=0; j < L2_y_end-L2_y_begin ; j++)
					for(size_t i=0; i < L2_x_end-L2_x_begin ; i++)
					{
//----------------->
						data[Hx][entry+i+j*s->grids[X]]+=CD*(data[Ez][entry+i+j*s->grids[X]]-data[Ez][entry+i+(j+1)*s->grids[X]]);
						data[Hy][entry+i+j*s->grids[X]]+=CD*(data[Ez][entry+i+1+j*s->grids[X]]-data[Ez][entry+i+j*s->grids[X]]);
					}
#endif
				}
			}

			for(size_t L3_index_x=0 ; L3_index_x < DIVIDE_CEIL(s->grids[X]-2,shared_loop_size); L3_index_x++)
			for(size_t L3_index_y=0 ; L3_index_y < DIVIDE_CEIL(s->grids[Y]-2,shared_loop_size); L3_index_y++)
			{

				size_t L3_x_begin=L3_index_x*shared_loop_size+1;
				size_t L3_x_end=(L3_index_x+1)*shared_loop_size+1;
				if (L3_x_end>s->grids[X]-1) L3_x_end=s->grids[X]-1;
				assert(L3_x_end>L3_x_begin);

				size_t L3_y_begin=L3_index_y*shared_loop_size+1;
				size_t L3_y_end=(L3_index_y+1)*shared_loop_size+1;
				if (L3_y_end>s->grids[Y]-1) L3_y_end=s->grids[Y]-1;
				assert(L3_y_end>L3_y_begin);
				
				for(size_t L2_index_x=0 ; L2_index_x < DIVIDE_CEIL(L3_x_end-L3_x_begin,seperated_loop_size) ; L2_index_x++)
				for(size_t L2_index_y=0 ; L2_index_y < DIVIDE_CEIL(L3_y_end-L3_y_begin,seperated_loop_size) ; L2_index_y++)
				{
					size_t L2_x_begin=L2_index_x*seperated_loop_size;
					size_t L2_x_end=(L2_index_x+1)*seperated_loop_size;
					if (L2_x_end>L3_x_end-L3_x_begin) L2_x_end=L3_x_end-L3_x_begin;
					assert(L2_x_end>L2_x_begin);
	
					size_t L2_y_begin=L2_index_y*seperated_loop_size;
					size_t L2_y_end=(L2_index_y+1)*seperated_loop_size;
					if (L2_y_end>L3_y_end-L3_y_begin) L2_y_end=L3_y_end-L3_y_begin;
					assert(L2_y_end>L2_y_begin);

#ifdef SEPERATED_CACHE_INNER
					for(size_t L1_index_x=0 ; L1_index_x < DIVIDE_CEIL(L2_x_end-L2_x_begin,seperated_loop_size_inner) ; L1_index_x++)
					for(size_t L1_index_y=0 ; L1_index_y < DIVIDE_CEIL(L2_y_end-L2_y_begin,seperated_loop_size_inner) ; L1_index_y++)
					{
						size_t L1_x_begin=L1_index_x*seperated_loop_size_inner;
						size_t L1_x_end=(L1_index_x+1)*seperated_loop_size_inner;
						if (L1_x_end>L2_x_end-L2_x_begin) L1_x_end=L2_x_end-L2_x_begin;
						assert(L1_x_end>L1_x_begin);
						
						size_t L1_y_begin=L1_index_y*seperated_loop_size_inner;
						size_t L1_y_end=(L1_index_y+1)*seperated_loop_size_inner;
						if (L1_y_end>L2_y_end-L2_y_begin) L1_y_end=L2_y_end-L2_y_begin;
						assert(L1_y_end>L1_y_begin);

						size_t x0=L3_x_begin+L2_x_begin+L1_x_begin;
						size_t y0=L3_y_begin+L2_y_begin+L1_y_begin;
						size_t entry=x0+y0*s->grids[X];

						for(size_t j=0; j < L1_y_end-L1_y_begin ; j++)
						for(size_t i=0; i < L1_x_end-L1_x_begin ; i++)
						{
//----------------->
						data[Ez][entry+i+j*s->grids[X]]+=mat[CB][entry+i+j*s->grids[X]]*CD*(data[Hy][entry+i+j*s->grids[X]]-data[Hy][entry+i-1+j*s->grids[X]]+data[Hx][entry+i+(j-1)*s->grids[X]]-data[Hx][entry+i+j*s->grids[X]]);
						}
					}
#else

					size_t x0=L3_x_begin+L2_x_begin;
					size_t y0=L3_y_begin+L2_y_begin;
					size_t entry=x0+y0*s->grids[X];
					for(size_t j=0; j < L2_y_end-L2_y_begin ; j++)
					for(size_t i=0; i < L2_x_end-L2_x_begin ; i++)
					{
//----------------->
						data[Ez][entry+i+j*s->grids[X]]+=mat[CB][entry+i+j*s->grids[X]]*CD*(data[Hy][entry+i+j*s->grids[X]]-data[Hy][entry+i-1+j*s->grids[X]]+data[Hx][entry+i+(j-1)*s->grids[X]]-data[Hx][entry+i+j*s->grids[X]]);
					}
#endif
				}
			}
			break;
	}
}
template<typename float_t>
void field<float_t,2>::step_TE_OMP(){
#ifdef SEPERATED_CACHE_INNER
///For TM, only 4 fields(CB,Ez,Hx,Hy) will be used
	DEFINE_TRIPLE_LOOP(4);
#define TRIPLE_LOOP
#else
	DEFINE_DUAL_LOOP(4);
#define DUAL_LOOP
#endif
}
template<typename float_t>
void field<float_t,2>::step_TMTE_OMP(){
}
/////////////////////////////////////////////////////
//Two Dimension, OCL
////////////////////////////////////////////////////
template<typename float_t>
void field<float_t,2>::step_source_OCL(){
}
template<typename float_t>
void field<float_t,2>::step_TM_OCL(){
}
template<typename float_t>
void field<float_t,2>::step_TE_OCL(){
}
template<typename float_t>
void field<float_t,2>::step_TMTE_OCL(){
}
/////////////////////////////////////////////////////
//Two Dimension, step function, general
////////////////////////////////////////////////////
template<typename float_t>
void field<float_t,2>::step(PARALLEL_TYPE pt){
	if(s->is_simple()){
		EXEC_STEP(step_source,pt);
		switch(p_mode){
			case TMTE:
				EXEC_STEP(step_TMTE,pt);
				break;
			case TM:
				EXEC_STEP(step_TM,pt);
				break;
			case TE:
				EXEC_STEP(step_TE,pt);
				break;
		}
	
	}
	time+=dt;
}
template<typename float_t>
void field<float_t,2>::stepto(float_t t,PARALLEL_TYPE pt){
	double begin=omp_get_wtime();
	double zero=begin;
	int count=0;
	printf("[CFDTD] --Start-- Simulation time began from: %8.2f\n",time);
	while(t>time)
	{
		step(pt);
		++count;
		double time_elapsed=(omp_get_wtime()-begin);
		if(time_elapsed>=3){
			printf("[CFDTD] [Running] Stepped to simulation time: %8.2f | speed %13.6f sec/step | %8.2f secs to finish...\n",time,time_elapsed/count,(t-time)/dt*time_elapsed/count);
			begin=omp_get_wtime();
			count=0;
		}
	}
	printf("[CFDTD] ---End--- Simulation time reached to: %8.2f | Used CPU time: %8.2f secs\n",time,omp_get_wtime()-zero);
}
}
#endif
