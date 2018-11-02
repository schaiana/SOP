# TerminALL Main Makefile

include Makedefs

LIBDIR := lib

all: TerminALL.o
	(cd $(LIBDIR) && make all)
	g++ TerminALL.o $(LDFLAGS) -lTerminALL -o TerminALL

clean: FORCE
	(cd $(LIBDIR) && make clean)
	rm -f *.o TerminALL core*

FORCE: 
	
