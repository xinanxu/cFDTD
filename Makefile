CXX		:= g++
CFLAGS	:= -g -Wall -Werror -c -O0 -fopenmp -I./include -std=c++11
LDFLAGS	:= -Wall -static -L$(CURDIR)
LIBS	:= -lcfdtd

DIRS    := src
TARGET	:= libcfdtd.a
SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJECTS := $(SOURCES:.cpp=.o)

TESTDIRS    := unittest
TESTSRCS 	:= $(foreach dir, $(TESTDIRS), $(wildcard $(dir)/*.cpp))
TESTOBJS	:= $(TESTSRCS:.cpp=.o)
TESTEXES    := $(TESTOBJS:.o=.exe)
TESTTHEM	:= $(TESTEXES:.exe=.exe-test)
 
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "[AR] $@"
	@ar rcs $@ $<

%.o : %.cpp
	@echo "[CXX] $<"
	@mkdir -p $(@D)
	@$(CXX) $(CFLAGS) -o $@ $<

test: $(TARGET) $(TESTEXES) $(TESTTHEM)

%.exe : %.o 
	@echo "[LD] $<"
	@$(CXX) $(LDFLAGS) -o $@ $< $(LIBS)

$(TESTTHEM):
	@echo "[TEST]$(notdir $(@:.exe-test=.exe))"
	@$(@:.exe-test=.exe)
	@echo "[PASS]$(notdir $(@:.exe-test=.exe))"

clean:
	rm -rf $(OBJECTS) $(TESTOBJS) $(TESTEXES) $(TARGET)
	
.PHONY: all clean test
