
out: vectoradd.cu
	nvcc -g -lcudart  vectoradd.cu
#-lcuda

clean:
	rm -rf ./a.out

run:
	LD_PRELOAD=../../intercept/libintercept.so ./a.out 2> log.txt


.PHONY: run