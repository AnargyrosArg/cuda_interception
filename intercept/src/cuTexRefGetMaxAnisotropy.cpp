#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefGetMaxAnisotropy)(
int *, 
CUtexref
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefGetMaxAnisotropy(int *pmaxAniso, CUtexref hTexRef) {
		fprintf(stderr, "cuTexRefGetMaxAnisotropy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefGetMaxAnisotropy)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefGetMaxAnisotropy = (CUresult (*)(
			int *, 
			CUtexref)
			) dlsym(original_libcuda_handle, "cuTexRefGetMaxAnisotropy");
			fprintf(stderr, "original_cuTexRefGetMaxAnisotropy:%p\n", original_cuTexRefGetMaxAnisotropy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefGetMaxAnisotropy():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefGetMaxAnisotropy(
		pmaxAniso, 
		hTexRef
		);
	}
}