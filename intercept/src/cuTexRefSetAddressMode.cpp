#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetAddressMode)(
CUtexref, 
int, 
CUaddress_mode
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetAddressMode(CUtexref hTexRef, int dim, CUaddress_mode am) {
		fprintf(stderr, "===============\ncuTexRefSetAddressMode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetAddressMode)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetAddressMode = (CUresult (*)(
			CUtexref, 
			int, 
			CUaddress_mode)
			) dlsym(original_libcuda_handle, "cuTexRefSetAddressMode");
			fprintf(stderr, "original_cuTexRefSetAddressMode:%p\n", original_cuTexRefSetAddressMode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetAddressMode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexRefSetAddressMode(
		hTexRef, 
		dim, 
		am
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}