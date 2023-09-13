#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetGraphMemAttribute)(
int, 
enum cudaGraphMemAttributeType, 
void *
);
extern "C"
{
	cudaError_t cudaDeviceGetGraphMemAttribute(int device, enum cudaGraphMemAttributeType attr, void *value) {
		fprintf(stderr, "===============\ncudaDeviceGetGraphMemAttribute()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetGraphMemAttribute)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetGraphMemAttribute = (cudaError_t (*)(
			int, 
			enum cudaGraphMemAttributeType, 
			void *)
			) dlsym(RTLD_NEXT, "cudaDeviceGetGraphMemAttribute");
			fprintf(stderr, "original_cudaDeviceGetGraphMemAttribute:%p\n", original_cudaDeviceGetGraphMemAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetGraphMemAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetGraphMemAttribute(
		device, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}