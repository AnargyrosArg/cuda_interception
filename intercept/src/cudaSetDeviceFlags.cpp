#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaSetDeviceFlags)(
unsigned int
);
extern "C"
{
	cudaError_t cudaSetDeviceFlags(unsigned int flags) {
		fprintf(stderr, "===============\ncudaSetDeviceFlags()\n");
		char* __dlerror;
		if (!original_cudaSetDeviceFlags)
		{
			//fetch the original function addr using dlsym
			original_cudaSetDeviceFlags = (cudaError_t (*)(
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaSetDeviceFlags");
			fprintf(stderr, "original_cudaSetDeviceFlags:%p\n", original_cudaSetDeviceFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaSetDeviceFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaSetDeviceFlags(
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}