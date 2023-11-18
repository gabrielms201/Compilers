CC = gcc
CFLAGS = -Wall -g
BINDIR = bin
SRCDIR = src

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(addprefix $(BINDIR)/, $(notdir $(SRCS:.c=.o)))
TARGET = $(BINDIR)/MiniLexicalAndSintaxRecognizer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
