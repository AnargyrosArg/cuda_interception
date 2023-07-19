#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsResourceGetMappedMipmappedArray)(
CUmipmappedArray *, 
CUgraphicsResource
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsResourceGetMappedMipmappedArray(CUmipmappedArray *pMipmappedArray, CUgraphicsResource resource) {
		fprintf(stderr, "cuGraphicsResourceGetMappedMipmappedArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsResourceGetMappedMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsResourceGetMappedMipmappedArray = (CUresult (*)(
			CUmipmappedArray *, 
			CUgraphicsResource)
			) dlsym(original_libcuda_handle, "cuGraphicsResourceGetMappedMipmappedArray");
			fprintf(stderr, "original_cuGraphicsResourceGetMappedMipmappedArray:%p\n", original_cuGraphicsResourceGetMappedMipmappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsResourceGetMappedMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphicsResourceGetMappedMipmappedArray(
		pMipmappedArray, 
		resource
		);
	}
}