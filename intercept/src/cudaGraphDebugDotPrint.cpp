#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphDebugDotPrint)(
cudaGraph_t, 
const char *, 
unsigned int
);
extern "C"
{
	cudaError_t cudaGraphDebugDotPrint(cudaGraph_t graph, const char *path, unsigned int flags) {
		fprintf(stderr, "===============\ncudaGraphDebugDotPrint()\n");
		char* __dlerror;
		if (!original_cudaGraphDebugDotPrint)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphDebugDotPrint = (cudaError_t (*)(
			cudaGraph_t, 
			const char *, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaGraphDebugDotPrint");
			fprintf(stderr, "original_cudaGraphDebugDotPrint:%p\n", original_cudaGraphDebugDotPrint);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphDebugDotPrint():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphDebugDotPrint(
		graph, 
		path, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}