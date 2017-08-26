TARGETS = part1 part2 part3 part4

all: clean $(TARGETS)

$(TARGETS):
	gcc -Wall -g $@.c -o $@
	
clean:
	rm -f $(TARGETS)
