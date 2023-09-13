#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemset)(
void *, 
int, 
size_t
);
extern "C"
{
	cudaError_t cudaMemset(void *devPtr, int value, size_t count) {
		fprintf(stderr, "===============\ncudaMemset()\n");
		char* __dlerror;
		if (!original_cudaMemset)
		{
			//fetch the original function addr using dlsym
			original_cudaMemset = (cudaError_t (*)(
			void *, 
			int, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMemset");
			fprintf(stderr, "original_cudaMemset:%p\n", original_cudaMemset);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemset():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemset(
		devPtr, 
		value, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}