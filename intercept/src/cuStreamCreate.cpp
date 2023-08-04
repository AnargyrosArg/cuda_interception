#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamCreate)(
CUstream *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamCreate(CUstream *phStream, unsigned int Flags) {
		fprintf(stderr, "===============\ncuStreamCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamCreate)
		{
			//fetch the original function addr using dlsym
			original_cuStreamCreate = (CUresult (*)(
			CUstream *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamCreate");
			fprintf(stderr, "original_cuStreamCreate:%p\n", original_cuStreamCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamCreate(
		phStream, 
		Flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}