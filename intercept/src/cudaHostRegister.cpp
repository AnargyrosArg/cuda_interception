#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaHostRegister)(
void *, 
size_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaHostRegister(void *ptr, size_t size, unsigned int flags) {
		fprintf(stderr, "===============\ncudaHostRegister()\n");
		char* __dlerror;
		if (!original_cudaHostRegister)
		{
			//fetch the original function addr using dlsym
			original_cudaHostRegister = (cudaError_t (*)(
			void *, 
			size_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaHostRegister");
			fprintf(stderr, "original_cudaHostRegister:%p\n", original_cudaHostRegister);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaHostRegister():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaHostRegister(
		ptr, 
		size, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}