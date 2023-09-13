#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceSetSharedMemConfig)(
enum cudaSharedMemConfig
);
extern "C"
{
	cudaError_t cudaDeviceSetSharedMemConfig(enum cudaSharedMemConfig config) {
		fprintf(stderr, "===============\ncudaDeviceSetSharedMemConfig()\n");
		char* __dlerror;
		if (!original_cudaDeviceSetSharedMemConfig)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceSetSharedMemConfig = (cudaError_t (*)(
			enum cudaSharedMemConfig)
			) dlsym(RTLD_NEXT, "cudaDeviceSetSharedMemConfig");
			fprintf(stderr, "original_cudaDeviceSetSharedMemConfig:%p\n", original_cudaDeviceSetSharedMemConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceSetSharedMemConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceSetSharedMemConfig(
		config
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}