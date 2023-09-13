#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamGetFlags)(
cudaStream_t, 
unsigned int *
);
extern "C"
{
	cudaError_t cudaStreamGetFlags(cudaStream_t hStream, unsigned int *flags) {
		fprintf(stderr, "===============\ncudaStreamGetFlags()\n");
		char* __dlerror;
		if (!original_cudaStreamGetFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamGetFlags = (cudaError_t (*)(
			cudaStream_t, 
			unsigned int *)
			) dlsym(RTLD_NEXT, "cudaStreamGetFlags");
			fprintf(stderr, "original_cudaStreamGetFlags:%p\n", original_cudaStreamGetFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamGetFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamGetFlags(
		hStream, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}