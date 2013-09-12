#include "cfdtd.h"
#include <iostream>
#include <cassert>
using namespace std;
using namespace cfdtd;
dielectric<double> eps(const vec<double,1>& v){
	if (v.x>1)
		return dielectric<double>(12.0);
	return dielectric<double>(1.0);
}
int main(){

	dielectric<float> airF(1.0);
	dielectric<double> airD(1.0);
	dielectric<long double> airLD(1.0);
	assert(airF.is_simple());
	assert(airD.is_simple());
	assert(airLD.is_simple());
	cout<<"!!!!!!Material Test Passed!!!!!!"<<endl;
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
	assert(v1_3D.x==-14.0&&v1_3D.z==-18.0);
	cout<<"!!!!!!Vector Test Passed!!!!!!"<<endl;
	structure<double,1>* s1d=new custom_structure<double,1>(10,100);
	assert(s1d->interval=0.01&&s1d->edges[X]==10);
	assert(s1d->is_simple()==0);
	(dynamic_cast<custom_structure<double,1>* >(s1d))->set_dielectric_func(eps);
	assert(s1d->is_simple()==1);
	cout<<"!!!!!!Custom Structure Test Passed!!!!!!"<<endl;
	return 0;
}
