#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDestroyExternalSemaphore)(
cudaExternalSemaphore_t
);
extern "C"
{
	cudaError_t cudaDestroyExternalSemaphore(cudaExternalSemaphore_t extSem) {
		fprintf(stderr, "===============\ncudaDestroyExternalSemaphore()\n");
		char* __dlerror;
		if (!original_cudaDestroyExternalSemaphore)
		{
			//fetch the original function addr using dlsym
			original_cudaDestroyExternalSemaphore = (cudaError_t (*)(
			cudaExternalSemaphore_t)
			) dlsym(RTLD_NEXT, "cudaDestroyExternalSemaphore");
			fprintf(stderr, "original_cudaDestroyExternalSemaphore:%p\n", original_cudaDestroyExternalSemaphore);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDestroyExternalSemaphore():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDestroyExternalSemaphore(
		extSem
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}