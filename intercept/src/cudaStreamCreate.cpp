#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamCreate)(
cudaStream_t *
);
extern "C"
{
	cudaError_t cudaStreamCreate(cudaStream_t *pStream) {
		fprintf(stderr, "===============\ncudaStreamCreate()\n");
		char* __dlerror;
		if (!original_cudaStreamCreate)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamCreate = (cudaError_t (*)(
			cudaStream_t *)
			) dlsym(RTLD_NEXT, "cudaStreamCreate");
			fprintf(stderr, "original_cudaStreamCreate:%p\n", original_cudaStreamCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamCreate(
		pStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}