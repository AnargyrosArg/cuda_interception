#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMallocManaged)(
void **, 
size_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaMallocManaged(void **devPtr, size_t size, unsigned int flags) {
		fprintf(stderr, "===============\ncudaMallocManaged()\n");
		char* __dlerror;
		if (!original_cudaMallocManaged)
		{
			//fetch the original function addr using dlsym
			original_cudaMallocManaged = (cudaError_t (*)(
			void **, 
			size_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaMallocManaged");
			fprintf(stderr, "original_cudaMallocManaged:%p\n", original_cudaMallocManaged);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMallocManaged():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMallocManaged(
		devPtr, 
		size, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}