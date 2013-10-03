#ifndef _IO_H
#define _IO_H
#include <limits>
#include <cstddef> 
#include <fstream>
#include "bmp.h"
namespace cfdtd{

enum io_type{IO_BINARY,IO_TEXT,IO_IMAGE_BMP};

template<typename float_t>
int input(char* filename,float_t* data,io_type type,size_t max_size=std::numeric_limits<size_t>::max()){
   	std::ifstream ifs;
	size_t i;
	switch(type){
		case IO_BINARY:
   	             ifs.open(filename,std::ifstream::binary);
					i=0;
					while(!ifs.eof() && i<max_size){
						ifs.read((char*)(data+i),sizeof(float_t));
						++i;
					}
    	    ifs.close();
			break;
		case IO_TEXT:
            	    ifs.open(filename,std::ifstream::in);
    	   			i=0;
        			while(!ifs.eof() && i<max_size){
           	   	 	ifs>>data[i];
						++i;
       		 		}
        	ifs.close();
			break;
	}
        return 0;
};
template<typename float_t>
int output(char* filename,float_t* data,io_type type,size_t max_col=1,size_t max_row=1){
    std::ofstream ofs;
	switch(type){
		case IO_BINARY:
   	            	 ofs.open(filename,std::ofstream::binary);
   	            	 for(size_t j=0;j<max_row;++j)
   	        	             for(size_t i=0;i<max_col;++i)
   	    	                         ofs.write((char*)(data+i+j*max_col),sizeof(float_t));
   	  	   ofs.close();
		   break;
		case IO_TEXT:
                ofs.open(filename,std::ofstream::out);
                for(size_t j=0;j<max_row;++j){
                        for(size_t i=0;i<max_col;++i)
                                ofs<<data[i+j*max_col]<<"\t";
                        ofs<<std::endl;
                }
        	ofs.close();
			break;
		case IO_IMAGE_BMP:
			floats_to_bitmap_file(filename,data,max_row,max_col);
			break;
	}
        return 0;
};
}

#endif
