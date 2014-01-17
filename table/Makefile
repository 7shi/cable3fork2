TARGET = $(TSVS) $(CS)
SRCS1  = table.fsx table-addr.fsx table-index.fsx table-minmax.fsx table-8.fsx
SRCS2  = table-c.fsx
SRCS   = $(SRCS1) $(SRCS2)
EXES   = $(addprefix $(ODIR)/,$(SRCS:.fsx=.exe)
TSVS   = $(SRCS1:.fsx=.tsv)
CS     = $(SRCS2:.fsx=.c)
ODIR   = bin/Debug
FSC    = fsharpc --nologo -g

all: $(ODIR) $(TARGET)

$(ODIR):
	mkdir -p $(ODIR)

%.tsv %.c: $(ODIR)/%.exe
	cd $(ODIR) && ./`basename $<` > ../../$@

$(ODIR)/%.exe: %.fsx
	$(FSC) -o $@ $<

clean:
	rm -rf bin $(TARGET)
