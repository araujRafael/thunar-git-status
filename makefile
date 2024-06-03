CC = gcc
CFLAGS = -fPIC $(shell pkg-config --cflags thunarx-3) -I ./libgit2/include
LDFLAGS = $(shell pkg-config --libs thunarx-3) -L ./libgit2/build -lgit2

PLUGIN_NAME = myplugin.so
SRC = myplugin.c
OBJ = $(SRC:.c=.o)

all: $(PLUGIN_NAME)

$(PLUGIN_NAME): $(OBJ)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJ) $(PLUGIN_NAME)
