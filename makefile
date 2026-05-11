SOURCE_FILES = $(wildcard src/*.c)
OBJECT_FILES = $(SOURCE_FILES:src/%.c=obj/%.o)

CC = cc

TARGET = build/libcpers.so
COMPILE_FLAGS = -Iinclude -fPIC -lm
LINKER_FLAGS = -shared

TEST_BUILD_FLAGS = -Lbuild -l$(TARGET:build/lib%.so=%) $(COMPILE_FLAGS)

$(TARGET): $(OBJECT_FILES)
	mkdir -p build
	$(CC) -o $@ $^ $(LINKER_FLAGS)

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c -o $@ $^ $(COMPILE_FLAGS)

clean:
	rm -f $(TARGET) $(OBJECT_FILES) test

test: $(TARGET) test.c
	$(CC) -o $@ test.c $(TEST_BUILD_FLAGS)

run_test: test
	export LD_LIBRARY_PATH=$(CURDIR)/build && ./test
