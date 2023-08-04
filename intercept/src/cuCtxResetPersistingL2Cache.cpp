#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxResetPersistingL2Cache)(
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxResetPersistingL2Cache() {
		fprintf(stderr, "===============\ncuCtxResetPersistingL2Cache()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxResetPersistingL2Cache)
		{
			//fetch the original function addr using dlsym
			original_cuCtxResetPersistingL2Cache = (CUresult (*)(
			void)
			) dlsym(original_libcuda_handle, "cuCtxResetPersistingL2Cache");
			fprintf(stderr, "original_cuCtxResetPersistingL2Cache:%p\n", original_cuCtxResetPersistingL2Cache);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxResetPersistingL2Cache():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuCtxResetPersistingL2Cache(
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}