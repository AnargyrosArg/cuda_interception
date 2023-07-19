#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetBorderColor)(
float *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetBorderColor(float *pBorderColor, CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefGetBorderColor()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetBorderColor)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetBorderColor = (CUresult (*)(
			float *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetBorderColor");
			fprintf(stderr, "original_cuTexRefGetBorderColor:%p\n", original_cuTexRefGetBorderColor);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetBorderColor():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetBorderColor(
		pBorderColor, 
		hTexRef
		);
	}
}