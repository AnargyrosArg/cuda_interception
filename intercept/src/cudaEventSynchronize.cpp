#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaEventSynchronize)(
cudaEvent_t
);
extern "C"
{
	cudaError_t cudaEventSynchronize(cudaEvent_t event) {
		fprintf(stderr, "===============\ncudaEventSynchronize()\n");
		char* __dlerror;
		if (!original_cudaEventSynchronize)
		{
			//fetch the original function addr using dlsym
			original_cudaEventSynchronize = (cudaError_t (*)(
			cudaEvent_t)
			) dlsym(RTLD_NEXT, "cudaEventSynchronize");
			fprintf(stderr, "original_cudaEventSynchronize:%p\n", original_cudaEventSynchronize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaEventSynchronize():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaEventSynchronize(
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}