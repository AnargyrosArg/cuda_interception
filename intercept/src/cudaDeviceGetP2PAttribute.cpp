#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetP2PAttribute)(
int *, 
enum cudaDeviceP2PAttr, 
int, 
int
);
extern "C"
{
	cudaError_t cudaDeviceGetP2PAttribute(int *value, enum cudaDeviceP2PAttr attr, int srcDevice, int dstDevice) {
		fprintf(stderr, "===============\ncudaDeviceGetP2PAttribute()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetP2PAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetP2PAttribute = (cudaError_t (*)(
			int *, 
			enum cudaDeviceP2PAttr, 
			int, 
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceGetP2PAttribute");
			fprintf(stderr, "original_cudaDeviceGetP2PAttribute:%p\n", original_cudaDeviceGetP2PAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetP2PAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetP2PAttribute(
		value, 
		attr, 
		srcDevice, 
		dstDevice
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}