#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemGetInfo)(
size_t *, 
size_t *
);
extern "C"
{
	cudaError_t cudaMemGetInfo(size_t *free, size_t *total) {
		fprintf(stderr, "===============\ncudaMemGetInfo()\n");
		char* __dlerror;
		if (!original_cudaMemGetInfo)
		{
			//fetch the original function addr using dlsym
			original_cudaMemGetInfo = (cudaError_t (*)(
			size_t *, 
			size_t *)
			) dlsym(RTLD_NEXT, "cudaMemGetInfo");
			fprintf(stderr, "original_cudaMemGetInfo:%p\n", original_cudaMemGetInfo);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemGetInfo():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemGetInfo(
		free, 
		total
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}