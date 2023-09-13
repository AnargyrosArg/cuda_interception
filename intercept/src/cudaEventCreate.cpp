#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaEventCreate)(
cudaEvent_t *
);
extern "C"
{
	cudaError_t cudaEventCreate(cudaEvent_t *event) {
		fprintf(stderr, "===============\ncudaEventCreate()\n");
		char* __dlerror;
		if (!original_cudaEventCreate)
		{
			//fetch the original function addr using dlsym
			original_cudaEventCreate = (cudaError_t (*)(
			cudaEvent_t *)
			) dlsym(RTLD_NEXT, "cudaEventCreate");
			fprintf(stderr, "original_cudaEventCreate:%p\n", original_cudaEventCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaEventCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaEventCreate(
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}