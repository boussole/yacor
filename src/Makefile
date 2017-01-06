LIB=libyacor.a
OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
CXXFLAGS += -std=c++14 -Wall -Werror -g -pg
INC += -I../

all: static_lib

static_lib: $(OBJ)
	ar rv $(LIB) $(OBJ)

clean:
	rm -rf *.o
	rm -rf $(LIB)
	rm -rf *.out
	rm -rf *.gch
	rm -rf *.a

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(INC) -c $^

.PHONY: all clean static_lib