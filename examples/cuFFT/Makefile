CUDA_TOOLKIT := $(shell dirname $$(command -v nvcc))/..
INC          := -I$(CUDA_TOOLKIT)/include -I../../include -lcufft
FLAGS        := -O3 -std=c++11

all: 3d_c2c_example

3d_c2c_example: 3d_c2c_example.cpp
	nvcc -x cu $(FLAGS) $(INC) 3d_c2c_example.cpp -o a.out $(LIBS)

clean:
	rm -f a.out

.PHONY: clean all
