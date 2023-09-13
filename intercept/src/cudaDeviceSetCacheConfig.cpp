#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceSetCacheConfig)(
enum cudaFuncCache
);
extern "C"
{
	cudaError_t cudaDeviceSetCacheConfig(enum cudaFuncCache cacheConfig) {
		fprintf(stderr, "===============\ncudaDeviceSetCacheConfig()\n");
		char* __dlerror;
		if (!original_cudaDeviceSetCacheConfig)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceSetCacheConfig = (cudaError_t (*)(
			enum cudaFuncCache)
			) dlsym(RTLD_NEXT, "cudaDeviceSetCacheConfig");
			fprintf(stderr, "original_cudaDeviceSetCacheConfig:%p\n", original_cudaDeviceSetCacheConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceSetCacheConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceSetCacheConfig(
		cacheConfig
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}