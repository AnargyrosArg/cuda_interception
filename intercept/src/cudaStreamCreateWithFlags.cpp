#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamCreateWithFlags)(
cudaStream_t *, 
unsigned int
);
extern "C"
{
	cudaError_t cudaStreamCreateWithFlags(cudaStream_t *pStream, unsigned int flags) {
		fprintf(stderr, "===============\ncudaStreamCreateWithFlags()\n");
		char* __dlerror;
		if (!original_cudaStreamCreateWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamCreateWithFlags = (cudaError_t (*)(
			cudaStream_t *, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaStreamCreateWithFlags");
			fprintf(stderr, "original_cudaStreamCreateWithFlags:%p\n", original_cudaStreamCreateWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamCreateWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamCreateWithFlags(
		pStream, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}