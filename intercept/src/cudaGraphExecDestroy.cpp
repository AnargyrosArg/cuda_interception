#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecDestroy)(
cudaGraphExec_t
);
extern "C"
{
	cudaError_t cudaGraphExecDestroy(cudaGraphExec_t graphExec) {
		fprintf(stderr, "===============\ncudaGraphExecDestroy()\n");
		char* __dlerror;
		if (!original_cudaGraphExecDestroy)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecDestroy = (cudaError_t (*)(
			cudaGraphExec_t)
			) dlsym(RTLD_NEXT, "cudaGraphExecDestroy");
			fprintf(stderr, "original_cudaGraphExecDestroy:%p\n", original_cudaGraphExecDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecDestroy(
		graphExec
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}