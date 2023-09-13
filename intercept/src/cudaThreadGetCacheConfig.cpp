#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaThreadGetCacheConfig)(
enum cudaFuncCache *
);
extern "C"
{
	cudaError_t cudaThreadGetCacheConfig(enum cudaFuncCache *pCacheConfig) {
		fprintf(stderr, "===============\ncudaThreadGetCacheConfig()\n");
		char* __dlerror;
		if (!original_cudaThreadGetCacheConfig)
		{
			//fetch the original function addr using dlsym
			original_cudaThreadGetCacheConfig = (cudaError_t (*)(
			enum cudaFuncCache *)
			) dlsym(RTLD_NEXT, "cudaThreadGetCacheConfig");
			fprintf(stderr, "original_cudaThreadGetCacheConfig:%p\n", original_cudaThreadGetCacheConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaThreadGetCacheConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaThreadGetCacheConfig(
		pCacheConfig
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}