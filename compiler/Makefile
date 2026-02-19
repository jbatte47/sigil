CC = gcc
LEX = flex
YACC = bison
CFLAGS = -Wall -g

SRCS = sigil.tab.c lex.yy.c ast.c nodes.c semantic.c runtime.c compiler.c
OBJS = $(SRCS:.c=.o)
TARGET = sigil_compiler

all: $(TARGET)

$(TARGET): sigil.tab.c lex.yy.c $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

sigil.tab.c sigil.tab.h: sigil.y
	$(YACC) -d sigil.y

lex.yy.c: sigil.l sigil.tab.h
	$(LEX) sigil.l

# Run the gold standard example
run: $(TARGET)
	./$(TARGET) ../examples/fireball_staff.sigil

clean:
	rm -f $(TARGET) *.o sigil.tab.c sigil.tab.h lex.yy.c
