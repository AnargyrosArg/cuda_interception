#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGetErrorString)(
CUresult, 
const char **
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGetErrorString(CUresult error, const char **pStr) {
		fprintf(stderr, "===============\ncuGetErrorString()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGetErrorString)
		{
			//fetch the original function addr using dlsym
			original_cuGetErrorString = (CUresult (*)(
			CUresult, 
			const char **)
			) dlsym(original_libcuda_handle, "cuGetErrorString");
			fprintf(stderr, "original_cuGetErrorString:%p\n", original_cuGetErrorString);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGetErrorString():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGetErrorString(
		error, 
		pStr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}