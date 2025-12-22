all: 
	gcc main.c Include/glad/glad.c -o out -lGL -lglfw -ldl

build:
	gcc main.c Include/glad/glad.c -o out -lGL -lglfw -ldl

run:
	./out

clean:
	rm ./out

