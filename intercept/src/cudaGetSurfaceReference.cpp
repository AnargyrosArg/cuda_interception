#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetSurfaceReference)(
const struct surfaceReference **, 
const void *
);
extern "C"
{
	cudaError_t cudaGetSurfaceReference(const struct surfaceReference **surfref, const void *symbol) {
		fprintf(stderr, "===============\ncudaGetSurfaceReference()\n");
		char* __dlerror;
		if (!original_cudaGetSurfaceReference)
		{
			//fetch the original function addr using dlsym
			original_cudaGetSurfaceReference = (cudaError_t (*)(
			const struct surfaceReference **, 
			const void *)
			) dlsym(RTLD_NEXT, "cudaGetSurfaceReference");
			fprintf(stderr, "original_cudaGetSurfaceReference:%p\n", original_cudaGetSurfaceReference);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetSurfaceReference():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetSurfaceReference(
		surfref, 
		symbol
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}