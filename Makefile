SUBDIRS := src test research#$(wildcard */.)

all: $(SUBDIRS)

clean:
	$(foreach dir,$(SUBDIRS),make -C $(dir) clean && ) true

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all clean $(SUBDIRS)
