#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolGetAttribute)(
CUmemoryPool, 
CUmemPool_attribute, 
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolGetAttribute(CUmemoryPool pool, CUmemPool_attribute attr, void *value) {
		fprintf(stderr, "===============\ncuMemPoolGetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolGetAttribute = (CUresult (*)(
			CUmemoryPool, 
			CUmemPool_attribute, 
			void *)
			) dlsym(original_libcuda_handle, "cuMemPoolGetAttribute");
			fprintf(stderr, "original_cuMemPoolGetAttribute:%p\n", original_cuMemPoolGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemPoolGetAttribute(
		pool, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}