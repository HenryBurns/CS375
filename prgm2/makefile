CC = g++
CC_FLAGS = -std=c++14 -Wall -pedantic -g -Wextra
 
# File names
EXEC1 =BestFirstSearch
SOURCES =$(wildcard *.cpp)
OBJECTS1 =priority_attempt.o 

ALL: $(OBJECTS1) 
	$(CC) $(OBJECTS1) -o $(EXEC1)
 
# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@
 
# Main target
 
# To remove generated files
clean:
	rm -f $(EXEC1) $(EXEC2) $(EXEC3) *.o
checkmem:
	valgrind ./$(EXEC)
