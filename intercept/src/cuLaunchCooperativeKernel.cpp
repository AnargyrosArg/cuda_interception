#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLaunchCooperativeKernel)(
CUfunction, 
unsigned int, 
unsigned int, 
unsigned int, 
unsigned int, 
unsigned int, 
unsigned int, 
unsigned int, 
CUstream, 
void **
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLaunchCooperativeKernel(CUfunction f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ, unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ, unsigned int sharedMemBytes, CUstream hStream, void **kernelParams) {
		fprintf(stderr, "===============\ncuLaunchCooperativeKernel()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLaunchCooperativeKernel)
		{
			//fetch the original function addr using dlsym
			original_cuLaunchCooperativeKernel = (CUresult (*)(
			CUfunction, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			unsigned int, 
			CUstream, 
			void **)
			) dlsym(original_libcuda_handle, "cuLaunchCooperativeKernel");
			fprintf(stderr, "original_cuLaunchCooperativeKernel:%p\n", original_cuLaunchCooperativeKernel);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLaunchCooperativeKernel():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuLaunchCooperativeKernel(
		f, 
		gridDimX, 
		gridDimY, 
		gridDimZ, 
		blockDimX, 
		blockDimY, 
		blockDimZ, 
		sharedMemBytes, 
		hStream, 
		kernelParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}