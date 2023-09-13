#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMallocHost)(
void **, 
size_t
);
extern "C"
{
	cudaError_t cudaMallocHost(void **ptr, size_t size) {
		fprintf(stderr, "===============\ncudaMallocHost()\n");
		char* __dlerror;
		if (!original_cudaMallocHost)
		{
			//fetch the original function addr using dlsym
			original_cudaMallocHost = (cudaError_t (*)(
			void **, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMallocHost");
			fprintf(stderr, "original_cudaMallocHost:%p\n", original_cudaMallocHost);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMallocHost():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMallocHost(
		ptr, 
		size
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}