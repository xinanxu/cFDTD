#ifndef _SOURCE_H
#define _SOURCE_H
#include <cassert>
#include <type_traits>
#include <limits>
#include <cmath>
namespace cfdtd{
template<typename float_t>
class source{
		static_assert(std::is_floating_point<float_t>::value,"Your type is not a float type...");
	public:
		source(const field_component& component_in,const float_t& duration_in):duration(duration_in),component(component_in){
			assert(duration_in>=0.);
		};
		float_t duration;
		field_component component;
		virtual float_t get_intensity(float_t t)=0;
		virtual ~source(){};
};
///E=Amp*sin(2*pi*f*t+phi)
template<typename float_t>
class sinc_source:public source<float_t>{
	public:
		float_t frequency;
		float_t amplitude;
		float_t phase;
		sinc_source(const field_component& component_in,const float_t& frequency_in,const float_t& amplitude_in=1., const float_t& duration_in=std::numeric_limits<float_t>::max(),const float_t& phase_in=0.):source<float_t>(component_in,duration_in),frequency(frequency_in),amplitude(amplitude_in),phase(phase_in){};
		virtual float_t get_intensity(float_t t){
			if(t<0)
				return amplitude*sin(phase);
			if(t>source<float_t>::duration)
				return amplitude*sin(2*M_PI*frequency*source<float_t>::duration+phase);
			return amplitude*sin(2*M_PI*frequency*t+phase);
		};
		virtual ~sinc_source(){};
};
///E=exp(-0.5*((t-mu)/sigma)^2)*Amp*sin(2*pi*f*(t-mu)+phi)
template<typename float_t>
class gaussian_source:public source<float_t>{
	public:
		static constexpr float_t GAUSSIAN_DURATION_SCALE=3.;
		float_t frequency;
		float_t amplitude;
		float_t phase;
		float_t fwidth;
		float_t sigma;
		gaussian_source(const field_component& component_in,const float_t& frequency_in,float_t fwidth_in,const float_t& amplitude_in=1., const float_t& phase_in=0.):source<float_t>(component_in,0.),frequency(frequency_in),amplitude(amplitude_in),phase(phase_in),fwidth(fwidth_in){
			sigma=M_PI/fwidth;
			source<float_t>::duration=GAUSSIAN_DURATION_SCALE*2.*sigma;
		};
		virtual float_t get_intensity(float_t t){
			if(t<0)
				return 0;
			if(t>source<float_t>::duration)
				t=source<float_t>::duration;
			return exp(-0.5*((t-GAUSSIAN_DURATION_SCALE*sigma)*(t-GAUSSIAN_DURATION_SCALE*sigma)/sigma/sigma))*amplitude*sin(2*M_PI*frequency*(t-GAUSSIAN_DURATION_SCALE)+phase);
		};
};
}

#endif
