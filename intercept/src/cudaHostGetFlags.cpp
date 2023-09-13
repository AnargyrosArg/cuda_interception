#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaHostGetFlags)(
unsigned int *, 
void *
);
extern "C"
{
	cudaError_t cudaHostGetFlags(unsigned int *pFlags, void *pHost) {
		fprintf(stderr, "===============\ncudaHostGetFlags()\n");
		char* __dlerror;
		if (!original_cudaHostGetFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaHostGetFlags = (cudaError_t (*)(
			unsigned int *, 
			void *)
			) dlsym(RTLD_NEXT, "cudaHostGetFlags");
			fprintf(stderr, "original_cudaHostGetFlags:%p\n", original_cudaHostGetFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaHostGetFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaHostGetFlags(
		pFlags, 
		pHost
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}