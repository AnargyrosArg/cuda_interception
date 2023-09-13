#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDestroySurfaceObject)(
cudaSurfaceObject_t
);
extern "C"
{
	cudaError_t cudaDestroySurfaceObject(cudaSurfaceObject_t surfObject) {
		fprintf(stderr, "===============\ncudaDestroySurfaceObject()\n");
		char* __dlerror;
		if (!original_cudaDestroySurfaceObject)
		{
			//fetch the original function addr using dlsym
			original_cudaDestroySurfaceObject = (cudaError_t (*)(
			cudaSurfaceObject_t)
			) dlsym(RTLD_NEXT, "cudaDestroySurfaceObject");
			fprintf(stderr, "original_cudaDestroySurfaceObject:%p\n", original_cudaDestroySurfaceObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDestroySurfaceObject():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDestroySurfaceObject(
		surfObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}