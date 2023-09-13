#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaPointerGetAttributes)(
struct cudaPointerAttributes *, 
const void *
);
extern "C"
{
	cudaError_t cudaPointerGetAttributes(struct cudaPointerAttributes *attributes, const void *ptr) {
		fprintf(stderr, "===============\ncudaPointerGetAttributes()\n");
		char* __dlerror;
		if (!original_cudaPointerGetAttributes)
		{
			//fetch the original function addr using dlsym
			original_cudaPointerGetAttributes = (cudaError_t (*)(
			struct cudaPointerAttributes *, 
			const void *)
			) dlsym(RTLD_NEXT, "cudaPointerGetAttributes");
			fprintf(stderr, "original_cudaPointerGetAttributes:%p\n", original_cudaPointerGetAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaPointerGetAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaPointerGetAttributes(
		attributes, 
		ptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}