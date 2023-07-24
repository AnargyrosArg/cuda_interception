#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsGLRegisterBuffer)(
CUgraphicsResource *, 
GLuint, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsGLRegisterBuffer(CUgraphicsResource *pCudaResource, GLuint buffer, unsigned int Flags) {
		fprintf(stderr, "cuGraphicsGLRegisterBuffer()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsGLRegisterBuffer)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsGLRegisterBuffer = (CUresult (*)(
			CUgraphicsResource *, 
			GLuint, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphicsGLRegisterBuffer");
			fprintf(stderr, "original_cuGraphicsGLRegisterBuffer:%p\n", original_cuGraphicsGLRegisterBuffer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsGLRegisterBuffer():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphicsGLRegisterBuffer(
		pCudaResource, 
		buffer, 
		Flags
		);
	}
}