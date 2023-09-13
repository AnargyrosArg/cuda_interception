#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceDisablePeerAccess)(
int
);
extern "C"
{
	cudaError_t cudaDeviceDisablePeerAccess(int peerDevice) {
		fprintf(stderr, "===============\ncudaDeviceDisablePeerAccess()\n");
		char* __dlerror;
		if (!original_cudaDeviceDisablePeerAccess)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceDisablePeerAccess = (cudaError_t (*)(
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceDisablePeerAccess");
			fprintf(stderr, "original_cudaDeviceDisablePeerAccess:%p\n", original_cudaDeviceDisablePeerAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceDisablePeerAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceDisablePeerAccess(
		peerDevice
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}