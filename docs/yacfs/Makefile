CFLAGS = -Wall -Wextra -O3 -march=native -flto -D_FILE_OFFSET_BITS=64 -I src -I/usr/include/fuse3
LDFLAGS = -lfuse3 -lzstd -lxxhash -lpthread -flto

OBJS = src/main.o src/ops.o src/pool.o src/metadata.o src/snapshot.o
TARGET = yacfs

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c src/yacfs.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
