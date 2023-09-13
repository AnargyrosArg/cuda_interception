#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphInstantiateWithFlags)(
cudaGraphExec_t *, 
cudaGraph_t, 
unsigned long long
);
extern "C"
{
	cudaError_t cudaGraphInstantiateWithFlags(cudaGraphExec_t *pGraphExec, cudaGraph_t graph, unsigned long long flags) {
		fprintf(stderr, "===============\ncudaGraphInstantiateWithFlags()\n");
		char* __dlerror;
		if (!original_cudaGraphInstantiateWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphInstantiateWithFlags = (cudaError_t (*)(
			cudaGraphExec_t *, 
			cudaGraph_t, 
			unsigned long long)
			) dlsym(RTLD_NEXT, "cudaGraphInstantiateWithFlags");
			fprintf(stderr, "original_cudaGraphInstantiateWithFlags:%p\n", original_cudaGraphInstantiateWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphInstantiateWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphInstantiateWithFlags(
		pGraphExec, 
		graph, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}