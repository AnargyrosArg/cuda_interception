#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLaunchGridAsync)(
CUfunction, 
int, 
int, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLaunchGridAsync(CUfunction f, int grid_width, int grid_height, CUstream hStream) {
		fprintf(stderr, "cuLaunchGridAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLaunchGridAsync)
		{
			//fetch the original function addr using dlsym
			original_cuLaunchGridAsync = (CUresult (*)(
			CUfunction, 
			int, 
			int, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuLaunchGridAsync");
			fprintf(stderr, "original_cuLaunchGridAsync:%p\n", original_cuLaunchGridAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLaunchGridAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLaunchGridAsync(
		f, 
		grid_width, 
		grid_height, 
		hStream
		);
	}
}