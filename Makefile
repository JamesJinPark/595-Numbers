# this rule invokes the rule below
#
all: numbers

numbers: numbers.c
	clang -o numbers numbers.c -lpthread -g
	
clean: 
	rm -rf *.o
	
clobber: clean
	rm -rf numbers
	rm -rf *.out
