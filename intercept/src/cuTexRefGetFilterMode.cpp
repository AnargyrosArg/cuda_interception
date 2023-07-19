#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetFilterMode)(
CUfilter_mode *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetFilterMode(CUfilter_mode *pfm, CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefGetFilterMode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetFilterMode)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetFilterMode = (CUresult (*)(
			CUfilter_mode *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetFilterMode");
			fprintf(stderr, "original_cuTexRefGetFilterMode:%p\n", original_cuTexRefGetFilterMode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetFilterMode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetFilterMode(
		pfm, 
		hTexRef
		);
	}
}