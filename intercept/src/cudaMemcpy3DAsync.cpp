#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy3DAsync)(
const struct cudaMemcpy3DParms *, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpy3DAsync(const struct cudaMemcpy3DParms *p, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpy3DAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpy3DAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy3DAsync = (cudaError_t (*)(
			const struct cudaMemcpy3DParms *, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpy3DAsync");
			fprintf(stderr, "original_cudaMemcpy3DAsync:%p\n", original_cudaMemcpy3DAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy3DAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy3DAsync(
		p, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}