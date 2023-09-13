#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphClone)(
cudaGraph_t *, 
cudaGraph_t
);
extern "C"
{
	cudaError_t cudaGraphClone(cudaGraph_t *pGraphClone, cudaGraph_t originalGraph) {
		fprintf(stderr, "===============\ncudaGraphClone()\n");
		char* __dlerror;
		if (!original_cudaGraphClone)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphClone = (cudaError_t (*)(
			cudaGraph_t *, 
			cudaGraph_t)
			) dlsym(RTLD_NEXT, "cudaGraphClone");
			fprintf(stderr, "original_cudaGraphClone:%p\n", original_cudaGraphClone);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphClone():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphClone(
		pGraphClone, 
		originalGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}