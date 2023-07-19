#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuParamSetTexRef)(
CUfunction, 
int, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuParamSetTexRef(CUfunction hfunc, int texunit, CUtexref hTexRef) {
		fprintf(stderr, "cuParamSetTexRef()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuParamSetTexRef)
		{
			//fetch the original function addr using dlsym
			original_cuParamSetTexRef = (CUresult (*)(
			CUfunction, 
			int, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuParamSetTexRef");
			fprintf(stderr, "original_cuParamSetTexRef:%p\n", original_cuParamSetTexRef);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuParamSetTexRef():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuParamSetTexRef(
		hfunc, 
		texunit, 
		hTexRef
		);
	}
}