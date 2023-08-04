#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleGetFunction)(
CUfunction *, 
CUmodule, 
const char *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuModuleGetFunction(CUfunction *hfunc, CUmodule hmod, const char *name) {
		fprintf(stderr, "===============\ncuModuleGetFunction()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleGetFunction)
		{
			//fetch the original function addr using dlsym
			original_cuModuleGetFunction = (CUresult (*)(
			CUfunction *, 
			CUmodule, 
			const char *)
			) dlsym(original_libcuda_handle, "cuModuleGetFunction");
			fprintf(stderr, "original_cuModuleGetFunction:%p\n", original_cuModuleGetFunction);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleGetFunction():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuModuleGetFunction(
		hfunc, 
		hmod, 
		name
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}