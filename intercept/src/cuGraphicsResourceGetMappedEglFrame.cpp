#include <cudaEGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsResourceGetMappedEglFrame)(
CUeglFrame *, 
CUgraphicsResource, 
unsigned int, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsResourceGetMappedEglFrame(CUeglFrame *eglFrame, CUgraphicsResource resource, unsigned int index, unsigned int mipLevel) {
		fprintf(stderr, "cuGraphicsResourceGetMappedEglFrame()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsResourceGetMappedEglFrame)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsResourceGetMappedEglFrame = (CUresult (*)(
			CUeglFrame *, 
			CUgraphicsResource, 
			unsigned int, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphicsResourceGetMappedEglFrame");
			fprintf(stderr, "original_cuGraphicsResourceGetMappedEglFrame:%p\n", original_cuGraphicsResourceGetMappedEglFrame);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsResourceGetMappedEglFrame():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphicsResourceGetMappedEglFrame(
		eglFrame, 
		resource, 
		index, 
		mipLevel
		);
	}
}