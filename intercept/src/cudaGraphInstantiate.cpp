#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphInstantiate)(
cudaGraphExec_t *, 
cudaGraph_t, 
cudaGraphNode_t *, 
char *, 
size_t
);
extern "C"
{
	cudaError_t cudaGraphInstantiate(cudaGraphExec_t *pGraphExec, cudaGraph_t graph, cudaGraphNode_t *pErrorNode, char *pLogBuffer, size_t bufferSize) {
		fprintf(stderr, "===============\ncudaGraphInstantiate()\n");
		char* __dlerror;
		if (!original_cudaGraphInstantiate)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphInstantiate = (cudaError_t (*)(
			cudaGraphExec_t *, 
			cudaGraph_t, 
			cudaGraphNode_t *, 
			char *, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaGraphInstantiate");
			fprintf(stderr, "original_cudaGraphInstantiate:%p\n", original_cudaGraphInstantiate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphInstantiate():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphInstantiate(
		pGraphExec, 
		graph, 
		pErrorNode, 
		pLogBuffer, 
		bufferSize
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}