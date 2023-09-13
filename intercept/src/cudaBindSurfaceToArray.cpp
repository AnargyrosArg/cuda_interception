#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaBindSurfaceToArray)(
const struct surfaceReference *, 
cudaArray_const_t, 
const struct cudaChannelFormatDesc *
);
extern "C"
{
	cudaError_t cudaBindSurfaceToArray(const struct surfaceReference *surfref, cudaArray_const_t array, const struct cudaChannelFormatDesc *desc) {
		fprintf(stderr, "===============\ncudaBindSurfaceToArray()\n");
		char* __dlerror;
		if (!original_cudaBindSurfaceToArray)
		{
			//fetch the original function addr using dlsym
			original_cudaBindSurfaceToArray = (cudaError_t (*)(
			const struct surfaceReference *, 
			cudaArray_const_t, 
			const struct cudaChannelFormatDesc *)
			) dlsym(RTLD_NEXT, "cudaBindSurfaceToArray");
			fprintf(stderr, "original_cudaBindSurfaceToArray:%p\n", original_cudaBindSurfaceToArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaBindSurfaceToArray():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaBindSurfaceToArray(
		surfref, 
		array, 
		desc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}