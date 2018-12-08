OBJS = \
 main.o \
 match.o \
 ordenaExterna.o \
 split.o \
 ordenaInterna.o \
 merge.o \
 listaDados.o \
 geraDados.o \
 compares.o

proj.exe: $(OBJS)
	gcc -o $@ $^ -lm

%.o : %.c
	gcc -c -o $@ -c $<
	
clean:
	rm $(OBJS) proj.exe