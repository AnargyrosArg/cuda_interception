#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaHostGetDevicePointer)(
void **, 
void *, 
unsigned int
);
extern "C"
{
	cudaError_t cudaHostGetDevicePointer(void **pDevice, void *pHost, unsigned int flags) {
		fprintf(stderr, "===============\ncudaHostGetDevicePointer()\n");
		char* __dlerror;
		if (!original_cudaHostGetDevicePointer)
		{
			//fetch the original function addr using dlsym
			original_cudaHostGetDevicePointer = (cudaError_t (*)(
			void **, 
			void *, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaHostGetDevicePointer");
			fprintf(stderr, "original_cudaHostGetDevicePointer:%p\n", original_cudaHostGetDevicePointer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaHostGetDevicePointer():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaHostGetDevicePointer(
		pDevice, 
		pHost, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}