#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetMaxAnisotropy)(
CUtexref, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetMaxAnisotropy(CUtexref hTexRef, unsigned int maxAniso) {
		fprintf(stderr, "===============\ncuTexRefSetMaxAnisotropy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetMaxAnisotropy)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetMaxAnisotropy = (CUresult (*)(
			CUtexref, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuTexRefSetMaxAnisotropy");
			fprintf(stderr, "original_cuTexRefSetMaxAnisotropy:%p\n", original_cuTexRefSetMaxAnisotropy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetMaxAnisotropy():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexRefSetMaxAnisotropy(
		hTexRef, 
		maxAniso
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}