
out: vectoradd.cu
	nvcc vectoradd.cu  -lnvidia-ml
#-lcudart 
#-lcuda

clean:
	rm -rf ./a.out

run:
	LD_PRELOAD=../../intercept/libintercept.so ./a.out 2> log.txt


.PHONY: run