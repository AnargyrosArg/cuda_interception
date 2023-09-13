#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemAdvise)(
const void *, 
size_t, 
enum cudaMemoryAdvise, 
int
);
extern "C"
{
	cudaError_t cudaMemAdvise(const void *devPtr, size_t count, enum cudaMemoryAdvise advice, int device) {
		fprintf(stderr, "===============\ncudaMemAdvise()\n");
		char* __dlerror;
		if (!original_cudaMemAdvise)
		{
			//fetch the original function addr using dlsym
			original_cudaMemAdvise = (cudaError_t (*)(
			const void *, 
			size_t, 
			enum cudaMemoryAdvise, 
			int)
			) dlsym(RTLD_NEXT, "cudaMemAdvise");
			fprintf(stderr, "original_cudaMemAdvise:%p\n", original_cudaMemAdvise);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemAdvise():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemAdvise(
		devPtr, 
		count, 
		advice, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}