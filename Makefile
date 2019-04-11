#####################################
# @author: Abdullah Emad 			#
# Makefile for The binary tree test #
#####################################

# compiler to use
CC = g++

# flags to pass compiler
CFLAGS = -ggdb3 -O0 -Wall -Werror

# name for executable
EXE = bin/ndnpi

# space-separated list of header files
HDRS = include/cranbtree.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS = -lcunit -lndn-cxx -lboost_system

# space-separated list of source files
SRCS = src/datastructures/linkedlist.c src/datastructures/hashtable.c src/modules/* src/data/*


# Included folders
INCLUDES = include


# automatically generated list of object files
OBJS = $(SRCS:.c=.o)



###########################################
#   Compile object files into a library   #
###########################################

# default target make
default: mkbin
	$(CC) $(CFLAGS) $(SRCS) src/ndnpi.cpp $(LIBS) -o bin/ndnpi -I $(INCLUDES)
	
# creates the object file
compile:
	$(CC) $(CFLAGS) -c $(SRCS) -I $(INCLUDES)

#creates the library
createlib: 
	ar rc libndnpi.a ndnpi.o

#create index on the library
indexlib: 
	ranlib libndnpi.a

# dependencies 
$(OBJS): $(HDRS) Makefile



###################################
# Install in the correct locations#
###################################
install: 
	mkdir -p /usr/local/include/cranberry && \
	cp $(HDRS) /usr/local/include/cranberry/cranbtree.h && \
	cp libcranbtree.a /usr/local/lib/libcranbtree.a

uninstall: 
	rm -f /usr/local/include/cranberry/cranbtree.h && \
	rm -f /usr/local/lib/libcranbtree.a && \
	rmdir --ignore-fail-on-non-empty /usr/local/include/cranberry

###############################
#		Test and cli		  #
#	   for developers		  #
###############################


#Creates binary for the test in /bin
test: mkbin
	$(CC) $(CFLAGS) $(SRCS) test/test.c $(LIBS) -o bin/test -I $(INCLUDES)


#creates the bin directory if it does not exist
mkbin:
	mkdir -p bin

#Fix any code styling issue
beautiful: 
	indent -bad -bap -nbc -bbo -hnl -brs -c33 -cd33 -ncdb -ci4 -cli0 -d0 -bls -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1 -bli0 src/*.c include/*.h test/*.c && rm -rf include/*~* include/datastructures/*~* src/*~* src/datastructures/*~* test/*~*

# housekeeping
clean:
	rm -f core *.o *.a bin/cli bin/test

.PHONY: test cli

