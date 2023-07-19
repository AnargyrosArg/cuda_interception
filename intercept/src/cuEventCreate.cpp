#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEventCreate)(
CUevent *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEventCreate(CUevent *phEvent, unsigned int Flags) {
		fprintf(stderr, "cuEventCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEventCreate)
		{
			//fetch the original function addr using dlsym
			original_cuEventCreate = (CUresult (*)(
			CUevent *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuEventCreate");
			fprintf(stderr, "original_cuEventCreate:%p\n", original_cuEventCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEventCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEventCreate(
		phEvent, 
		Flags
		);
	}
}