#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaSetDoubleForDevice)(
double *
);
extern "C"
{
	cudaError_t cudaSetDoubleForDevice(double *d) {
		fprintf(stderr, "===============\ncudaSetDoubleForDevice()\n");
		char* __dlerror;
		if (!original_cudaSetDoubleForDevice)
		{
			//fetch the original function addr using dlsym
			original_cudaSetDoubleForDevice = (cudaError_t (*)(
			double *)
			) dlsym(RTLD_NEXT, "cudaSetDoubleForDevice");
			fprintf(stderr, "original_cudaSetDoubleForDevice:%p\n", original_cudaSetDoubleForDevice);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaSetDoubleForDevice():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaSetDoubleForDevice(
		d
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}