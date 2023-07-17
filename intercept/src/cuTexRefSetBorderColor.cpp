#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetBorderColor)(
CUtexref, 
float *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetBorderColor(CUtexref hTexRef, float *pBorderColor) {
		fprintf(stderr, "cuTexRefSetBorderColor()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetBorderColor)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetBorderColor = (CUresult (*)(
			CUtexref, 
			float *)
			) dlsym(original_libcuda_handle, "cuTexRefSetBorderColor");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetBorderColor():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefSetBorderColor(
		hTexRef, 
		pBorderColor
		);
	}
}