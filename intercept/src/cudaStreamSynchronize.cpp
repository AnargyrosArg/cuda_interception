#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamSynchronize)(
cudaStream_t
);
extern "C"
{
	cudaError_t cudaStreamSynchronize(cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaStreamSynchronize()\n");
		char* __dlerror;
		if (!original_cudaStreamSynchronize)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamSynchronize = (cudaError_t (*)(
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaStreamSynchronize");
			fprintf(stderr, "original_cudaStreamSynchronize:%p\n", original_cudaStreamSynchronize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamSynchronize():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamSynchronize(
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}