#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphCreate)(
cudaGraph_t *, 
unsigned int
);
extern "C"
{
	cudaError_t cudaGraphCreate(cudaGraph_t *pGraph, unsigned int flags) {
		fprintf(stderr, "===============\ncudaGraphCreate()\n");
		char* __dlerror;
		if (!original_cudaGraphCreate)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphCreate = (cudaError_t (*)(
			cudaGraph_t *, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaGraphCreate");
			fprintf(stderr, "original_cudaGraphCreate:%p\n", original_cudaGraphCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphCreate(
		pGraph, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}