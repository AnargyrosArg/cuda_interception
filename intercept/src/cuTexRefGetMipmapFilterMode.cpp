#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetMipmapFilterMode)(
CUfilter_mode *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetMipmapFilterMode(CUfilter_mode *pfm, CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefGetMipmapFilterMode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetMipmapFilterMode)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetMipmapFilterMode = (CUresult (*)(
			CUfilter_mode *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetMipmapFilterMode");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetMipmapFilterMode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetMipmapFilterMode(
		pfm, 
		hTexRef
		);
	}
}