

out: matSumKernel.cu drivertest.cpp
	nvcc -ptx matSumKernel.cu
	nvcc drivertest.cpp -lcuda

clean:
	rm -rf a.out matSumKernel.ptx


run:
	LD_PRELOAD=../../intercept/libintercept.so ./a.out 


.PHONY: run
