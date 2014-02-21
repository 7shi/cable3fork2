TARGET = cable3
CFLAGS = -O2 -std=c99 -Wall -g
CXXFLAGS = -O2 -Wall -g
IFLAGS = -bap -br -ce -ci4 -cli0 -d0 -di0 -i8 -ip -l79 -nbc -ncdb \
		 -ndj -ei -nfc1 -nlp -npcs -psl -sc -sob

all: $(TARGET)

$(TARGET): cable3.o 8086run.o
	$(CXX) -o $@ $^

cable3.o: cable3.c
8086run.o: 8086run.cpp

clean:
	rm -f $(TARGET) $(TARGET).exe *.o

nbindent/nbindent:
	cd nbindent && $(MAKE)

indent: nbindent/nbindent
	./nbindent/nbindent cable3.c tmp.c $(IFLAGS) && mv tmp.c cable3.c
