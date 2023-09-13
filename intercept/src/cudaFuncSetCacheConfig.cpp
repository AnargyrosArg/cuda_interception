#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFuncSetCacheConfig)(
const void *, 
enum cudaFuncCache
);
extern "C"
{
	cudaError_t cudaFuncSetCacheConfig(const void *func, enum cudaFuncCache cacheConfig) {
		fprintf(stderr, "===============\ncudaFuncSetCacheConfig()\n");
		char* __dlerror;
		if (!original_cudaFuncSetCacheConfig)
		{
			//fetch the original function addr using dlsym
			original_cudaFuncSetCacheConfig = (cudaError_t (*)(
			const void *, 
			enum cudaFuncCache)
			) dlsym(RTLD_NEXT, "cudaFuncSetCacheConfig");
			fprintf(stderr, "original_cudaFuncSetCacheConfig:%p\n", original_cudaFuncSetCacheConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFuncSetCacheConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFuncSetCacheConfig(
		func, 
		cacheConfig
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}