#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyPeerAsync)(
void *, 
int, 
const void *, 
int, 
size_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpyPeerAsync(void *dst, int dstDevice, const void *src, int srcDevice, size_t count, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpyPeerAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpyPeerAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyPeerAsync = (cudaError_t (*)(
			void *, 
			int, 
			const void *, 
			int, 
			size_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpyPeerAsync");
			fprintf(stderr, "original_cudaMemcpyPeerAsync:%p\n", original_cudaMemcpyPeerAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyPeerAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyPeerAsync(
		dst, 
		dstDevice, 
		src, 
		srcDevice, 
		count, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}