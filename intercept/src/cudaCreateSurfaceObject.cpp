#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaCreateSurfaceObject)(
cudaSurfaceObject_t *, 
const struct cudaResourceDesc *
);
extern "C"
{
	cudaError_t cudaCreateSurfaceObject(cudaSurfaceObject_t *pSurfObject, const struct cudaResourceDesc *pResDesc) {
		fprintf(stderr, "===============\ncudaCreateSurfaceObject()\n");
		char* __dlerror;
		if (!original_cudaCreateSurfaceObject)
		{
			//fetch the original function addr using dlsym
			original_cudaCreateSurfaceObject = (cudaError_t (*)(
			cudaSurfaceObject_t *, 
			const struct cudaResourceDesc *)
			) dlsym(RTLD_NEXT, "cudaCreateSurfaceObject");
			fprintf(stderr, "original_cudaCreateSurfaceObject:%p\n", original_cudaCreateSurfaceObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaCreateSurfaceObject():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaCreateSurfaceObject(
		pSurfObject, 
		pResDesc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}