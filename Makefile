
.PHONY: build
build:
	gcc -o build src/*.c src/editor/*.c include/*.c -Wall -O3 -D_DEFAULT_SOURCE -I/usr/local/lua/include -L/usr/local/lua  -llua -lraylib

.PHONY: dev
dev:
	gcc -o build src/*.c src/editor/*.c include/*.c -Wall -I/usr/local/lua/include -L/usr/local/lua -llua -lm -lraylib

.PHONY: run
run:
	make dev
	./build
