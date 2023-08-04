#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGetErrorName)(
CUresult, 
const char **
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGetErrorName(CUresult error, const char **pStr) {
		fprintf(stderr, "===============\ncuGetErrorName()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGetErrorName)
		{
			//fetch the original function addr using dlsym
			original_cuGetErrorName = (CUresult (*)(
			CUresult, 
			const char **)
			) dlsym(original_libcuda_handle, "cuGetErrorName");
			fprintf(stderr, "original_cuGetErrorName:%p\n", original_cuGetErrorName);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGetErrorName():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGetErrorName(
		error, 
		pStr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}