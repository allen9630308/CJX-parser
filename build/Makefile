CC := g++
FLAGS := -Wall
TARGET := ../bin/parser
LIBS := 
ROOT_DIR := ..
INCLUDE := ${ROOT_DIR}/include
OBJECTS := *.o
SOURCES := ${ROOT_DIR}/source/*.cpp

all:
	$(CC) $(FLAGS) -o $(TARGET) -I $(INCLUDE) $(LIBS) $(SOURCES)

clean:
	rm -rf $(TARGET) $(OBJECTS)
