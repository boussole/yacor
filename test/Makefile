CXXFLAGS := -I../src -std=c++14 -Wall -Werror -g -pg -fsanitize=address
LDFLAGS := -L../src -lyacor
FILES := $(wildcard *.cpp)

all: $(FILES)

clean:
	rm -rf $(basename $(FILES))

$(FILES):
	$(CXX) $(CXXFLAGS) $@ -o $(basename $@) $(LDFLAGS)
	
.PHONY: all $(FILES)