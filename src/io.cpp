#include "io.h"
#include <fstream>
using namespace std;
namespace cfdtd{
template<typename float_t>
int input(char* filename,float_t* data,io_type type,size_t max_size){
	ifstream ifs;
	if(type==IO_BINARY)
		ifs=ifstream(filename,ifstream::binary);
	if(type==IO_TEXT)
		ifs=ifstream(filename,ifstream::in);
	size_t i=0;
	while(!ifs.eof() && --max_size>=0){
		ifs>>data[i];
	}
	ifs.close();
	return 0;
}
template<typename float_t>
int output(char* filename,float_t* data,io_type type,size_t max_col,size_t max_row){
	ofstream ofs;
	if(type==IO_BINARY){
		ofs=ofstream(filename,ofstream::binary);
		for(size_t j=0;j<max_row;++j){
			for(size_t i=0;i<max_col;++i)
				ofs<<data[i+j*max_col]<<"\t";
			ofs<<endl;
		}
	}
	if(type==IO_TEXT){
		ofs=ofstream(filename,ofstream::out);
		for(size_t j=0;j<max_row;++j)
			for(size_t i=0;i<max_col;++i)
				ofs<<data[i+j*max_col];
	}
	ofs.close();
	return 0;
}
}
