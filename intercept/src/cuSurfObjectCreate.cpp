#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuSurfObjectCreate)(
CUsurfObject *, 
const CUDA_RESOURCE_DESC *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuSurfObjectCreate(CUsurfObject *pSurfObject, const CUDA_RESOURCE_DESC *pResDesc) {
		fprintf(stderr, "===============\ncuSurfObjectCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuSurfObjectCreate)
		{
			//fetch the original function addr using dlsym
			original_cuSurfObjectCreate = (CUresult (*)(
			CUsurfObject *, 
			const CUDA_RESOURCE_DESC *)
			) dlsym(original_libcuda_handle, "cuSurfObjectCreate");
			fprintf(stderr, "original_cuSurfObjectCreate:%p\n", original_cuSurfObjectCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuSurfObjectCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuSurfObjectCreate(
		pSurfObject, 
		pResDesc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}