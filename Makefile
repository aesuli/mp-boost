CC = g++

CFLAGS = -Wall -O4 -pedantic

PROGRAMS = boostTrain boostTest showEvaluation mergeEvaluation

all: libMPB.a $(PROGRAMS)

clean:
	rm -rf *.o *~ *.a core

# Library

CCOBJS = boostingmodel.o adaboostmhmodel.o mpboostmodel.o document.o documentset.o \
	prediction.o distribution.o adaboostmhhypothesis.o \
	predictionset.o example.o exampleset.o mpboosthypothesis.o \
	invertedlist.o evaluation.o

libMPB.a: $(CCOBJS)
	ar rvu $@ $(CCOBJS)
	ranlib $@

boostingmodel.o: boostingmodel.cpp \
	boostingmodel.h
	$(CC) $(CFLAGS) -c boostingmodel.cpp

mpboostmodel.o: mpboostmodel.cpp \
	mpboostmodel.h \
	defs.h \
	exampleset.h \
	example.h \
	distribution.h \
	document.h \
	predictionset.h \
	prediction.h \
	mpboosthypothesis.h \
	invertedlist.h \
	evaluation.h
	$(CC) $(CFLAGS) -c mpboostmodel.cpp

adaboostmhmodel.o: adaboostmhmodel.cpp \
	adaboostmhmodel.h \
	defs.h \
	exampleset.h \
	example.h \
	distribution.h \
	document.h \
	predictionset.h \
	prediction.h \
	adaboostmhhypothesis.h \
	invertedlist.h \
	evaluation.h
	$(CC) $(CFLAGS) -c adaboostmhmodel.cpp

distribution.o: distribution.cpp \
	distribution.h
	$(CC) $(CFLAGS) -c distribution.cpp

document.o: document.cpp \
	document.h
	$(CC) $(CFLAGS) -c document.cpp

documentset.o: documentset.cpp \
	documentset.h \
	document.h
	$(CC) $(CFLAGS) -c documentset.cpp

evaluation.o: evaluation.cpp \
	evaluation.h \
	defs.h
	$(CC) $(CFLAGS) -c evaluation.cpp

prediction.o: prediction.cpp \
	prediction.h
	$(CC) $(CFLAGS) -c prediction.cpp

predictionset.o: predictionset.cpp \
	predictionset.h \
	prediction.h
	$(CC) $(CFLAGS) -c predictionset.cpp

example.o: example.cpp \
	example.h \
	document.h
	$(CC) $(CFLAGS) -c example.cpp

exampleset.o: exampleset.cpp \
	exampleset.h \
	example.h \
	document.h
	$(CC) $(CFLAGS) -c exampleset.cpp

adaboostmhhypothesis.o: adaboostmhhypothesis.cpp \
	adaboostmhhypothesis.h
	$(CC) $(CFLAGS) -c adaboostmhhypothesis.cpp

mpboosthypothesis.o: mpboosthypothesis.cpp \
	mpboosthypothesis.h
	$(CC) $(CFLAGS) -c mpboosthypothesis.cpp

invertedlist.o: invertedlist.cpp \
	invertedlist.h \
	exampleset.h \
	example.h \
	document.h
	$(CC) $(CFLAGS) -c invertedlist.cpp

# PROGRAMS

boostTrain: boostTrain.cpp libMPB.a aarg.h
	$(CC) $(CFLAGS) -o $@ $@.cpp -lm -lMPB -L.

boostTest: boostTest.cpp libMPB.a aarg.h
	$(CC) $(CFLAGS) -o $@ $@.cpp -lm -lMPB -L.

mergeEvaluation: mergeEvaluation.cpp libMPB.a aarg.h
	$(CC) $(CFLAGS) -o $@ $@.cpp -lm -lMPB -L.

showEvaluation: showEvaluation.cpp libMPB.a aarg.h
	$(CC) $(CFLAGS) -o $@ $@.cpp -lm -lMPB -L.
