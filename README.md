# Image Equalizer
A c++ with openMP implementation of an image equalizer

## Installing
```bash
make            #Compiles parallel and sequential versions
make build      #Compiles parralel
make buildS     #Compiles sequential
make buildI     #Compiles with intel compiler
make testGen    #Tests tool with generated images and saves metrics to metrics/testsGen.csv
make testGenSeq #Same as testGen but only sequential. saves metrics to metrics/testsGenSeq.csv
make testImg    #Same as testGen but for real images saves metrics to metrics/tests.csv
make testSeq    #Same as testGenSeq but for real images saves metrics to metrics/testsSeq.csv
```

## Usage
```bash
equalizer imagepath [thread ...] [-print] [-gen] [-it=<iterations>]
```
#### Equalize image 2 times for each thread
```bash
equalizer img/example.pgm -it=2 #-it default value is 20
```

#### Equalize image 2 times for thread 4 and 8 and save it
```bash
equalizer img/example.pgm 4 8 -it=2 -print #only writes last image equalized
```

#### Generate image with area 30 * 30 and equalize it 2 time using 10 threads and save
```bash
equalizer 30 10 -gen -it=2 -print
```

## Files
[ImageEqualizer.cpp](./src/ImageEqualizer.cpp) -  Implementation of image equalization algorithm

[AlgoTester.cpp](./src/AlgoTester.cpp) - File that contains our Main function

[ImageManager.cpp](./src/ImageManager.cpp) - Class that contains the IO functions

[Counters.cpp](./src/Counters.cpp) - Class that contains time measurement functions
