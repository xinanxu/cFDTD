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
	cout<<"!!!!!!Basic Test Passed!!!!!!"<<endl;
	return 0;
}
