# MNIST Example with the PyTorch C++ Frontend

This folder contains an example of training a computer vision model to recognize
digits in images from the MNIST dataset, using the PyTorch C++ frontend.

The entire training code is contained in `mnist.cpp`.

To build the code, run the following commands from your terminal:

```shell
$ cd mnist
$ mkdir build
$ cd build
$ cmake -DCMAKE_PREFIX_PATH=/path/to/libtorch ..
$ make
```

where `/path/to/libtorch` should be the path to the unzipped _LibTorch_
distribution, which you can get from the [PyTorch
homepage](https://pytorch.org/get-started/locally/).

Execute the compiled binary to train the model:

```shell
$ ./mnist
Train Epoch: 1 [59584/60000] Loss: 0.4232
Test set: Average loss: 0.1989 | Accuracy: 0.940
Train Epoch: 2 [59584/60000] Loss: 0.1926
Test set: Average loss: 0.1338 | Accuracy: 0.959
Train Epoch: 3 [59584/60000] Loss: 0.1390
Test set: Average loss: 0.0997 | Accuracy: 0.969
Train Epoch: 4 [59584/60000] Loss: 0.1239
Test set: Average loss: 0.0875 | Accuracy: 0.972
...
```

--
If you get a linker error, replace contents of ./build/CMakeFiles/mnist.dir/link.txt with this:

/opt/rh/devtoolset-9/root/usr/bin/c++    -rdynamic CMakeFiles/mnist.dir/mnist.cpp.o  -o mnist  -Wl,--copy-dt-needed-entries -Wl,-rpath,/home1/public/argyrosan/temp/libtorch/lib:/usr/local/cuda-11.7/lib64 /home1/public/argyrosan/temp/libtorch/lib/libtorch.so /home1/public/argyrosan/temp/libtorch/lib/libc10.so /home1/public/argyrosan/temp/libtorch/lib/libkineto.a -lcuda /usr/local/cuda-11.7/lib64/libnvrtc.so /usr/local/cuda-11.7/lib64/libnvToolsExt.so /usr/local/cuda-11.7/lib64/libcudart.so /home1/public/argyrosan/temp/libtorch/lib/libc10_cuda.so -Wl,--no-as-needed,"/home1/public/argyrosan/temp/libtorch/lib/libtorch_cpu.so" -Wl,--as-needed -Wl,--no-as-needed,"/home1/public/argyrosan/temp/libtorch/lib/libtorch_cuda.so" -Wl,--as-needed /home1/public/argyrosan/temp/libtorch/lib/libc10_cuda.so /home1/public/argyrosan/temp/libtorch/lib/libc10.so /usr/local/cuda-11.7/lib64/libcufft.so /usr/local/cuda-11.7/lib64/libcurand.so /usr/local/cuda-11.7/lib64/libcublas.so /usr/local/cuda-11.7/lib64/libcublasLt.so -Wl,--no-as-needed,"/home1/public/argyrosan/temp/libtorch/lib/libtorch.so"  -Wl,--as-needed /usr/local/cuda-11.7/lib64/libnvToolsExt.so /usr/local/cuda-11.7/lib64/libcudart.so 

--
Once compiled run with:

LD_PRELOAD=../../../intercept/libintercept.so ./mnist  2> log.txt