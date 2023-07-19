#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
#include <string.h>

CUresult (*original_cuGetProcAddress)(
const char *, 
void **, 
int, 
cuuint64_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGetProcAddress(const char *symbol, void **pfn, int cudaVersion, cuuint64_t flags) {
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGetProcAddress)
		{
			//fetch the original function addr using dlsym
			original_cuGetProcAddress = (CUresult (*)(
			const char *, 
			void **, 
			int, 
			cuuint64_t)
			) dlsym(original_libcuda_handle, "cuGetProcAddress");
			fprintf(stderr, "original cuGetProcAddress:%p\n", original_cuGetProcAddress);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGetProcAddress():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult res =  original_cuGetProcAddress(
		symbol, 
		pfn, 
		cudaVersion, 
		flags
		);

		
		fprintf(stderr, "cuGetProcAddress() for symbol %s --- %p\n",symbol,*pfn);
		void* handle = dlopen("/home1/public/argyrosan/cuda_interception/intercept/libintercept.so",RTLD_NOW);
		__dlerror =dlerror();
		if(__dlerror){
			fprintf(stderr, "dlopen error for function cuGetProcAddress():%s\n", __dlerror);
			fflush(stderr);
		}
		*pfn = dlsym(handle,symbol);	
		__dlerror = dlerror();
		if(__dlerror){
			char symbol2[strlen(symbol)+4];
			for(int i=0;i<strlen(symbol);i++){
				symbol2[i] = symbol[i];
			}
			symbol2[strlen(symbol)] = '_';
			symbol2[strlen(symbol)+1] = 'v';
			symbol2[strlen(symbol)+2] = '2';
			symbol2[strlen(symbol)+3] = '\0';
			*pfn = dlsym(handle,symbol2);
			
			__dlerror = dlerror();
			if(__dlerror){
				fprintf(stderr, "dlopen error for function cuGetProcAddress():%s\n", __dlerror);
			}



			fprintf(stderr, "instead returning %s --- %p\n",symbol2,*pfn);
			return res;
		}
		fprintf(stderr, "instead returning %s --- %p\n",symbol,*pfn);
		return res;
	}
}