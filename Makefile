#Makefile

TARGET = pa1
WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla #-Werror #-03
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)

TESTFLAGS =  -DTEST_ALL -DTEST_REST

SRCS = main.c source/*  #this is where I put my source file

OBJS = $(SRCS:%.c=%.o)

VALGRIND = valgrind --tool=memcheck --verbose --leak-check=full --log-file=valgrind.log ./$(TARGET) examples/original/gophers outputs/gopher.count outputs/gopher.tree outputs/gopher.code outputs/gopher.hbt

VALGRIND2 = valgrind --tool=memcheck --verbose --leak-check=full --log-file=valgrind2.log ./$(TARGET) examples/original/woods outputs/woods.count outputs/woods.tree outputs/woods.code outputs/woods.hbt

$(TARGET): $(OBJS)
	$(GCC) $(TESTFLAGS) $(OBJS) -o $(TARGET)

testmemory: $(TARGET)
	$(VALGRIND)

testmem: $(TARGET)
	$(VALGRIND2)

clean:
		-rm $(TARGET)
		-rm *.log
		-rm *.o

test1: $(TARGET)
	./$(TARGET) examples/original/gophers outputs/count/gophers.count outputs/tree/gophers.tree outputs/code/gophers.code outputs/compressed/gophers.hbt
	diff outputs/count/gophers.count examples/count/gophers.count
	diff outputs/tree/gophers.tree examples/tree/gophers.tree
	diff outputs/code/gophers.code examples/code/gophers.code
	diff outputs/compressed/gophers.hbt examples/compressed/gophers.hbt

test2: $(TARGET)
	./$(TARGET) examples/original/lorum outputs/count/lorum.count outputs/tree/lorum.tree outputs/code/lorum.code outputs/compressed/lorum.hbt
	-diff outputs/count/lorum.count examples/count/lorum.count
	-diff outputs/tree/lorum.tree examples/tree/lorum.tree
	-diff outputs/code/lorum.code examples/code/lorum.code
	-diff outputs/compressed/lorum.hbt examples/compressed/lorum.hbt

test3: $(TARGET)
	./$(TARGET) examples/original/stone outputs/count/stone.count outputs/tree/stone.tree outputs/code/stone.code outputs/compressed/stone.hbt
	diff outputs/count/stone.count examples/count/stone.count
	diff outputs/tree/stone.tree examples/tree/stone.tree
	diff outputs/code/stone.code examples/code/stone.code
	diff outputs/compressed/stone.hbt examples/compressed/stone.hbt

test4: $(TARGET)
	./$(TARGET) examples/original/woods outputs/count/woods.count outputs/tree/woods.tree outputs/code/woods.code outputs/compressed/woods.hbt
	diff outputs/count/woods.count examples/count/woods.count
	diff outputs/tree/woods.tree examples/tree/woods.tree
	diff outputs/code/woods.code examples/code/woods.code
	diff outputs/compressed/woods.hbt examples/compressed/woods.hbt

test5: $(TARGET)
	./$(TARGET) examples/original/binary1 outputs/count/binary1.count outputs/tree/binary1.tree outputs/code/binary1.code outputs/compressed/binary1.hbt
	diff outputs/count/binary1.count examples/count/binary1.count
	diff outputs/tree/binary1.tree examples/tree/binary1.tree
	diff outputs/code/binary1.code examples/code/binary1.code
	diff outputs/compressed/binary1.hbt examples/compressed/binary1.hbt

test6: $(TARGET)
	./$(TARGET) examples/original/empty outputs/count/empty.count outputs/tree/empty.tree outputs/code/empty.code outputs/compressed/empty.hbt
	diff outputs/count/empty.count examples/count/empty.count
	diff outputs/tree/empty.tree examples/tree/empty.tree
	diff outputs/code/empty.code examples/code/empty.code
	diff outputs/compressed/empty.hbt examples/compressed/empty.hbt



.c.o:
	$(GCC) $(TESTFLAGS) -c $*.c
