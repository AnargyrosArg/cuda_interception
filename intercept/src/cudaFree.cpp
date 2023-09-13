#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFree)(
void *
);
extern "C"
{
	cudaError_t cudaFree(void *devPtr) {
		fprintf(stderr, "===============\ncudaFree()\n");
		char* __dlerror;
		if (!original_cudaFree)
		{
			//fetch the original function addr using dlsym
			original_cudaFree = (cudaError_t (*)(
			void *)
			) dlsym(RTLD_NEXT, "cudaFree");
			fprintf(stderr, "original_cudaFree:%p\n", original_cudaFree);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFree():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFree(
		devPtr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}