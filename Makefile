compile: bin bin/image.o bin/filter.o bin/main.o
	gcc bin/image.o bin/filter.o bin/main.o -lm -oblur

bin: 
	mkdir bin/

bin/image.o: src/image.c
	gcc -o bin/image.o -c src/image.c

bin/filter.o: src/filter.c
	gcc -o bin/filter.o -c src/filter.c

bin/main.o: src/main.c
	gcc -o bin/main.o -c src/main.c

clean_compile: compile
	rm -rf bin

exec: compile
	./blur

	
