#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy3D)(
const struct cudaMemcpy3DParms *
);
extern "C"
{
	cudaError_t cudaMemcpy3D(const struct cudaMemcpy3DParms *p) {
		fprintf(stderr, "===============\ncudaMemcpy3D()\n");
		char* __dlerror;
		if (!original_cudaMemcpy3D)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy3D = (cudaError_t (*)(
			const struct cudaMemcpy3DParms *)
			) dlsym(RTLD_NEXT, "cudaMemcpy3D");
			fprintf(stderr, "original_cudaMemcpy3D:%p\n", original_cudaMemcpy3D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy3D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy3D(
		p
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}