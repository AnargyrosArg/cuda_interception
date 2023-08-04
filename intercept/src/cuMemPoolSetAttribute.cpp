#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolSetAttribute)(
CUmemoryPool, 
CUmemPool_attribute, 
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolSetAttribute(CUmemoryPool pool, CUmemPool_attribute attr, void *value) {
		fprintf(stderr, "===============\ncuMemPoolSetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolSetAttribute = (CUresult (*)(
			CUmemoryPool, 
			CUmemPool_attribute, 
			void *)
			) dlsym(original_libcuda_handle, "cuMemPoolSetAttribute");
			fprintf(stderr, "original_cuMemPoolSetAttribute:%p\n", original_cuMemPoolSetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemPoolSetAttribute(
		pool, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}