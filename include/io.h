#ifndef _IO_H
#define _IO_H
#include <limits>
#include <cstddef> 
namespace cfdtd{

enum io_type{IO_BINARY,IO_TEXT,IO_IMAGE_BMP};

template<typename float_t>
int input(char* filename,float_t* data,io_type type,size_t max_size=std::numeric_limits<size_t>::max());
template<typename float_t>
int output(char* filename,float_t* data,io_type type,size_t max_col=1,size_t max_row=1);


}

#endif
