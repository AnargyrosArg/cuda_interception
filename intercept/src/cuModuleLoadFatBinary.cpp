#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleLoadFatBinary)(
CUmodule *, 
const void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuModuleLoadFatBinary(CUmodule *module, const void *fatCubin) {
		fprintf(stderr, "cuModuleLoadFatBinary()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleLoadFatBinary)
		{
			//fetch the original function addr using dlsym
			original_cuModuleLoadFatBinary = (CUresult (*)(
			CUmodule *, 
			const void *)
			) dlsym(original_libcuda_handle, "cuModuleLoadFatBinary");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleLoadFatBinary():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuModuleLoadFatBinary(
		module, 
		fatCubin
		);
	}
}