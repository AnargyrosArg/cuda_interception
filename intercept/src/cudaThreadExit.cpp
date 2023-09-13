#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaThreadExit)(
);
extern "C"
{
	cudaError_t cudaThreadExit() {
		fprintf(stderr, "===============\ncudaThreadExit()\n");
		char* __dlerror;
		if (!original_cudaThreadExit)
		{
			//fetch the original function addr using dlsym
			original_cudaThreadExit = (cudaError_t (*)(
			void)
			) dlsym(RTLD_NEXT, "cudaThreadExit");
			fprintf(stderr, "original_cudaThreadExit:%p\n", original_cudaThreadExit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaThreadExit():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaThreadExit(
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}