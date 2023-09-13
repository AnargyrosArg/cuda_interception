#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaLaunchHostFunc)(
cudaStream_t, 
cudaHostFn_t, 
void *
);
extern "C"
{
	cudaError_t cudaLaunchHostFunc(cudaStream_t stream, cudaHostFn_t fn, void *userData) {
		fprintf(stderr, "===============\ncudaLaunchHostFunc()\n");
		char* __dlerror;
		if (!original_cudaLaunchHostFunc)
		{
			//fetch the original function addr using dlsym
			original_cudaLaunchHostFunc = (cudaError_t (*)(
			cudaStream_t, 
			cudaHostFn_t, 
			void *)
			) dlsym(RTLD_NEXT, "cudaLaunchHostFunc");
			fprintf(stderr, "original_cudaLaunchHostFunc:%p\n", original_cudaLaunchHostFunc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaLaunchHostFunc():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaLaunchHostFunc(
		stream, 
		fn, 
		userData
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}