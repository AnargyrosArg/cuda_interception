#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceReset)(
);
extern "C"
{
	cudaError_t cudaDeviceReset() {
		fprintf(stderr, "===============\ncudaDeviceReset()\n");
		char* __dlerror;
		if (!original_cudaDeviceReset)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceReset = (cudaError_t (*)(
			void)
			) dlsym(RTLD_NEXT, "cudaDeviceReset");
			fprintf(stderr, "original_cudaDeviceReset:%p\n", original_cudaDeviceReset);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceReset():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceReset(
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}