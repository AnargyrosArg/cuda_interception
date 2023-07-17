#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetAddress_v2)(
size_t *, 
CUtexref, 
CUdeviceptr, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetAddress_v2(size_t *ByteOffset, CUtexref hTexRef, CUdeviceptr dptr, size_t bytes) {
		fprintf(stderr, "cuTexRefSetAddress_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetAddress_v2)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetAddress_v2 = (CUresult (*)(
			size_t *, 
			CUtexref, 
			CUdeviceptr, 
			size_t)
			) dlsym(original_libcuda_handle, "cuTexRefSetAddress_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetAddress_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefSetAddress_v2(
		ByteOffset, 
		hTexRef, 
		dptr, 
		bytes
		);
	}
}