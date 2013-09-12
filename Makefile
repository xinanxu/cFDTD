DIRS    := src
SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJS    := $(patsubst %.cpp, %.o, $(SOURCES))
OBJS    := $(foreach o,$(OBJS),./obj/$(o))
DEPFILES:= $(patsubst %.o, %.P, $(OBJS))
 
TESTDIRS    := test
TESTSOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
TESTOBJS    := $(patsubst %.cpp, %.o, $(SOURCES))
TESTOBJS    := $(foreach o,$(OBJS),./obj/$(o))
TESTEXES    := $(TESTOBJS:.o=.exe)
TESTDEPFILES:= $(patsubst %.o, %.P, $(OBJS))

CFLAGS   = -g -Wall -Werror -c -m64 -march=native -mtune=native -msse4.2 -O3 -fpic -I$(CURDIR)/include
LDFLAGS  = -Wall -L$(CURDIR) -Wl,-rpath=$(CURDIR)
LIBS 	 = -lcfdtd
COMPILER = g++
 
TARGET: libcfdtd.so

libcfdtd.so: $(OBJS)
	$(COMPILER) -shared $(OBJS) -o $@

test: $(TESTEXES)
 
obj/%.o : %.cpp
	@mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) -o $@ -MMD -MF obj/$*.P $<
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < obj/$*.P >> obj/$*.P;
 
obj/%.exe : obj/%.o
	@mkdir -p $(@D)
	$(COMPILER) $(LDFLAGS)  -o $@ $< $(LIBS)
	./$@ 
	
clean:
	rm -rf obj

.PHONY: clean test all

-include $(DEPFILES)
