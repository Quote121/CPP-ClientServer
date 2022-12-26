# CC = x86_64-w64-mingw32-g++.exe # 64 bit compiler
ifeq ($(OS),Windows_NT) # 32 bit compiler
	CC := i686-w64-mingw32-g++.exe
	# CC := x86_64-w64-mingw32-g++.exe
else
	CC := g++
endif

FLAGS := -Wall
##lboost_thread 
CXX_STANDARD := 20
BINARIES := client 

## FUCK THIS SHIT ILL WRITE MY OWN LIBRARY
#INCLUDE_PATHS specifies the additional include paths we'll need -IC:\mingw_dev_lib\include\SDL2
INCLUDE_PATHS =  -I"C:\msys64\mingw64\include"

#LIBRARY_PATHS specifies the additional library paths we'll need -LC:\mingw_dev_lib\lib
LIBRARY_PATHS =  -L"C:\msys64\mingw64\lib"

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window

# PATHS=$(LIBRARY_PATHS) $(INCLUDE_PATHS)


all: ${BINARIES}

client: client.cpp
	${CC} -std=c++${CXX_STANDARD} ${PATHS} ${FLAGS} $^ -o $@ 

server: server.cpp
	${CC} -std=c++${CXX_STANDARD} $^ -o $@ ${FLAGS}

clean:
	rm -rf *.o ${BINARIES} *.exe

analyze:

leakcheck:
ifeq ($(OS),Windows_NT)
	@echo Valgrind not on Windows_NT
else
	valgrind --leak-check=yes ./${target}
endif

debug: all
	gdb ${TARGET}
