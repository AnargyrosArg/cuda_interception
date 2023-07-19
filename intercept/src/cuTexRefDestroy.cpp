#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefDestroy)(
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefDestroy(CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefDestroy = (CUresult (*)(
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefDestroy");
			fprintf(stderr, "original_cuTexRefDestroy:%p\n", original_cuTexRefDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefDestroy(
		hTexRef
		);
	}
}