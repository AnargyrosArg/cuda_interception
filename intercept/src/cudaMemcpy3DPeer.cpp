#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy3DPeer)(
const struct cudaMemcpy3DPeerParms *
);
extern "C"
{
	cudaError_t cudaMemcpy3DPeer(const struct cudaMemcpy3DPeerParms *p) {
		fprintf(stderr, "===============\ncudaMemcpy3DPeer()\n");
		char* __dlerror;
		if (!original_cudaMemcpy3DPeer)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy3DPeer = (cudaError_t (*)(
			const struct cudaMemcpy3DPeerParms *)
			) dlsym(RTLD_NEXT, "cudaMemcpy3DPeer");
			fprintf(stderr, "original_cudaMemcpy3DPeer:%p\n", original_cudaMemcpy3DPeer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy3DPeer():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy3DPeer(
		p
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}