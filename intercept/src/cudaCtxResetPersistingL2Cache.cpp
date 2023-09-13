#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaCtxResetPersistingL2Cache)(
);
extern "C"
{
	cudaError_t cudaCtxResetPersistingL2Cache() {
		fprintf(stderr, "===============\ncudaCtxResetPersistingL2Cache()\n");
		char* __dlerror;
		if (!original_cudaCtxResetPersistingL2Cache)
		{
			//fetch the original function addr using dlsym
			original_cudaCtxResetPersistingL2Cache = (cudaError_t (*)(
			void)
			) dlsym(RTLD_NEXT, "cudaCtxResetPersistingL2Cache");
			fprintf(stderr, "original_cudaCtxResetPersistingL2Cache:%p\n", original_cudaCtxResetPersistingL2Cache);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaCtxResetPersistingL2Cache():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaCtxResetPersistingL2Cache(
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}