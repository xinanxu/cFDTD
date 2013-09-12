#include "cfdtd.h"
#include <iostream>
#include <cassert>
using namespace std;
using namespace cfdtd;

int main(){

	dielectric<float> airF(1.0);
	dielectric<double> airD(1.0);
	dielectric<long double> airLD(1.0);
	assert(airF.is_simple());
	assert(airD.is_simple());
	assert(airLD.is_simple());
	vec<double,1> v1_1D(1.);
	vec<double,1> v2_1D(2.0);
	v1_1D+=v2_1D*6./3.;
	v1_1D/=5.;
	assert(v1_1D.x==1.0);
	vec<float,2> v1_2D(1.0,2.0);
	vec<float,2> v2_2D(3.0,4.0);
	v2_2D-=v1_2D*2;
	assert(v2_2D.x==1.0&&v2_2D.y==0.0);
	vec<float,3> v1_3D(1.0,2.0,3.0);
	vec<float,3> v2_3D(4.0,5.0,6.0);
	v1_3D-=v2_3D*2;
	v1_3D*=2;
	cout<<v1_3D.x<<v1_3D.z;
	assert(v1_3D.x==-14.0&&v1_3D.z==-18.0);
	cout<<"!!!!!!Basic Test Passed!!!!!!"<<endl;
	return 0;
}
