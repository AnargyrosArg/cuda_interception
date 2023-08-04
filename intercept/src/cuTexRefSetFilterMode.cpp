#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetFilterMode)(
CUtexref, 
CUfilter_mode
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetFilterMode(CUtexref hTexRef, CUfilter_mode fm) {
		fprintf(stderr, "===============\ncuTexRefSetFilterMode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetFilterMode)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetFilterMode = (CUresult (*)(
			CUtexref, 
			CUfilter_mode)
			) dlsym(original_libcuda_handle, "cuTexRefSetFilterMode");
			fprintf(stderr, "original_cuTexRefSetFilterMode:%p\n", original_cuTexRefSetFilterMode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetFilterMode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexRefSetFilterMode(
		hTexRef, 
		fm
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}