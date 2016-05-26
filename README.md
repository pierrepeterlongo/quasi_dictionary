# quasi_dictionnary

Provides functions for indexing a set of key-values elements. The query has false positives which rate is below 1/2^fingerprint_size. 
A false positive is an element not indexed for which the structures provides a value.
Note that values provided for indexed elements are correct.  

The quasi_dictionnay uses the BooPHM Minimal perfect hash function for associating a key to a value. A fingerprint value is automatically added to each indexed element for probabilistic set representation.

# Getting the latest source code

## Requirements

CMake 2.6+; see http://www.cmake.org/cmake/resources/software.html

c++ compiler; compilation was tested with gcc and g++ version>=4.5 (Linux) and clang version>=4.1 (Mac OSX).

## Instructions


    # get a local copy of source code
    git clone --recursive https://github.com/pierrepeterlongo/quasi_dictionary.git
    
    # compile the code an run a simple test on your computer
    cd quasi_dictionary
    make
    # runs a quasi-dictionary instance with 1 million elements and a finger print of size 12
    ./qd_test 1000000 12 
    
    
# Simple example and usage
The src directory contains a file named **qd_tester.cpp** that provides a full quasi-dicitonary usage example

