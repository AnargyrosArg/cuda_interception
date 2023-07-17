#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuParamSetf)(
CUfunction, 
int, 
float
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuParamSetf(CUfunction hfunc, int offset, float value) {
		fprintf(stderr, "cuParamSetf()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuParamSetf)
		{
			//fetch the original function addr using dlsym
			original_cuParamSetf = (CUresult (*)(
			CUfunction, 
			int, 
			float)
			) dlsym(original_libcuda_handle, "cuParamSetf");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuParamSetf():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuParamSetf(
		hfunc, 
		offset, 
		value
		);
	}
}