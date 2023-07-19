#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLaunchGrid)(
CUfunction, 
int, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLaunchGrid(CUfunction f, int grid_width, int grid_height) {
		fprintf(stderr, "cuLaunchGrid()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLaunchGrid)
		{
			//fetch the original function addr using dlsym
			original_cuLaunchGrid = (CUresult (*)(
			CUfunction, 
			int, 
			int)
			) dlsym(original_libcuda_handle, "cuLaunchGrid");
			fprintf(stderr, "original_cuLaunchGrid:%p\n", original_cuLaunchGrid);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLaunchGrid():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLaunchGrid(
		f, 
		grid_width, 
		grid_height
		);
	}
}