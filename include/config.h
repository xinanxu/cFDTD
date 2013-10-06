#ifndef _CONFIG_H
#define _CONFIG_H
#define LEVEL1_DCACHE_SIZE 32768
#define LEVEL2_CACHE_SIZE 262144
#define LEVEL3_CACHE_SIZE 6291456
#define _NPROCESSORS_ONLN 4
#define MEM_ALIGNMENT 16
namespace cfdtd{
	enum PARALLEL_TYPE{SERIAL=0,OMP,OCL};
}
#endif
#ifndef NDEBUG
#include <cstdio>
#define dprintf(...) fprintf(stderr,__VA_ARGS__)
#else
#define dprintf(...)
#endif
