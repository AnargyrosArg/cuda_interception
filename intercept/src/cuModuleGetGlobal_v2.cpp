#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleGetGlobal_v2)(
CUdeviceptr *, 
size_t *, 
CUmodule, 
const char *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuModuleGetGlobal_v2(CUdeviceptr *dptr, size_t *bytes, CUmodule hmod, const char *name) {
		fprintf(stderr, "cuModuleGetGlobal_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleGetGlobal_v2)
		{
			//fetch the original function addr using dlsym
			original_cuModuleGetGlobal_v2 = (CUresult (*)(
			CUdeviceptr *, 
			size_t *, 
			CUmodule, 
			const char *)
			) dlsym(original_libcuda_handle, "cuModuleGetGlobal_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleGetGlobal_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuModuleGetGlobal_v2(
		dptr, 
		bytes, 
		hmod, 
		name
		);
	}
}