#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetSharedMemConfig)(
enum cudaSharedMemConfig *
);
extern "C"
{
	cudaError_t cudaDeviceGetSharedMemConfig(enum cudaSharedMemConfig *pConfig) {
		fprintf(stderr, "===============\ncudaDeviceGetSharedMemConfig()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetSharedMemConfig)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetSharedMemConfig = (cudaError_t (*)(
			enum cudaSharedMemConfig *)
			) dlsym(RTLD_NEXT, "cudaDeviceGetSharedMemConfig");
			fprintf(stderr, "original_cudaDeviceGetSharedMemConfig:%p\n", original_cudaDeviceGetSharedMemConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetSharedMemConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetSharedMemConfig(
		pConfig
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}