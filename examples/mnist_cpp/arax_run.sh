#!/bin/bash
echo "RUN Mnist"
cd build

#enable JIT
export CUDA_FORCE_PTX_JIT=1

export  LD_LIBRARY_PATH="/spare/anaconda3/envs/pytorch-dev/lib/":${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH="/home1/public/argyrosan/auto_talk/build/lib64/":${LD_LIBRARY_PATH}
#export LD_LIBRARY_PATH="/spare/manospavl/pytorch/build/lib":${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH="/home1/public/argyrosan/temp/libtorch/lib":${LD_LIBRARY_PATH}
echo ${LD_LIBRARY_PATH}
gdb ./mnist
