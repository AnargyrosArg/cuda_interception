#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetFormat)(
CUtexref, 
CUarray_format, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetFormat(CUtexref hTexRef, CUarray_format fmt, int NumPackedComponents) {
		fprintf(stderr, "cuTexRefSetFormat()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetFormat)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetFormat = (CUresult (*)(
			CUtexref, 
			CUarray_format, 
			int)
			) dlsym(original_libcuda_handle, "cuTexRefSetFormat");
			fprintf(stderr, "original_cuTexRefSetFormat:%p\n", original_cuTexRefSetFormat);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetFormat():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefSetFormat(
		hTexRef, 
		fmt, 
		NumPackedComponents
		);
	}
}