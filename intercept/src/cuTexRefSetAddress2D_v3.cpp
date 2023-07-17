#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexRefSetAddress2D_v3)(
CUtexref, 
const CUDA_ARRAY_DESCRIPTOR *, 
CUdeviceptr, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexRefSetAddress2D_v3(CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR *desc, CUdeviceptr dptr, size_t Pitch) {
		fprintf(stderr, "cuTexRefSetAddress2D_v3()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexRefSetAddress2D_v3)
		{
			//fetch the original function addr using dlsym
			original_cuTexRefSetAddress2D_v3 = (CUresult (*)(
			CUtexref, 
			const CUDA_ARRAY_DESCRIPTOR *, 
			CUdeviceptr, 
			size_t)
			) dlsym(original_libcuda_handle, "cuTexRefSetAddress2D_v3");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexRefSetAddress2D_v3():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexRefSetAddress2D_v3(
		hTexRef, 
		desc, 
		dptr, 
		Pitch
		);
	}
}