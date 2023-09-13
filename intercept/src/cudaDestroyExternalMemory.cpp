#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDestroyExternalMemory)(
cudaExternalMemory_t
);
extern "C"
{
	cudaError_t cudaDestroyExternalMemory(cudaExternalMemory_t extMem) {
		fprintf(stderr, "===============\ncudaDestroyExternalMemory()\n");
		char* __dlerror;
		if (!original_cudaDestroyExternalMemory)
		{
			//fetch the original function addr using dlsym
			original_cudaDestroyExternalMemory = (cudaError_t (*)(
			cudaExternalMemory_t)
			) dlsym(RTLD_NEXT, "cudaDestroyExternalMemory");
			fprintf(stderr, "original_cudaDestroyExternalMemory:%p\n", original_cudaDestroyExternalMemory);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDestroyExternalMemory():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDestroyExternalMemory(
		extMem
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}