CFLAGS= -lm

mlrunner: csvParse.o knn.o mlrunner.o
	gcc -o $@ $^ ${CFLAGS}
%.o: %.c
	gcc -c $< -o $@

clean:
	rm -rf *.o mlrunner