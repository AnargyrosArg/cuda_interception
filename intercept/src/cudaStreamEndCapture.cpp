#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamEndCapture)(
cudaStream_t, 
cudaGraph_t *
);
extern "C"
{
	cudaError_t cudaStreamEndCapture(cudaStream_t stream, cudaGraph_t *pGraph) {
		fprintf(stderr, "===============\ncudaStreamEndCapture()\n");
		char* __dlerror;
		if (!original_cudaStreamEndCapture)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamEndCapture = (cudaError_t (*)(
			cudaStream_t, 
			cudaGraph_t *)
			) dlsym(RTLD_NEXT, "cudaStreamEndCapture");
			fprintf(stderr, "original_cudaStreamEndCapture:%p\n", original_cudaStreamEndCapture);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamEndCapture():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamEndCapture(
		stream, 
		pGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}