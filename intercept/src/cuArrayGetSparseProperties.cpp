#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuArrayGetSparseProperties)(
CUDA_ARRAY_SPARSE_PROPERTIES *, 
CUarray
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuArrayGetSparseProperties(CUDA_ARRAY_SPARSE_PROPERTIES *sparseProperties, CUarray array) {
		fprintf(stderr, "cuArrayGetSparseProperties()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuArrayGetSparseProperties)
		{
			//fetch the original function addr using dlsym
			original_cuArrayGetSparseProperties = (CUresult (*)(
			CUDA_ARRAY_SPARSE_PROPERTIES *, 
			CUarray)
			) dlsym(original_libcuda_handle, "cuArrayGetSparseProperties");
			fprintf(stderr, "original_cuArrayGetSparseProperties:%p\n", original_cuArrayGetSparseProperties);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuArrayGetSparseProperties():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuArrayGetSparseProperties(
		sparseProperties, 
		array
		);
	}
}