all: 
	gcc main.c ./Include/glad/glad.c ./Include/shader/shader.c ./Include/stb_image/stb_image.c -o out -O -lGL -lglfw -ldl -lm

build:
	gcc main.c ./Include/glad/glad.c ./Include/shader/shader.c ./Include/stb_image/stb_image.c -o out -O -lGL -lglfw -ldl -lm

run:
	./out

clean:
	rm ./out

