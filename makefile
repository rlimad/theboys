# Makefile para The Boys (preserva conjunto.o)
CC       = gcc
CFLAGS   = -std=c99 -Wall -Wextra -Werror -g -Iincludes -Itads
RM       = rm -f

SRCDIR   = src
TADSDIR  = tads

# Fontes .c (sem conjunto.c)
SRCS = \
    $(SRCDIR)/entity.c     \
    $(SRCDIR)/event.c      \
    $(SRCDIR)/log.c        \
    $(SRCDIR)/simulation.c \
    $(SRCDIR)/utils.c      \
    $(TADSDIR)/fprio.c     \
    $(TADSDIR)/lista.c     \
    theboys.c

# Objetos gerados
OBJS = $(SRCS:.c=.o)

# Objeto pré-compilado (não será removido pelo clean)
DEPS_O = $(TADSDIR)/conjunto.o

# Executável final
TARGET = theboys

.PHONY: all clean

all: $(TARGET)

# Link final, inclui o conjunto.o
$(TARGET): $(OBJS) $(DEPS_O)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Compilação genérica de .c → .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean **NÃO** remove conjunto.o
clean:
	$(RM) $(OBJS) $(TARGET)
