#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuFuncGetModule)(
CUmodule *, 
CUfunction
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuFuncGetModule(CUmodule *hmod, CUfunction hfunc) {
		fprintf(stderr, "===============\ncuFuncGetModule()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuFuncGetModule)
		{
			//fetch the original function addr using dlsym
			original_cuFuncGetModule = (CUresult (*)(
			CUmodule *, 
			CUfunction)
			) dlsym(original_libcuda_handle, "cuFuncGetModule");
			fprintf(stderr, "original_cuFuncGetModule:%p\n", original_cuFuncGetModule);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuFuncGetModule():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuFuncGetModule(
		hmod, 
		hfunc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}