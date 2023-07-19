#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetFlags)(
unsigned int *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetFlags(unsigned int *pFlags, CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefGetFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetFlags)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetFlags = (CUresult (*)(
			unsigned int *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetFlags");
			fprintf(stderr, "original_cuTexRefGetFlags:%p\n", original_cuTexRefGetFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetFlags(
		pFlags, 
		hTexRef
		);
	}
}