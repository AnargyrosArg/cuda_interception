#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetMipmapLevelClamp)(
float *, 
float *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetMipmapLevelClamp(float *pminMipmapLevelClamp, float *pmaxMipmapLevelClamp, CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefGetMipmapLevelClamp()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetMipmapLevelClamp)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetMipmapLevelClamp = (CUresult (*)(
			float *, 
			float *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetMipmapLevelClamp");
			fprintf(stderr, "original_cuTexRefGetMipmapLevelClamp:%p\n", original_cuTexRefGetMipmapLevelClamp);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetMipmapLevelClamp():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetMipmapLevelClamp(
		pminMipmapLevelClamp, 
		pmaxMipmapLevelClamp, 
		hTexRef
		);
	}
}