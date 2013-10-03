#ifndef _SOURCE_H
#define _SOURCE_H
#include <cassert>
#include <type_traits>
#include "field.h"
#include <limits>
#include <cmath>
namespace cfdtd{
template<typename float_t>
class source{
	public:
		source(const field_component& component_in,const float_t& duration_in):duration(duration_in),component(component_in){
			static_assert(duration_in>=0.),"Source should have possitive duration time");
		};
		static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
		float_t duration;
		field_component component;
		virtual float_t get_intensity(float_t t)=0;
		~source(){};
};
///E=Amp*sin(2*pi*f*t+phi)
template<typename float_t>
class sinc_source:public source<float_t>{
	public:
		float_t frequency;
		float_t phase;
		float_t amplitude;
		sinc_source(const field_component& component_in,const float_t& frequency_in,const float_t& amplitude_in, const float_t& duration=std::numeric_limits<float_t>::max(),const float_t& phase_in=0.):source(component_in,duration_in),frequency(frequency_in),amplitude(amplitude_in),phase{phase_in};
		virtual float_t get_intensity(float_t t){
			return amplitude*sin(2*M_PI*frequency*t+phase);
		};
};
}

#endif
