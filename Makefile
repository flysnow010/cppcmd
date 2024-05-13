all:
	@make -C src
	@make -C test

clean:
	@make -C src clean
	@make -C test clean

run:
	@./bin/test

.PHNOY: all clean test
