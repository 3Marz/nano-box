
.PHONY: build
build:
	gcc -o build src/*.c src/editor/*.c include/*.c -Wall -std=c99 -Wno-missing-braces -Wunused-result -O1 -D_DEFAULT_SOURCE -I/usr/local/lua/include -L/usr/local/lua  -llua -lGLU -lGL -lX11

.PHONY: dev
dev:
	gcc -o build src/*.c src/editor/*.c include/*.c -Wall -Wunused-result -I/usr/local/lua/include -L/usr/local/lua -llua -lGLU -lGL -lX11 -lm

.PHONY: run
run:
	make dev
	./build
