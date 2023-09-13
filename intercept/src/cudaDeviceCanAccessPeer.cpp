#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceCanAccessPeer)(
int *, 
int, 
int
);
extern "C"
{
	cudaError_t cudaDeviceCanAccessPeer(int *canAccessPeer, int device, int peerDevice) {
		fprintf(stderr, "===============\ncudaDeviceCanAccessPeer()\n");
		char* __dlerror;
		if (!original_cudaDeviceCanAccessPeer)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceCanAccessPeer = (cudaError_t (*)(
			int *, 
			int, 
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceCanAccessPeer");
			fprintf(stderr, "original_cudaDeviceCanAccessPeer:%p\n", original_cudaDeviceCanAccessPeer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceCanAccessPeer():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceCanAccessPeer(
		canAccessPeer, 
		device, 
		peerDevice
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}