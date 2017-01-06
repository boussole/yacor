FILES := $(wildcard *.cpp)

all: $(FILES)

clean:
	rm -rf $(basename $(FILES))

$(FILES):
	$(CXX) $@ -o $(basename $@)
	
.PHONY: all $(FILES)
	