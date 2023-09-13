#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetSurfaceObjectResourceDesc)(
struct cudaResourceDesc *, 
cudaSurfaceObject_t
);
extern "C"
{
	cudaError_t cudaGetSurfaceObjectResourceDesc(struct cudaResourceDesc *pResDesc, cudaSurfaceObject_t surfObject) {
		fprintf(stderr, "===============\ncudaGetSurfaceObjectResourceDesc()\n");
		char* __dlerror;
		if (!original_cudaGetSurfaceObjectResourceDesc)
		{
			//fetch the original function addr using dlsym
			original_cudaGetSurfaceObjectResourceDesc = (cudaError_t (*)(
			struct cudaResourceDesc *, 
			cudaSurfaceObject_t)
			) dlsym(RTLD_NEXT, "cudaGetSurfaceObjectResourceDesc");
			fprintf(stderr, "original_cudaGetSurfaceObjectResourceDesc:%p\n", original_cudaGetSurfaceObjectResourceDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetSurfaceObjectResourceDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetSurfaceObjectResourceDesc(
		pResDesc, 
		surfObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}