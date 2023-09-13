#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaLaunchCooperativeKernel)(
const void *, 
dim3, 
dim3, 
void **, 
size_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaLaunchCooperativeKernel(const void *func, dim3 gridDim, dim3 blockDim, void **args, size_t sharedMem, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaLaunchCooperativeKernel()\n");
		char* __dlerror;
		if (!original_cudaLaunchCooperativeKernel)
		{
			//fetch the original function addr using dlsym
			original_cudaLaunchCooperativeKernel = (cudaError_t (*)(
			const void *, 
			dim3, 
			dim3, 
			void **, 
			size_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaLaunchCooperativeKernel");
			fprintf(stderr, "original_cudaLaunchCooperativeKernel:%p\n", original_cudaLaunchCooperativeKernel);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaLaunchCooperativeKernel():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaLaunchCooperativeKernel(
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