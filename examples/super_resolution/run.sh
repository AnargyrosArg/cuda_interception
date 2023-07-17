#Train model
LD_PRELOAD=../../intercept/libintercept.so python main.py --cuda --upscale_factor 3 --batchSize 4 --testBatchSize 100 --nEpochs 5 --lr 0.001 2> log.txt

#Use model
LD_PRELOAD=../../intercept/libintercept.so python super_resolve.py --cuda --input_image dataset/BSDS300/images/test/16077.jpg --model model_epoch_4.pth --output_filename out.png 2> log2.txt

rm -rf model_epoch*.pth