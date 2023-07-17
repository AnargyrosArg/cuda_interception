all: example supportedVgpus

example:
	/usr/local/cuda-11.7/bin/nvcc example.c -lnvidia-ml -o example
supportedVgpus: 
	/usr/local/cuda-11.7/bin/nvcc supportedVgpus.c -lnvidia-ml -o supportedVgpus
clean:
	-@rm -f example.o
	-@rm -f example
	-@rm -f supportedVgpus.o
	-@rm -f supportedVgpus
