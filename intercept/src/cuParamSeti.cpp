#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuParamSeti)(
CUfunction, 
int, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuParamSeti(CUfunction hfunc, int offset, unsigned int value) {
		fprintf(stderr, "===============\ncuParamSeti()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuParamSeti)
		{
			//fetch the original function addr using dlsym
			original_cuParamSeti = (CUresult (*)(
			CUfunction, 
			int, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuParamSeti");
			fprintf(stderr, "original_cuParamSeti:%p\n", original_cuParamSeti);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuParamSeti():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuParamSeti(
		hfunc, 
		offset, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}