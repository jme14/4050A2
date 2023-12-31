##############################################################################
# Makefile for graph library
# CMP_SC 4050
# Fall 2023
# By Jim Ries (RiesJ@missouri.edu)
##############################################################################
CC = gcc
CPP = g++
CFLAGS = -Wall -Werror -c -g -std=c11 -I.
CPPFLAGS = -Wall -Werror -c -g -I.
LDFLAGS = -lm -lcs4050 -L.

default: test 

%.o : %.c 
	@echo Compiling $^ 
	@$(CC) $(CFLAGS) $^

test : main.o MSTc.o UtilC.o libcs4050.a
	@echo Linking $@ 
	@$(CC) $^ $(LDFLAGS) -o $@

clean : 
	@rm -rf *.o
	@rm -rf test
	@echo All Clean!
