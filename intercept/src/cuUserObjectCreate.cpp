#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuUserObjectCreate)(
CUuserObject *, 
void *, 
CUhostFn, 
unsigned int, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuUserObjectCreate(CUuserObject *object_out, void *ptr, CUhostFn destroy, unsigned int initialRefcount, unsigned int flags) {
		fprintf(stderr, "cuUserObjectCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuUserObjectCreate)
		{
			//fetch the original function addr using dlsym
			original_cuUserObjectCreate = (CUresult (*)(
			CUuserObject *, 
			void *, 
			CUhostFn, 
			unsigned int, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuUserObjectCreate");
			fprintf(stderr, "original_cuUserObjectCreate:%p\n", original_cuUserObjectCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuUserObjectCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuUserObjectCreate(
		object_out, 
		ptr, 
		destroy, 
		initialRefcount, 
		flags
		);
	}
}