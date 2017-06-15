TARGETS = compressR_LOLS compressR_worker_LOLS compressT_LOLS

all: clean $(TARGETS)

$(TARGETS):
	gcc -Wall -g -pthread $@.c -o $@
	
clean:
	rm -f $(TARGETS)
