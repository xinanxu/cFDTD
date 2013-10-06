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
	return 1.;
	if (v.x>5)
		return dielectric<double>(12.0);
	if (v.y>5)
		return dielectric<double>(12.0);
	return dielectric<double>(1.0);
}
int main(){
	custom_structure<double,2>* s1=new custom_structure<double,2>(10,10,20);
	custom_structure<double,2>* s2=new custom_structure<double,2>(10,10,20);
	s1->set_dielectric_func(eps_fun2);
	s2->set_dielectric_func(eps_fun2);
	field<double,2> f1(s1,boundary_layer<double,2>(1.0,ZERO),TM);
	field<double,2> f2(s2,boundary_layer<double,2>(1.0,ZERO),TM);
	gaussian_source<double>* src=new gaussian_source<double>(Ez,1.,0.1);
	f1.push_source(vec<double,2>(5,5),src,0.);
	f1.stepto(100,OMP);
	return 0;
}
