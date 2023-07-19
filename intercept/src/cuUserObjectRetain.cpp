#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuUserObjectRetain)(
CUuserObject, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuUserObjectRetain(CUuserObject object, unsigned int count) {
		fprintf(stderr, "cuUserObjectRetain()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuUserObjectRetain)
		{
			//fetch the original function addr using dlsym
			original_cuUserObjectRetain = (CUresult (*)(
			CUuserObject, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuUserObjectRetain");
			fprintf(stderr, "original_cuUserObjectRetain:%p\n", original_cuUserObjectRetain);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuUserObjectRetain():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuUserObjectRetain(
		object, 
		count
		);
	}
}