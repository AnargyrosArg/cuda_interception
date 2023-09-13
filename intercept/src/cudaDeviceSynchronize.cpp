#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceSynchronize)(
);
extern "C"
{
	cudaError_t cudaDeviceSynchronize() {
		fprintf(stderr, "===============\ncudaDeviceSynchronize()\n");
		char* __dlerror;
		if (!original_cudaDeviceSynchronize)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceSynchronize = (cudaError_t (*)(
			void)
			) dlsym(RTLD_NEXT, "cudaDeviceSynchronize");
			fprintf(stderr, "original_cudaDeviceSynchronize:%p\n", original_cudaDeviceSynchronize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceSynchronize():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceSynchronize(
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}