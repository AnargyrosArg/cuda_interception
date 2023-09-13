#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFreeHost)(
void *
);
extern "C"
{
	cudaError_t cudaFreeHost(void *ptr) {
		fprintf(stderr, "===============\ncudaFreeHost()\n");
		char* __dlerror;
		if (!original_cudaFreeHost)
		{
			//fetch the original function addr using dlsym
			original_cudaFreeHost = (cudaError_t (*)(
			void *)
			) dlsym(RTLD_NEXT, "cudaFreeHost");
			fprintf(stderr, "original_cudaFreeHost:%p\n", original_cudaFreeHost);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFreeHost():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFreeHost(
		ptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}