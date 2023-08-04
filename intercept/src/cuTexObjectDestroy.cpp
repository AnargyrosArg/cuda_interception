#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexObjectDestroy)(
CUtexObject
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexObjectDestroy(CUtexObject texObject) {
		fprintf(stderr, "===============\ncuTexObjectDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexObjectDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuTexObjectDestroy = (CUresult (*)(
			CUtexObject)
			) dlsym(original_libcuda_handle, "cuTexObjectDestroy");
			fprintf(stderr, "original_cuTexObjectDestroy:%p\n", original_cuTexObjectDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexObjectDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexObjectDestroy(
		texObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}