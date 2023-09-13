#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaEventCreateWithFlags)(
cudaEvent_t *, 
unsigned int
);
extern "C"
{
	cudaError_t cudaEventCreateWithFlags(cudaEvent_t *event, unsigned int flags) {
		fprintf(stderr, "===============\ncudaEventCreateWithFlags()\n");
		char* __dlerror;
		if (!original_cudaEventCreateWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaEventCreateWithFlags = (cudaError_t (*)(
			cudaEvent_t *, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaEventCreateWithFlags");
			fprintf(stderr, "original_cudaEventCreateWithFlags:%p\n", original_cudaEventCreateWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaEventCreateWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaEventCreateWithFlags(
		event, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}