CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = 1_pipe 2_pipeline 3_fifo_read 4_fifo_write

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
