#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleGetSurfRef)(
CUsurfref *, 
CUmodule, 
const char *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuModuleGetSurfRef(CUsurfref *pSurfRef, CUmodule hmod, const char *name) {
		fprintf(stderr, "===============\ncuModuleGetSurfRef()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleGetSurfRef)
		{
			//fetch the original function addr using dlsym
			original_cuModuleGetSurfRef = (CUresult (*)(
			CUsurfref *, 
			CUmodule, 
			const char *)
			) dlsym(original_libcuda_handle, "cuModuleGetSurfRef");
			fprintf(stderr, "original_cuModuleGetSurfRef:%p\n", original_cuModuleGetSurfRef);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleGetSurfRef():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuModuleGetSurfRef(
		pSurfRef, 
		hmod, 
		name
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}