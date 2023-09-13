#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFuncSetSharedMemConfig)(
const void *, 
enum cudaSharedMemConfig
);
extern "C"
{
	cudaError_t cudaFuncSetSharedMemConfig(const void *func, enum cudaSharedMemConfig config) {
		fprintf(stderr, "===============\ncudaFuncSetSharedMemConfig()\n");
		char* __dlerror;
		if (!original_cudaFuncSetSharedMemConfig)
		{
			//fetch the original function addr using dlsym
			original_cudaFuncSetSharedMemConfig = (cudaError_t (*)(
			const void *, 
			enum cudaSharedMemConfig)
			) dlsym(RTLD_NEXT, "cudaFuncSetSharedMemConfig");
			fprintf(stderr, "original_cudaFuncSetSharedMemConfig:%p\n", original_cudaFuncSetSharedMemConfig);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFuncSetSharedMemConfig():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFuncSetSharedMemConfig(
		func, 
		config
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}