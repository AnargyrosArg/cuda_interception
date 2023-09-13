#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceEnablePeerAccess)(
int, 
unsigned int
);
extern "C"
{
	cudaError_t cudaDeviceEnablePeerAccess(int peerDevice, unsigned int flags) {
		fprintf(stderr, "===============\ncudaDeviceEnablePeerAccess()\n");
		char* __dlerror;
		if (!original_cudaDeviceEnablePeerAccess)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceEnablePeerAccess = (cudaError_t (*)(
			int, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaDeviceEnablePeerAccess");
			fprintf(stderr, "original_cudaDeviceEnablePeerAccess:%p\n", original_cudaDeviceEnablePeerAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceEnablePeerAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceEnablePeerAccess(
		peerDevice, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}