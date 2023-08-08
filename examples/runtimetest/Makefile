
out: vectoradd.cu
	nvcc -g vectoradd.cu
#-lcudart 
#-lcuda

clean:
	rm -rf ./a.out

run:
	LD_PRELOAD=../../intercept/libintercept.so ./a.out 2> log.txt


.PHONY: run