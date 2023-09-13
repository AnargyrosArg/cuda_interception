#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetDeviceFlags)(
unsigned int *
);
extern "C"
{
	cudaError_t cudaGetDeviceFlags(unsigned int *flags) {
		fprintf(stderr, "===============\ncudaGetDeviceFlags()\n");
		char* __dlerror;
		if (!original_cudaGetDeviceFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaGetDeviceFlags = (cudaError_t (*)(
			unsigned int *)
			) dlsym(RTLD_NEXT, "cudaGetDeviceFlags");
			fprintf(stderr, "original_cudaGetDeviceFlags:%p\n", original_cudaGetDeviceFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetDeviceFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetDeviceFlags(
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}