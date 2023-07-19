#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetArray)(
CUarray *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetArray(CUarray *phArray, CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefGetArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetArray)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetArray = (CUresult (*)(
			CUarray *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetArray");
			fprintf(stderr, "original_cuTexRefGetArray:%p\n", original_cuTexRefGetArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetArray():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetArray(
		phArray, 
		hTexRef
		);
	}
}