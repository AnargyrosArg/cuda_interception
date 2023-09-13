#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaHostAlloc)(
void **, 
size_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaHostAlloc(void **pHost, size_t size, unsigned int flags) {
		fprintf(stderr, "===============\ncudaHostAlloc()\n");
		char* __dlerror;
		if (!original_cudaHostAlloc)
		{
			//fetch the original function addr using dlsym
			original_cudaHostAlloc = (cudaError_t (*)(
			void **, 
			size_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaHostAlloc");
			fprintf(stderr, "original_cudaHostAlloc:%p\n", original_cudaHostAlloc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaHostAlloc():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaHostAlloc(
		pHost, 
		size, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}