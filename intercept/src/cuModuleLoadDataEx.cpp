#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleLoadDataEx)(
CUmodule *, 
const void *, 
unsigned int, 
CUjit_option *, 
void **
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuModuleLoadDataEx(CUmodule *module, const void *image, unsigned int numOptions, CUjit_option *options, void **optionValues) {
		fprintf(stderr, "cuModuleLoadDataEx()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleLoadDataEx)
		{
			//fetch the original function addr using dlsym
			original_cuModuleLoadDataEx = (CUresult (*)(
			CUmodule *, 
			const void *, 
			unsigned int, 
			CUjit_option *, 
			void **)
			) dlsym(original_libcuda_handle, "cuModuleLoadDataEx");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleLoadDataEx():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuModuleLoadDataEx(
		module, 
		image, 
		numOptions, 
		options, 
		optionValues
		);
	}
}