DIRS    := src
SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJS    := $(patsubst %.cpp, %.o, $(SOURCES))
OBJS    := $(foreach o,$(OBJS),./obj/$(o))
DEPFILES:= $(patsubst %.o, %.P, $(OBJS))
 
CFLAGS   = -Wall -MMD -c
COMPILER = g++
 
#link the executable
main: $(OBJS)
	$(COMPILER) $(OBJS) -o main
 
#generate dependency information and compile
obj/%.o : %.cpp
	@mkdir -p $(@D)
	$(COMPILER) $(CFLAGS) -o $@ -MF obj/$*.P $<
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < obj/$*.P >> obj/$*.P;
 
#remove all generated files
clean:
	rm -f main
	rm -rf obj
 
#include the dependency information
-include $(DEPFILES)
