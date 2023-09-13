/*#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
void (*original_cudaStreamCallback_t)(
cudaStream_t, 
cudaError_t, 
void *
);
extern "C"
{
	// void cudaStreamCallback_t(cudaStream_t stream, cudaError_t status, void *userData) {
		fprintf(stderr, "===============\ncudaStreamCallback_t()\n");
		char* __dlerror;
		if (!original_cudaStreamCallback_t)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamCallback_t = (void (*)(
			cudaStream_t, 
			cudaError_t, 
			void *)
			) dlsym(RTLD_NEXT, "cudaStreamCallback_t");
			fprintf(stderr, "original_cudaStreamCallback_t:%p\n", original_cudaStreamCallback_t);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamCallback_t():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamCallback_t(
		stream, 
		status, 
		userData
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}
*/
