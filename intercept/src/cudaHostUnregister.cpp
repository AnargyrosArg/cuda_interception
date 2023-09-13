#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaHostUnregister)(
void *
);
extern "C"
{
	cudaError_t cudaHostUnregister(void *ptr) {
		fprintf(stderr, "===============\ncudaHostUnregister()\n");
		char* __dlerror;
		if (!original_cudaHostUnregister)
		{
			//fetch the original function addr using dlsym
			original_cudaHostUnregister = (cudaError_t (*)(
			void *)
			) dlsym(RTLD_NEXT, "cudaHostUnregister");
			fprintf(stderr, "original_cudaHostUnregister:%p\n", original_cudaHostUnregister);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaHostUnregister():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaHostUnregister(
		ptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}