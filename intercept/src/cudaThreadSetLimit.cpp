#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaThreadSetLimit)(
enum cudaLimit, 
size_t
);
extern "C"
{
	cudaError_t cudaThreadSetLimit(enum cudaLimit limit, size_t value) {
		fprintf(stderr, "===============\ncudaThreadSetLimit()\n");
		char* __dlerror;
		if (!original_cudaThreadSetLimit)
		{
			//fetch the original function addr using dlsym
			original_cudaThreadSetLimit = (cudaError_t (*)(
			enum cudaLimit, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaThreadSetLimit");
			fprintf(stderr, "original_cudaThreadSetLimit:%p\n", original_cudaThreadSetLimit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaThreadSetLimit():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaThreadSetLimit(
		limit, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}