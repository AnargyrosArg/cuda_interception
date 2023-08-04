#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLaunchKernel)(
CUfunction, 
unsigned int, 
unsigned int, 
unsigned int, 
unsigned int, 
unsigned int, 
unsigned int, 
unsigned int, 
CUstream, 
void **, 
void **
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLaunchKernel(CUfunction f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ, unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ, unsigned int sharedMemBytes, CUstream hStream, void **kernelParams, void **extra) {
		fprintf(stderr, "===============\ncuLaunchKernel()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLaunchKernel)
		{
			//fetch the original function addr using dlsym
			original_cuLaunchKernel = (CUresult (*)(
			CUfunction, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			CUstream, 
			void **, 
			void **)
			) dlsym(original_libcuda_handle, "cuLaunchKernel");
			fprintf(stderr, "original_cuLaunchKernel:%p\n", original_cuLaunchKernel);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLaunchKernel():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuLaunchKernel(
		f, 
		gridDimX, 
		gridDimY, 
		gridDimZ, 
		blockDimX, 
		blockDimY, 
		blockDimZ, 
		sharedMemBytes, 
		hStream, 
		kernelParams, 
		extra
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}