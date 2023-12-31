#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetFormat)(
CUarray_format *, 
int *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetFormat(CUarray_format *pFormat, int *pNumChannels, CUtexref hTexRef) {
		fprintf(stderr, "===============\ncuTexRefGetFormat()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetFormat)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetFormat = (CUresult (*)(
			CUarray_format *, 
			int *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetFormat");
			fprintf(stderr, "original_cuTexRefGetFormat:%p\n", original_cuTexRefGetFormat);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetFormat():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexRefGetFormat(
		pFormat, 
		pNumChannels, 
		hTexRef
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}