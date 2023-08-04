#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolSetAccess)(
CUmemoryPool, 
const CUmemAccessDesc *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolSetAccess(CUmemoryPool pool, const CUmemAccessDesc *map, size_t count) {
		fprintf(stderr, "===============\ncuMemPoolSetAccess()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolSetAccess)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolSetAccess = (CUresult (*)(
			CUmemoryPool, 
			const CUmemAccessDesc *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemPoolSetAccess");
			fprintf(stderr, "original_cuMemPoolSetAccess:%p\n", original_cuMemPoolSetAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolSetAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemPoolSetAccess(
		pool, 
		map, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}