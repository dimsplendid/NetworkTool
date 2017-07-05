# CC and CFLAGS are varilables
CC = g++ -g -Wall
CFLAGS = -c
AR = ar
ARFLAGS = rcv

# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
OPTFLAGS = -O3
DBGFLAGS = -g -D_DEBUG_ON_
OBJECTS = main.o Graph.o aux.o generator.o bruteforce.o tool.o tree.o
OBJECTS_PATH = main.o Graph.o aux.o generator.o bruteforce.o tool_path.o tree.o

all	: bin/MinCut
	@echo -n ""

dbg : bin/min_cut_dbg

bin/MinCut_path	: $(OBJECTS_PATH) lib
			$(CC) $(OPTFLAGS) $(OBJECTS_PATH) -ltm_usage -Llib -o $@
bin/MinCut	: $(OBJECTS) lib
			$(CC) $(OPTFLAGS) $(OBJECTS) -ltm_usage -Llib -o $@
main.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
Graph.o 		: src/Graph.cpp src/Graph.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
aux.o		: src/aux.cpp
			$(CC) $(CFLAGS) $< -o $@
generator.o	: src/generator.cpp src/generator.h
			$(CC) $(CFLAGS) $< -o $@
bruteforce.o: src/bruteforce.cpp
			$(CC) $(CFLAGS) $< -o $@
tool.o: src/tool.cpp src/tool.h
			$(CC) $(CFLAGS) $< -o $@
tool_path.o: src/tool.cpp src/tool.h
			$(CC) $(CFLAGS) -DPATH $< -o $@
tree.o: src/ST_tree.cpp src/ST_tree.h
			$(CC) $(CFLAGS) $< -o $@

# debug option
#bin/min_cut_dbg	: Graph_dbg.o main_dbg.o Min_cut_dbg.o
#			$(CC) $(OPTFLAGS) $(DBGFLAGS) Graph_dbg.o Min_cut_dbg.o main_dbg.o -lgsl -lgslcblas -lm -o $@
#main_dbg.o 	   	: src/main.cpp
#			$(CC) $(DBGFLAGS) $(CFLAGS) $< -o $@
#Graph_dbg.o 		: src/Graph.cpp src/Graph.h
#			$(CC) $(DBGFLAGS) $(CFLAGS) $(OPTFLAGS) $< -o $@
#Min_cut_dbg.o 		: src/Min_cut.cpp src/Min_cut.h
#			$(CC) $(DBGFLAGS) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o and executable files
lib: lib/libtm_usage.a

lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $<

clean:
		rm -rf *.o lib/*.a lib/*.o bin/*
