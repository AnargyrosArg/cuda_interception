#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyPeer)(
void *, 
int, 
const void *, 
int, 
size_t
);
extern "C"
{
	cudaError_t cudaMemcpyPeer(void *dst, int dstDevice, const void *src, int srcDevice, size_t count) {
		fprintf(stderr, "===============\ncudaMemcpyPeer()\n");
		char* __dlerror;
		if (!original_cudaMemcpyPeer)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyPeer = (cudaError_t (*)(
			void *, 
			int, 
			const void *, 
			int, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMemcpyPeer");
			fprintf(stderr, "original_cudaMemcpyPeer:%p\n", original_cudaMemcpyPeer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyPeer():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyPeer(
		dst, 
		dstDevice, 
		src, 
		srcDevice, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}