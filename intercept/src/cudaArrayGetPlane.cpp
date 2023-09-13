#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaArrayGetPlane)(
cudaArray_t *, 
cudaArray_t, 
unsigned int
);
extern "C"
{
	cudaError_t cudaArrayGetPlane(cudaArray_t *pPlaneArray, cudaArray_t hArray, unsigned int planeIdx) {
		fprintf(stderr, "===============\ncudaArrayGetPlane()\n");
		char* __dlerror;
		if (!original_cudaArrayGetPlane)
		{
			//fetch the original function addr using dlsym
			original_cudaArrayGetPlane = (cudaError_t (*)(
			cudaArray_t *, 
			cudaArray_t, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaArrayGetPlane");
			fprintf(stderr, "original_cudaArrayGetPlane:%p\n", original_cudaArrayGetPlane);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaArrayGetPlane():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaArrayGetPlane(
		pPlaneArray, 
		hArray, 
		planeIdx
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}