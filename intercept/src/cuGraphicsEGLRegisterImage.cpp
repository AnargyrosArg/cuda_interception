#include <cudaEGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsEGLRegisterImage)(
CUgraphicsResource *, 
EGLImageKHR, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsEGLRegisterImage(CUgraphicsResource *pCudaResource, EGLImageKHR image, unsigned int flags) {
		fprintf(stderr, "cuGraphicsEGLRegisterImage()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsEGLRegisterImage)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsEGLRegisterImage = (CUresult (*)(
			CUgraphicsResource *, 
			EGLImageKHR, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphicsEGLRegisterImage");
			fprintf(stderr, "original_cuGraphicsEGLRegisterImage:%p\n", original_cuGraphicsEGLRegisterImage);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsEGLRegisterImage():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphicsEGLRegisterImage(
		pCudaResource, 
		image, 
		flags
		);
	}
}