all: 
	make build

build:
	gcc main.c ./Include/External/*/*.c ./Include/Internal/*/*.c -o out -O -lGL -lglfw -ldl -lm -lassimp

run:
	./out

clean:
	rm ./out

