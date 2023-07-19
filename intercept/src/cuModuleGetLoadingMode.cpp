#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleGetLoadingMode)(
CUmoduleLoadingMode *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuModuleGetLoadingMode(CUmoduleLoadingMode *mode) {
		fprintf(stderr, "cuModuleGetLoadingMode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleGetLoadingMode)
		{
			//fetch the original function addr using dlsym
			original_cuModuleGetLoadingMode = (CUresult (*)(
			CUmoduleLoadingMode *)
			) dlsym(original_libcuda_handle, "cuModuleGetLoadingMode");
			fprintf(stderr, "original_cuModuleGetLoadingMode:%p\n", original_cuModuleGetLoadingMode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleGetLoadingMode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuModuleGetLoadingMode(
		mode
		);
	}
}