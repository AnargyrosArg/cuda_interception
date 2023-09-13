#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphDestroy)(
cudaGraph_t
);
extern "C"
{
	cudaError_t cudaGraphDestroy(cudaGraph_t graph) {
		fprintf(stderr, "===============\ncudaGraphDestroy()\n");
		char* __dlerror;
		if (!original_cudaGraphDestroy)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphDestroy = (cudaError_t (*)(
			cudaGraph_t)
			) dlsym(RTLD_NEXT, "cudaGraphDestroy");
			fprintf(stderr, "original_cudaGraphDestroy:%p\n", original_cudaGraphDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphDestroy(
		graph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}