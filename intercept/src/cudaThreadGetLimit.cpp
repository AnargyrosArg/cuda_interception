#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaThreadGetLimit)(
size_t *, 
enum cudaLimit
);
extern "C"
{
	cudaError_t cudaThreadGetLimit(size_t *pValue, enum cudaLimit limit) {
		fprintf(stderr, "===============\ncudaThreadGetLimit()\n");
		char* __dlerror;
		if (!original_cudaThreadGetLimit)
		{
			//fetch the original function addr using dlsym
			original_cudaThreadGetLimit = (cudaError_t (*)(
			size_t *, 
			enum cudaLimit)
			) dlsym(RTLD_NEXT, "cudaThreadGetLimit");
			fprintf(stderr, "original_cudaThreadGetLimit:%p\n", original_cudaThreadGetLimit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaThreadGetLimit():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaThreadGetLimit(
		pValue, 
		limit
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}