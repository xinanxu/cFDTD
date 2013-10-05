#ifndef _VEC_H
#define _VEC_H
#include <type_traits>
namespace cfdtd{
enum field_component{Ex,Hx,Dx,Bx,Ey,Hy,Dy,By,Ez,Hz,Dz,Bz};
enum mat_component{CA,CB,DA,DB,EPS};
template<typename float_t,int dim>
class vec{
	static_assert(dim<=3,"Dimension too large");
	static_assert(dim>=1,"Dimension too small");
};
template<typename float_t>
class vec<float_t,1>{
	static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	vec();
	vec(const float_t& x_in):x(x_in){};
	float_t x;
	vec operator+(const vec<float_t,1> &b){return vec<float_t,1>(x+b.x);};
	vec operator+=(const vec<float_t,1> &b){x+=b.x;return *this;};
	vec operator-(const vec<float_t,1> &b){return vec<float_t,1>(x-b.x);};
	vec operator-=(const vec<float_t,1> &b){x-=b.x;return *this;};
	vec operator*(const float_t &b){return vec<float_t,1>(x*b);};
	vec operator*=(const float_t &b){x*=b;return *this;};
	vec operator/(const float_t &b){return vec<float_t,1>(x/b);};
	vec operator/=(const float_t &b){x/=b;return *this;};
	vec operator=(const vec<float_t,1> &b){x=b.x;return *this;};
};
template<typename float_t>
class vec<float_t,2>{
	static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	vec();
	vec(const float_t& x_in,const float_t& y_in):x(x_in),y(y_in){};
	float_t x;
	float_t y;
	vec operator+(const vec<float_t,2> &b){return vec<float_t,2>(x+b.x,y+b.y);};
	vec operator+=(const vec<float_t,2> &b){x+=b.x;y+=b.y;return *this;};
	vec operator-(const vec<float_t,2> &b){return vec<float_t,2>(x-b.x,y-b.y);};
	vec operator-=(const vec<float_t,2> &b){x-=b.x;y-=b.y;return *this;};
	vec operator*(const float_t &b){return vec<float_t,2>(x*b,y*b);};
	vec operator*=(const float_t &b){x*=b;y*=b;return *this;};
	vec operator/(const float_t &b){return vec<float_t,2>(x/b,y/b);};
	vec operator/=(const float_t &b){x/=b;y/=b;return *this;};
	vec operator=(const vec<float_t,2> &b){x=b.x;y=b.y;return *this;};
	
};
template<typename float_t>
class vec<float_t,3>{
	static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
public:
	vec();
	vec(const float_t& x_in,const float_t& y_in,const float_t& z_in):x(x_in),y(y_in),z(z_in){};
	float_t x;
	float_t y;
	float_t z;
	vec operator+(const vec<float_t,3> &b){return vec<float_t,3>(x+b.x,y+b.y,z+b.z);};
	vec operator+=(const vec<float_t,3> &b){x+=b.x;y+=b.y;z+=b.z;return *this;};
	vec operator-(const vec<float_t,3> &b){return vec<float_t,3>(x-b.x,y-b.y,z-b.z);};
	vec operator-=(const vec<float_t,3> &b){x-=b.x;y-=b.y;z-=b.z;return *this;};
	vec operator*(const float_t &b){return vec<float_t,3>(x*b,y*b,z*b);};
	vec operator*=(const float_t &b){x*=b;y*=b;z*=b;return *this;};
	vec operator/(const float_t &b){return vec<float_t,3>(x/b,y/b,z/b);};
	vec operator/=(const float_t &b){x/=b;y/=b;z/=b;return *this;};
	vec operator=(const vec<float_t,3> &b){x=b.x;y=b.y;z=b.z;return *this;};
};
}
#endif
