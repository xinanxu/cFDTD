#ifndef _IO_H
#define _IO_H
#include <limits>
#include <cstddef> 
#include <fstream>
namespace cfdtd{

enum io_type{IO_BINARY,IO_TEXT,IO_IMAGE_BMP};

template<typename float_t>
int input(char* filename,float_t* data,io_type type,size_t max_size=std::numeric_limits<size_t>::max()){
        std::ifstream ifs;
        if(type==IO_BINARY)
                ifs.open(filename,std::ifstream::binary);
        if(type==IO_TEXT)
                ifs.open(filename,std::ifstream::in);
        size_t i=0;
        while(!ifs.eof() && --max_size>=0){
                ifs>>data[i];
        }
        ifs.close();
        return 0;
};
template<typename float_t>
int output(char* filename,float_t* data,io_type type,size_t max_col=1,size_t max_row=1){
        std::ofstream ofs;
        if(type==IO_BINARY){
                ofs.open(filename,std::ofstream::binary);
                for(size_t j=0;j<max_row;++j){
                        for(size_t i=0;i<max_col;++i)
                                ofs<<data[i+j*max_col]<<"\t";
                        ofs<<std::endl;
                }
        }
        if(type==IO_TEXT){
                ofs.open(filename,std::ofstream::out);
                for(size_t j=0;j<max_row;++j)
                        for(size_t i=0;i<max_col;++i)
                                ofs<<data[i+j*max_col];
        }
        ofs.close();
        return 0;
};
}

#endif
