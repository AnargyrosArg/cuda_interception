#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamDestroy)(
cudaStream_t
);
extern "C"
{
	cudaError_t cudaStreamDestroy(cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaStreamDestroy()\n");
		char* __dlerror;
		if (!original_cudaStreamDestroy)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamDestroy = (cudaError_t (*)(
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaStreamDestroy");
			fprintf(stderr, "original_cudaStreamDestroy:%p\n", original_cudaStreamDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamDestroy(
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}