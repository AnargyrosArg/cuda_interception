#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetMipmapLevelBias)(
float *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetMipmapLevelBias(float *pbias, CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefGetMipmapLevelBias()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetMipmapLevelBias)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetMipmapLevelBias = (CUresult (*)(
			float *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetMipmapLevelBias");
			fprintf(stderr, "original_cuTexRefGetMipmapLevelBias:%p\n", original_cuTexRefGetMipmapLevelBias);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetMipmapLevelBias():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetMipmapLevelBias(
		pbias, 
		hTexRef
		);
	}
}