DIRS    := src
SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJS    := $(patsubst %.cpp, %.o, $(SOURCES))
OBJS    := $(foreach o,$(OBJS),./obj/$(o))
 
TESTDIRS    := unittest
TESTSOURCES := $(foreach dir, $(TESTDIRS), $(wildcard $(dir)/*.cpp))
TESTOBJS    := $(patsubst %.cpp, %.o, $(TESTSOURCES))
TESTOBJS    := $(foreach o,$(TESTOBJS),./obj/$(o))
TESTEXES    := $(patsubst %.o, %.exe, $(TESTOBJS))

CFLAGS   = -g -Wall -c -O0 -fpic -I./include -std=c++11
LDFLAGS  = -Wall -L. -Wl,-rpath=$(CURDIR)
LIBS 	 = -lcfdtd
COMPILER = g++
 
TARGET: libcfdtd.so

libcfdtd.so: $(OBJS)
	@echo "[CXX] $@"
	@$(COMPILER) -shared $(OBJS) -o $@

test: libcfdtd.so $(TESTEXES) 

obj/%.o : %.cpp
	@echo "[CXX] $<"
	@mkdir -p $(@D)
	@$(COMPILER) $(CFLAGS) -o $@ $<
 
obj/%.exe : obj/%.o 
	@mkdir -p $(@D)
	@echo "[LD] $<"
	@$(COMPILER) $(LDFLAGS)  -o $@ $< $(LIBS)
	@echo "[TEST] $@"
	@./$@ 
	@echo "[PASS] $@"
	
clean:
	rm -rf obj libcfdtd.so

.PHONY: clean test

