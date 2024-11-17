
.PHONY: build
build:
	gcc -o build src/*.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O2 -D_DEFAULT_SOURCE -I/usr/local/lua/include -L/usr/local/lua -llua -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic -DPLATFORM_DESKTOP

.PHONY: run
run:
	make build
	./build
