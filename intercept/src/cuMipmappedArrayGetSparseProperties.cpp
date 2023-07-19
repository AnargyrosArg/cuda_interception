#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMipmappedArrayGetSparseProperties)(
CUDA_ARRAY_SPARSE_PROPERTIES *, 
CUmipmappedArray
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMipmappedArrayGetSparseProperties(CUDA_ARRAY_SPARSE_PROPERTIES *sparseProperties, CUmipmappedArray mipmap) {
		fprintf(stderr, "cuMipmappedArrayGetSparseProperties()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMipmappedArrayGetSparseProperties)
		{
			//fetch the original function addr using dlsym
			original_cuMipmappedArrayGetSparseProperties = (CUresult (*)(
			CUDA_ARRAY_SPARSE_PROPERTIES *, 
			CUmipmappedArray)
			) dlsym(original_libcuda_handle, "cuMipmappedArrayGetSparseProperties");
			fprintf(stderr, "original_cuMipmappedArrayGetSparseProperties:%p\n", original_cuMipmappedArrayGetSparseProperties);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMipmappedArrayGetSparseProperties():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMipmappedArrayGetSparseProperties(
		sparseProperties, 
		mipmap
		);
	}
}