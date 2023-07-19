#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetMipmapLevelBias)(
CUtexref, 
float
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetMipmapLevelBias(CUtexref hTexRef, float bias) {
		fprintf(stderr, "cuTexRefSetMipmapLevelBias()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetMipmapLevelBias)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetMipmapLevelBias = (CUresult (*)(
			CUtexref, 
			float)
			) dlsym(original_libcuda_handle, "cuTexRefSetMipmapLevelBias");
			fprintf(stderr, "original_cuTexRefSetMipmapLevelBias:%p\n", original_cuTexRefSetMipmapLevelBias);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetMipmapLevelBias():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefSetMipmapLevelBias(
		hTexRef, 
		bias
		);
	}
}