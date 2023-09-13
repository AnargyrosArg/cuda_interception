#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecMemcpyNodeSetParams1D)(
cudaGraphExec_t, 
cudaGraphNode_t, 
void *, 
const void *, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaGraphExecMemcpyNodeSetParams1D(cudaGraphExec_t hGraphExec, cudaGraphNode_t node, void *dst, const void *src, size_t count, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaGraphExecMemcpyNodeSetParams1D()\n");
		char* __dlerror;
		if (!original_cudaGraphExecMemcpyNodeSetParams1D)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecMemcpyNodeSetParams1D = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			void *, 
			const void *, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaGraphExecMemcpyNodeSetParams1D");
			fprintf(stderr, "original_cudaGraphExecMemcpyNodeSetParams1D:%p\n", original_cudaGraphExecMemcpyNodeSetParams1D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecMemcpyNodeSetParams1D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecMemcpyNodeSetParams1D(
		hGraphExec, 
		node, 
		dst, 
		src, 
		count, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}