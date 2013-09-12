DIRS    := src
SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJS    := $(patsubst %.cpp, %.o, $(SOURCES))
OBJS    := $(foreach o,$(OBJS),./obj/$(o))
DEPFILES:= $(patsubst %.o, %.P, $(OBJS))
 
TESTDIRS    := test
TESTSOURCES := $(foreach dir, $(TESTDIRS), $(wildcard $(dir)/*.cpp))
TESTOBJS    := $(patsubst %.cpp, %.o, $(TESTSOURCES))
TESTOBJS    := $(foreach o,$(TESTOBJS),./obj/$(o))
TESTEXES    := $(patsubst %.o, %.exe, $(TESTOBJS))
TESTDEPFILES:= $(patsubst %.o, %.P, $(TESTOBJS))

CFLAGS   = -g -Wall -c -m64 -march=native -mtune=native -msse4.2 -O3 -fpic -I$(CURDIR)/include -std=c++0x
LDFLAGS  = -Wall -L$(CURDIR) -Wl,-rpath=$(CURDIR)
LIBS 	 = -lcfdtd
COMPILER = g++
 
TARGET: libcfdtd.so

libcfdtd.so: $(OBJS)
	$(COMPILER) -shared $(OBJS) -o $@

test: $(TARGET)$(TESTEXES)

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
