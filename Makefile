all: 
	gcc main.c ./Include/External/*/*.c ./Include/Internal/*/*.c -o out -O -lGL -lglfw -ldl -lm

build:
	gcc main.c ./Include/External/*/*.c ./Include/Internal/*/*.c -o out -O -lGL -lglfw -ldl -lm

run:
	./out

clean:
	rm ./out

