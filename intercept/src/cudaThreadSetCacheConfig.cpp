#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaThreadSetCacheConfig)(
enum cudaFuncCache
);
extern "C"
{
	cudaError_t cudaThreadSetCacheConfig(enum cudaFuncCache cacheConfig) {
		fprintf(stderr, "===============\ncudaThreadSetCacheConfig()\n");
		char* __dlerror;
		if (!original_cudaThreadSetCacheConfig)
		{
			//fetch the original function addr using dlsym
			original_cudaThreadSetCacheConfig = (cudaError_t (*)(
			enum cudaFuncCache)
			) dlsym(RTLD_NEXT, "cudaThreadSetCacheConfig");
			fprintf(stderr, "original_cudaThreadSetCacheConfig:%p\n", original_cudaThreadSetCacheConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaThreadSetCacheConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaThreadSetCacheConfig(
		cacheConfig
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}