#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceFlushGPUDirectRDMAWrites)(
enum cudaFlushGPUDirectRDMAWritesTarget, 
enum cudaFlushGPUDirectRDMAWritesScope
);
extern "C"
{
	cudaError_t cudaDeviceFlushGPUDirectRDMAWrites(enum cudaFlushGPUDirectRDMAWritesTarget target, enum cudaFlushGPUDirectRDMAWritesScope scope) {
		fprintf(stderr, "===============\ncudaDeviceFlushGPUDirectRDMAWrites()\n");
		char* __dlerror;
		if (!original_cudaDeviceFlushGPUDirectRDMAWrites)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceFlushGPUDirectRDMAWrites = (cudaError_t (*)(
			enum cudaFlushGPUDirectRDMAWritesTarget, 
			enum cudaFlushGPUDirectRDMAWritesScope)
			) dlsym(RTLD_NEXT, "cudaDeviceFlushGPUDirectRDMAWrites");
			fprintf(stderr, "original_cudaDeviceFlushGPUDirectRDMAWrites:%p\n", original_cudaDeviceFlushGPUDirectRDMAWrites);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceFlushGPUDirectRDMAWrites():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceFlushGPUDirectRDMAWrites(
		target, 
		scope
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}