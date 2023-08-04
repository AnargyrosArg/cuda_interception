#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetFlags)(
CUtexref, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetFlags(CUtexref hTexRef, unsigned int Flags) {
		fprintf(stderr, "===============\ncuTexRefSetFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetFlags)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetFlags = (CUresult (*)(
			CUtexref, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuTexRefSetFlags");
			fprintf(stderr, "original_cuTexRefSetFlags:%p\n", original_cuTexRefSetFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexRefSetFlags(
		hTexRef, 
		Flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}