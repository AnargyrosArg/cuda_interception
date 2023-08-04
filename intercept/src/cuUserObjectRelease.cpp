#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuUserObjectRelease)(
CUuserObject, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuUserObjectRelease(CUuserObject object, unsigned int count) {
		fprintf(stderr, "===============\ncuUserObjectRelease()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuUserObjectRelease)
		{
			//fetch the original function addr using dlsym
			original_cuUserObjectRelease = (CUresult (*)(
			CUuserObject, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuUserObjectRelease");
			fprintf(stderr, "original_cuUserObjectRelease:%p\n", original_cuUserObjectRelease);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuUserObjectRelease():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuUserObjectRelease(
		object, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}