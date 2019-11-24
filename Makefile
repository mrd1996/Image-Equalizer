COMPILER ?= g++
FILE_DIR ?= src/AlgoTester.cpp src/ImageManager.cpp src/Counters.cpp src/ImageEqualizer.cpp
FLAGS    ?= -O3 -g -std=c++11

all: build test

build:
	$(COMPILER) $(FILE_DIR) $(FLAGS) -fopenmp -o bin/equalizer

buildS:
	$(COMPILER) $(FILE_DIR) $(FLAGS) -o bin/equalizerS

buildI:
	icpc -O3 -g $(FILE_DIR) -qopenmp -qop-report-phase=vec -qopt-report=2 -o bin/equalizerI

test:
	printf "threads,size,time(usecs)\n" > metrics/tests.csv; \
	for image in "img/cat.pgm" "img/cat2.pgm" "img/cat4.pgm" "img/cat16.pgm" "img/cat32.pgm" "img/cat64.pgm"; \
	do \
    	bin/equalizer $$image | tee -a metrics/tests.csv; \
	done

testSq:
	printf "threads,size,time(usecs)\n" > metrics/testseq.csv; \
	for image in "img/cat.pgm" "img/cat2.pgm" "img/cat4.pgm" "img/cat16.pgm" "img/cat32.pgm" "img/cat64.pgm"; \
	do \
    	bin/histogramS $$image 1 | tee -a metrics/testseq.csv; \
	done

noop:
	$(COMPILER) $(FILE_DIR) -o bin/histogram

clean:
	rm bin/*