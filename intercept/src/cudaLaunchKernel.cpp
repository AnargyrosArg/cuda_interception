#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaLaunchKernel)(
const void *, 
dim3, 
dim3, 
void **, 
size_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaLaunchKernel(const void *func, dim3 gridDim, dim3 blockDim, void **args, size_t sharedMem, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaLaunchKernel()\n");
		char* __dlerror;
		if (!original_cudaLaunchKernel)
		{
			//fetch the original function addr using dlsym
			original_cudaLaunchKernel = (cudaError_t (*)(
			const void *, 
			dim3, 
			dim3, 
			void **, 
			size_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaLaunchKernel");
			fprintf(stderr, "original_cudaLaunchKernel:%p\n", original_cudaLaunchKernel);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaLaunchKernel():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaLaunchKernel(
		func, 
		gridDim, 
		blockDim, 
		args, 
		sharedMem, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}