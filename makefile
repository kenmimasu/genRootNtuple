# Compilation flags (see ./configure --help for further documentation).
# ENABLE_SHARED=true
CXX=g++
CXX_COMMON=-O2 -std=c++98 -pedantic -W -Wall -Wshadow -fPIC
CXX_SHARED=-shared
# CXX_SONAME=-Wl,-soname,
# LIB_SUFFIX=.so

BUILD_DIR=`pwd -P`
PYTHIA8_DIR=/home/users/k/m/kmimasu/apps/pythia/gcc-4.9.1/8.226
ROOT_DIR=/cvmfs/cp3.uclouvain.be/root/root-6.06.02-sl6_amd64_gcc49# EVTGEN configuration.

PYTHIA8_LIB=$(PYTHIA8_DIR)/lib
PYTHIA8_INC=$(PYTHIA8_DIR)/include

CXX_COMMON:=-I$(PYTHIA8_INC) $(CXX_COMMON)
CXX_COMMON+= -L$(PYTHIA8_LIB) -Wl,-rpath,$(PYTHIA8_LIB) -lpythia8 -ldl 

ROOT_BIN=$(ROOT_DIR)/bin/
ROOT_LIB=$(ROOT_DIR)/lib
ROOT_INC=$(ROOT_DIR)/include


.SECONDEXPANSION:
.PHONY: all clean

genRootNtuple: $$@.cc  genRootNtuple.so 
	$(CXX) $< genRootNtuple.so -o $@ -w -I$(ROOT_INC) $(CXX_COMMON)\
	`$(ROOT_BIN)root-config --cflags` -Wl,-rpath,$(BUILD_DIR) \
	-Wl,-rpath,$(ROOT_LIB) `$(ROOT_BIN)root-config --glibs` 


genRootNtuple.so: genRootNtupleDct.cc
	$(CXX) $< -o $@ -w -I$(ROOT_INC) $(CXX_SHARED) $(CXX_COMMON)\
	`$(ROOT_BIN)root-config --cflags` 

genRootNtupleDct.cc: genRootNtuple.h genRootNtuple_LinkDef.h
	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(ROOT_LIB);\
	$(ROOT_BIN)rootcint -f $@ -c -I$(PYTHIA8_INC) $^

all: genRootNtuple
	
clean:
	rm -f genRootNtuple; rm -f *Dct.*; rm -f *.so; rm -rf *.pcm;
