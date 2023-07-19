#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetMipmapFilterMode)(
CUtexref, 
CUfilter_mode
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetMipmapFilterMode(CUtexref hTexRef, CUfilter_mode fm) {
		fprintf(stderr, "cuTexRefSetMipmapFilterMode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetMipmapFilterMode)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetMipmapFilterMode = (CUresult (*)(
			CUtexref, 
			CUfilter_mode)
			) dlsym(original_libcuda_handle, "cuTexRefSetMipmapFilterMode");
			fprintf(stderr, "original_cuTexRefSetMipmapFilterMode:%p\n", original_cuTexRefSetMipmapFilterMode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetMipmapFilterMode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefSetMipmapFilterMode(
		hTexRef, 
		fm
		);
	}
}