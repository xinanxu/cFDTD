#include "cfdtd.h"
#include <iostream>
#include <cassert>
using namespace std;
using namespace cfdtd;
dielectric<double> eps_fun1(const vec<double,1>& v){
	if (v.x>1)
		return dielectric<double>(12.0);
	return dielectric<double>(1.0);
}
dielectric<double> eps_fun2(const vec<double,2>& v){
	if (v.x>1)
		return dielectric<double>(12.0);
	if (v.y>1)
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
	custom_structure<double,1>* s1d=new custom_structure<double,1>(10,100);
	assert(s1d->interval=0.01&&s1d->edges[X]==10);
	assert(s1d->is_simple()==0);
	s1d->set_dielectric_func(eps_fun1);
	assert(s1d->is_simple()==1);
	custom_structure<double,2>* s2d=new custom_structure<double,2>(10,10,10);
	assert(s2d->interval==0.1&&s2d->edges[Y]==10);
	assert(s2d->is_simple()==0);
	s2d->set_dielectric_func(eps_fun2);
	assert(s2d->is_simple()==1);
	cout<<"!!!!!!Custom Structure Test Passed!!!!!!"<<endl;
	double* a=new double[3*4];
	double* b=new double[3*4];
	for(int i=0;i<12;++i) a[i]=i/10.;
	output<double>("temp.txt",a,IO_TEXT,3,4);
	input<double>("temp.txt",b,IO_TEXT,20);
	assert(b[4]==4./10.);
	output<double>("temp.dat",a,IO_BINARY,4,3);
	output<double>("temp.bmp",a,IO_IMAGE_BMP,4,3);
	input<double>("temp.dat",b,IO_BINARY);
	assert(b[4]==4./10.);
	field<double,2> f(s2d,boundary_layer<double,2>(1.0,NOTHING));
	output<double>("field2d.bmp",f.mat[CB],IO_IMAGE_BMP,100,100);
	cout<<"!!!!!!Basic IO test Passed!!!!!!"<<endl;
	return 0;
}
