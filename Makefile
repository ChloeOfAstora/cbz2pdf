OBJS	= cbz2pdf.o
SOURCE	= cbz2pdf.cpp
HEADER	=
OUT		= cbz2pdf
CC	 	= clang++-9
FLAGS	= -g -c -Wall -std=c++17 -O3
LFLAGS	= -lstdc++fs -lfltk
INCLUDES = -I/include/FL/

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

cbz2pdf.o: cbz2pdf.cpp
	$(CC) $(FLAGS) $(INCLUDES) cbz2pdf.cpp


clean:
	rm -f $(OBJS) $(OUT)