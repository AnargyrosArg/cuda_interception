#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuPointerGetAttributes)(
unsigned int, 
CUpointer_attribute *, 
void **, 
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuPointerGetAttributes(unsigned int numAttributes, CUpointer_attribute *attributes, void **data, CUdeviceptr ptr) {
		fprintf(stderr, "cuPointerGetAttributes()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuPointerGetAttributes)
		{
			//fetch the original function addr using dlsym
			original_cuPointerGetAttributes = (CUresult (*)(
			unsigned int, 
			CUpointer_attribute *, 
			void **, 
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuPointerGetAttributes");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuPointerGetAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuPointerGetAttributes(
		numAttributes, 
		attributes, 
		data, 
		ptr
		);
	}
}