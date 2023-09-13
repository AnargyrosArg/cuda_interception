#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetCacheConfig)(
enum cudaFuncCache *
);
extern "C"
{
	cudaError_t cudaDeviceGetCacheConfig(enum cudaFuncCache *pCacheConfig) {
		fprintf(stderr, "===============\ncudaDeviceGetCacheConfig()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetCacheConfig)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetCacheConfig = (cudaError_t (*)(
			enum cudaFuncCache *)
			) dlsym(RTLD_NEXT, "cudaDeviceGetCacheConfig");
			fprintf(stderr, "original_cudaDeviceGetCacheConfig:%p\n", original_cudaDeviceGetCacheConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetCacheConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetCacheConfig(
		pCacheConfig
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}