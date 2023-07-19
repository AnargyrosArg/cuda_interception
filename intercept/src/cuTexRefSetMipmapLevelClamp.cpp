#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetMipmapLevelClamp)(
CUtexref, 
float, 
float
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetMipmapLevelClamp(CUtexref hTexRef, float minMipmapLevelClamp, float maxMipmapLevelClamp) {
		fprintf(stderr, "cuTexRefSetMipmapLevelClamp()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetMipmapLevelClamp)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetMipmapLevelClamp = (CUresult (*)(
			CUtexref, 
			float, 
			float)
			) dlsym(original_libcuda_handle, "cuTexRefSetMipmapLevelClamp");
			fprintf(stderr, "original_cuTexRefSetMipmapLevelClamp:%p\n", original_cuTexRefSetMipmapLevelClamp);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetMipmapLevelClamp():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefSetMipmapLevelClamp(
		hTexRef, 
		minMipmapLevelClamp, 
		maxMipmapLevelClamp
		);
	}
}