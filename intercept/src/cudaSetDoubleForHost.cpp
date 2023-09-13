#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaSetDoubleForHost)(
double *
);
extern "C"
{
	cudaError_t cudaSetDoubleForHost(double *d) {
		fprintf(stderr, "===============\ncudaSetDoubleForHost()\n");
		char* __dlerror;
		if (!original_cudaSetDoubleForHost)
		{
			//fetch the original function addr using dlsym
			original_cudaSetDoubleForHost = (cudaError_t (*)(
			double *)
			) dlsym(RTLD_NEXT, "cudaSetDoubleForHost");
			fprintf(stderr, "original_cudaSetDoubleForHost:%p\n", original_cudaSetDoubleForHost);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaSetDoubleForHost():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaSetDoubleForHost(
		d
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}