#ifndef _STRUCTURE_H
#define _STRUCTURE_H
#include "material.h"
#include "vec.h"
#include <cstddef>
#include <type_traits>
#include <cassert>
#ifndef NULL
#define NULL 0
#endif
namespace{
	enum{X=0,Y=1,Z=2};
}
namespace cfdtd{
template<typename float_t,int dim>
class structure{
static_assert(dim<=3,"Dimension too large");
static_assert(dim>=1,"Dimension too small");
static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	structure(const float_t& x_size,const float_t& resolution_in):resolution(resolution_in),interval(1./resolution){
		static_assert(dim==1,"Dimension should be defined as 1");
		grids[X]=x_size*resolution;
		edges[X]=grids[X]*interval;	
		grids_total=grids[X];
	};
	structure(const float_t& x_size,const float_t& y_size,const float_t& resolution_in):resolution(resolution_in),interval(1./resolution){
		static_assert(dim==2,"Dimension should be defined as 2");
		grids[X]=x_size*resolution;
		edges[X]=grids[X]*interval;	
		grids[Y]=y_size*resolution;
		edges[Y]=grids[Y]*interval;	
		grids_total=grids[X]*grids[Y];
	};
	structure(const float_t& x_size,const float_t& y_size, const float_t& z_size,const float_t& resolution_in):resolution(resolution_in),interval(1./resolution){
		static_assert(dim==3,"Dimension should be defined as 3");
		grids[X]=x_size*resolution;
		edges[X]=grids[X]*interval;	
		grids[Y]=y_size*resolution;
		edges[Y]=grids[Y]*interval;	
		grids[Z]=z_size*resolution;
		edges[Z]=grids[Z]*interval;	
		grids_total=grids[X]*grids[Y]*grids[Z];
	};
	float_t edges[dim];//< Actual xyz_size
	float_t resolution;
	float_t interval;
	unsigned int grids[dim];//< num of nodes per axis
	unsigned int grids_total;
	virtual ~structure(){};
	virtual bool is_simple()=0;
	virtual material* get_material(const vec<float_t,dim>& v)=0;

	inline float_t get_pos(size_t num){return (0.5+num)*interval;};
	/// get the nearest index for a position, which is the most useful one
	inline size_t get_index(float_t pos){return pos*resolution;};
};

///Simple structure using function pointer
template<typename float_t,int dim>
class custom_structure:public structure<float_t,dim>{
public:
	custom_structure(const float_t& x_size,const float_t& resolution_in)\
		:structure<float_t,dim>(x_size,resolution_in){};
	custom_structure(const float_t& x_size,const float_t& y_size,const float_t& resolution_in)\
		:structure<float_t,dim>(x_size,y_size,resolution_in){};
	custom_structure(const float_t& x_size,const float_t& y_size, const float_t& z_size,\
			const float_t& resolution_in)\
		:structure<float_t,dim>(x_size,y_size,z_size,resolution_in){};
	void set_dielectric_func(dielectric<float_t> (*dielectric_func_in)(const vec<float_t,dim>& v)){
		assert(dielectric_func_in!=NULL);
		dielectric_func=dielectric_func_in;
		is_dielectric=true;
	};
	virtual bool is_simple(){return is_dielectric;};
	dielectric<float_t> (*dielectric_func)(const vec<float_t,dim>& v);
	bool is_dielectric;
	virtual material* get_material(const vec<float_t,dim>& v){
		assert(dielectric_func!=NULL);
		if(is_dielectric){
			dielectric<float_t>* mat=new dielectric<float_t>(dielectric_func(v));
			return (material*) mat;
		}
		return NULL;
	};
	virtual ~custom_structure(){};
};
}
#endif
