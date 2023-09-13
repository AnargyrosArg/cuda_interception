#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetNvSciSyncAttributes)(
void *, 
int, 
int
);
extern "C"
{
	cudaError_t cudaDeviceGetNvSciSyncAttributes(void *nvSciSyncAttrList, int device, int flags) {
		fprintf(stderr, "===============\ncudaDeviceGetNvSciSyncAttributes()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetNvSciSyncAttributes)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetNvSciSyncAttributes = (cudaError_t (*)(
			void *, 
			int, 
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceGetNvSciSyncAttributes");
			fprintf(stderr, "original_cudaDeviceGetNvSciSyncAttributes:%p\n", original_cudaDeviceGetNvSciSyncAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetNvSciSyncAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetNvSciSyncAttributes(
		nvSciSyncAttrList, 
		device, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}