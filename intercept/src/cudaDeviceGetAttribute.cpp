#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetAttribute)(
int *, 
enum cudaDeviceAttr, 
int
);
extern "C"
{
	cudaError_t cudaDeviceGetAttribute(int *value, enum cudaDeviceAttr attr, int device) {
		fprintf(stderr, "===============\ncudaDeviceGetAttribute()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetAttribute = (cudaError_t (*)(
			int *, 
			enum cudaDeviceAttr, 
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceGetAttribute");
			fprintf(stderr, "original_cudaDeviceGetAttribute:%p\n", original_cudaDeviceGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetAttribute(
		value, 
		attr, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}