#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaEventDestroy)(
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaEventDestroy(cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaEventDestroy()\n");
		char* __dlerror;
		if (!original_cudaEventDestroy)
		{
			//fetch the original function addr using dlsym
			original_cudaEventDestroy = (cudaError_t (*)(
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaEventDestroy");
			fprintf(stderr, "original_cudaEventDestroy:%p\n", original_cudaEventDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaEventDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaEventDestroy(
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}