#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMalloc)(
void **, 
size_t
);
extern "C"
{
	cudaError_t cudaMalloc(void **devPtr, size_t size) {
		fprintf(stderr, "===============\ncudaMalloc()\n");
		char* __dlerror;
		if (!original_cudaMalloc)
		{
			//fetch the original function addr using dlsym
			original_cudaMalloc = (cudaError_t (*)(
			void **, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMalloc");
			fprintf(stderr, "original_cudaMalloc:%p\n", original_cudaMalloc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMalloc():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMalloc(
		devPtr, 
		size
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}