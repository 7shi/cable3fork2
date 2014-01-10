TARGET = indent
SRCS = args.c indent.c io.c lexi.c parse.c pr_comment.c
OBJS = $(SRCS:.c=.o)
PREFIX = /usr/local

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

install: $(TARGET)
	install -cs $< $(PREFIX)/bin

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJS)
