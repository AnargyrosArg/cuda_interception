.PHONY: run

out: cublas_amax_example.cu 
	nvcc cublas_amax_example.cu -lcublas -o a.out

clean:
	rm -rf a.out *.txt

run:
	LD_PRELOAD=../../intercept/libintercept.so ./a.out 2> log.txt
