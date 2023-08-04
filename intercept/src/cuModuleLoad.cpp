#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleLoad)(
CUmodule *, 
const char *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuModuleLoad(CUmodule *module, const char *fname) {
		fprintf(stderr, "===============\ncuModuleLoad()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleLoad)
		{
			//fetch the original function addr using dlsym
			original_cuModuleLoad = (CUresult (*)(
			CUmodule *, 
			const char *)
			) dlsym(original_libcuda_handle, "cuModuleLoad");
			fprintf(stderr, "original_cuModuleLoad:%p\n", original_cuModuleLoad);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleLoad():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuModuleLoad(
		module, 
		fname
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}