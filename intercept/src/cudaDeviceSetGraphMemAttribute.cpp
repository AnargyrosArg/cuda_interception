#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceSetGraphMemAttribute)(
int, 
enum cudaGraphMemAttributeType, 
void *
);
extern "C"
{
	cudaError_t cudaDeviceSetGraphMemAttribute(int device, enum cudaGraphMemAttributeType attr, void *value) {
		fprintf(stderr, "===============\ncudaDeviceSetGraphMemAttribute()\n");
		char* __dlerror;
		if (!original_cudaDeviceSetGraphMemAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceSetGraphMemAttribute = (cudaError_t (*)(
			int, 
			enum cudaGraphMemAttributeType, 
			void *)
			) dlsym(RTLD_NEXT, "cudaDeviceSetGraphMemAttribute");
			fprintf(stderr, "original_cudaDeviceSetGraphMemAttribute:%p\n", original_cudaDeviceSetGraphMemAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceSetGraphMemAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceSetGraphMemAttribute(
		device, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}