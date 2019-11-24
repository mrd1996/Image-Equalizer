COMPILER ?= g++
FILE_DIR ?= src/AlgoTester.cpp src/ImageManager.cpp src/Counters.cpp src/ImageEqualizer.cpp
FLAGS    ?= -O3 -g -std=c++11

all: build buildS

#Compiles project
build:
	$(COMPILER) $(FILE_DIR) $(FLAGS) -fopenmp -o bin/equalizer

#Compile sequencial project
buildS:
	$(COMPILER) $(FILE_DIR) $(FLAGS) -o bin/equalizerS

#Compiles with intel compiler
buildI:
	icpc -O3 -g $(FILE_DIR) -qopenmp -qop-report-phase=vec -qopt-report=2 -o bin/equalizerI

#Tests Parallel for generated images of area X and produces results to metrics/testGen.csv
testGen:
	printf "threads,size,time(usecs)\n" > metrics/testsGen.csv; \
	for image in 50 100 1000 10000 100000; \
	do \
    	bin/equalizer $$image -gen | tee -a metrics/testsGen.csv; \
	done

#Tests Sequential for generated images of area X and produces results to metrics/testGenSeq.csv
testGenSeq:
	printf "threads,size,time(usecs)\n" > metrics/testsGenSeq.csv; \
	for image in 50 100 1000 10000 100000; \
	do \
    	bin/equalizerS $$image 1 -gen | tee -a metrics/testsGenSeq.csv; \
	done

#Tests for images of different sizes
testImg:
	printf "threads,size,time(usecs)\n" > metrics/tests.csv; \
	for image in "img/cat.pgm" "img/cat2.pgm" "img/cat4.pgm" "img/cat16.pgm" "img/cat32.pgm" "img/cat64.pgm"; \
	do \
    	bin/equalizer $$image | tee -a metrics/tests.csv; \
	done

#Tests for images of different size sequentialy
testSq:
	printf "threads,size,time(usecs)\n" > metrics/testSeq.csv; \
	for image in "img/cat.pgm" "img/cat2.pgm" "img/cat4.pgm" "img/cat16.pgm" "img/cat32.pgm" "img/cat64.pgm"; \
	do \
    	bin/histogramS $$image 1 | tee -a metrics/testSeq.csv; \
	done

clean:
	rm bin/*