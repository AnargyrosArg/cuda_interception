#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaThreadSynchronize)(
);
extern "C"
{
	cudaError_t cudaThreadSynchronize() {
		fprintf(stderr, "===============\ncudaThreadSynchronize()\n");
		char* __dlerror;
		if (!original_cudaThreadSynchronize)
		{
			//fetch the original function addr using dlsym
			original_cudaThreadSynchronize = (cudaError_t (*)(
			void)
			) dlsym(RTLD_NEXT, "cudaThreadSynchronize");
			fprintf(stderr, "original_cudaThreadSynchronize:%p\n", original_cudaThreadSynchronize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaThreadSynchronize():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaThreadSynchronize(
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}