#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphUpload)(
cudaGraphExec_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaGraphUpload(cudaGraphExec_t graphExec, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaGraphUpload()\n");
		char* __dlerror;
		if (!original_cudaGraphUpload)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphUpload = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaGraphUpload");
			fprintf(stderr, "original_cudaGraphUpload:%p\n", original_cudaGraphUpload);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphUpload():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphUpload(
		graphExec, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}