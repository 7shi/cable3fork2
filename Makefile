TARGET = cable3
CFLAGS = -O2 -std=c99
IFLAGS = -bap -br -ce -ci4 -cli0 -d0 -di0 -i8 -ip -l79 -nbc -ncdb \
		 -ndj -ei -nfc1 -nlp -npcs -psl -sc -sob
SRCS = cable3.c tables.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(TARGET).exe

nbindent/nbindent:
	cd nbindent && $(MAKE)

indent: nbindent/nbindent
	./nbindent/nbindent cable3.c tmp.c $(IFLAGS) && mv tmp.c cable3.c
