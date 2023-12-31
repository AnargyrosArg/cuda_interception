#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuParamSetSize)(
CUfunction, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuParamSetSize(CUfunction hfunc, unsigned int numbytes) {
		fprintf(stderr, "===============\ncuParamSetSize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuParamSetSize)
		{
			//fetch the original function addr using dlsym
			original_cuParamSetSize = (CUresult (*)(
			CUfunction, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuParamSetSize");
			fprintf(stderr, "original_cuParamSetSize:%p\n", original_cuParamSetSize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuParamSetSize():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuParamSetSize(
		hfunc, 
		numbytes
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}