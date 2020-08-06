CC = g++ -std=c++14
OBJS = simulator.o memory.o regfile.o error.o InstructionFetch.o InstructionDecode.o Execute.o MemoryAccess.o WriteBack.o

pipeline:${OBJS}
	${CC} -o pipeline ${OBJS}
error.o:error.cpp libraries.h
	${CC} -g -c error.cpp
regfile.o:regfile.cpp libraries.h
	${CC} -g -c regfile.cpp
memory.o:memory.cpp libraries.h
	${CC} -g -c memory.cpp
InstructionFetch.o:InstructionFetch.cpp libraries.h
	${CC} -g -c InstructionFetch.cpp
InstructionDecode.o:InstructionDecode.cpp libraries.h
	${CC} -g -c InstructionDecode.cpp
Execute.o:Execute.cpp libraries.h
	${CC} -g -c Execute.cpp
MemoryAccess.o:MemoryAccess.cpp libraries.h
	${CC} -g -c MemoryAccess.cpp
WriteBack.o:WriteBack.cpp libraries.h
	${CC} -g -c WriteBack.cpp
simulator.o:simulator.cpp libraries.h
	${CC} -g -c simulator.cpp

clean:
	rm -f pipeline ${OBJS}
clean_others:
	rm -f snapshot.rpt _snapshot.rpt error_dump.rpt _error_dump.rpt iimage.bin dimage.bin
clean_test:
	rm -f _snapshot.rpt _error_dump.rpt
golden:
	~/Archi2017_Project2/archiTA/simulator/pipeline
diff_snap:
	diff snapshot.rpt _snapshot.rpt -u
diff_error:
	diff error_dump.rpt _error_dump.rpt -u