# CC = x86_64-w64-mingw32-g++.exe # 64 bit compiler

ifeq ($(OS),Windows_NT) # 32 bit compiler
	CC := i686-w64-mingw32-g++.exe
	FLAGS := -lws2_32 -Wall -g
else 
	CC := g++
	FLAGS := -Wall -g
endif

# FLAGS := -Wall
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

## MISSING DEPENDENCIES
client: client.cpp
	${CC} -std=c++${CXX_STANDARD} ${PATHS} ${FLAGS} $^ -o $@ 

server: server.cpp
	${CC} -std=c++${CXX_STANDARD} $^ -o $@ ${FLAGS}


## FULL COMPILATION
clientTCP:
	${CC} -std=c++${CXX_STANDARD} ./mSockets/TCP/mClientTCP.cpp ./mSockets/serializer.cpp ./mSockets/mClient.cpp client.cpp -o client.exe ${FLAGS}

serverTCP:
	${CC} -std=c++${CXX_STANDARD} ./mSockets/TCP/mServerTCP.cpp ./mSockets/serializer.cpp ./mSockets/mClient.cpp server.cpp -o server.exe ${FLAGS}



timeServer:
	gcc ./EXAMPLE/time_server.c -o timeServer.exe -lws2_32



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
