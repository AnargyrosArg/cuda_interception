#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetArray)(
CUtexref, 
CUarray, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetArray(CUtexref hTexRef, CUarray hArray, unsigned int Flags) {
		fprintf(stderr, "===============\ncuTexRefSetArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetArray)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetArray = (CUresult (*)(
			CUtexref, 
			CUarray, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuTexRefSetArray");
			fprintf(stderr, "original_cuTexRefSetArray:%p\n", original_cuTexRefSetArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetArray():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexRefSetArray(
		hTexRef, 
		hArray, 
		Flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}