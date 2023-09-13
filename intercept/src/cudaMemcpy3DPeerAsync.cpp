#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy3DPeerAsync)(
const struct cudaMemcpy3DPeerParms *, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpy3DPeerAsync(const struct cudaMemcpy3DPeerParms *p, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpy3DPeerAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpy3DPeerAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy3DPeerAsync = (cudaError_t (*)(
			const struct cudaMemcpy3DPeerParms *, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpy3DPeerAsync");
			fprintf(stderr, "original_cudaMemcpy3DPeerAsync:%p\n", original_cudaMemcpy3DPeerAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy3DPeerAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy3DPeerAsync(
		p, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}