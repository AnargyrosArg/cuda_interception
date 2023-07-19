#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsSubResourceGetMappedArray)(
CUarray *, 
CUgraphicsResource, 
unsigned int, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsSubResourceGetMappedArray(CUarray *pArray, CUgraphicsResource resource, unsigned int arrayIndex, unsigned int mipLevel) {
		fprintf(stderr, "cuGraphicsSubResourceGetMappedArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsSubResourceGetMappedArray)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsSubResourceGetMappedArray = (CUresult (*)(
			CUarray *, 
			CUgraphicsResource, 
			unsigned int, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphicsSubResourceGetMappedArray");
			fprintf(stderr, "original_cuGraphicsSubResourceGetMappedArray:%p\n", original_cuGraphicsSubResourceGetMappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsSubResourceGetMappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphicsSubResourceGetMappedArray(
		pArray, 
		resource, 
		arrayIndex, 
		mipLevel
		);
	}
}