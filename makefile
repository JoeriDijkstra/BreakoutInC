##
# Software renderer
#
# @file
# @version 0.1

build:
	gcc -Wall -std=c99 ./src/*.c -o renderer

run:
	./renderer

clean:
	rm game

# end
