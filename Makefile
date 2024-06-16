READER_DIR = compress/bmp_reader
WRITER_DIR = decompress/bmp_writer
TREE_COMPRESS_DIR = compress/quad_tree_compress
TREE_DECOMPRESS_DIR = decompress/quad_tree_decompress

all: compress.exe decompress.exe


list/list.o: list/list.c list/list.h
	gcc -c list/list.c -o list/list.o

queue/queue.o: queue/queue.c queue/queue.h
	gcc -c queue/queue.c -o queue/queue.o

quad_tree/tree.o: quad_tree/tree.c quad_tree/tree.h
	gcc -c quad_tree/tree.c -o quad_tree/tree.o



compress.exe: compress/main.c $(READER_DIR)/reader.o $(TREE_COMPRESS_DIR)/compress.o list/list.o queue/queue.o quad_tree/tree.o
	gcc compress/main.c $(READER_DIR)/reader.o $(TREE_COMPRESS_DIR)/compress.o list/list.o queue/queue.o quad_tree/tree.o -o compress.exe

$(READER_DIR)/reader.o: $(READER_DIR)/reader.c $(READER_DIR)/reader.h
	gcc -c $(READER_DIR)/reader.c -o $(READER_DIR)/reader.o

$(TREE_COMPRESS_DIR)/compress.o: $(TREE_COMPRESS_DIR)/compress.c $(TREE_COMPRESS_DIR)/compress.h
	gcc -c $(TREE_COMPRESS_DIR)/compress.c -o $(TREE_COMPRESS_DIR)/compress.o



decompress.exe: decompress/main.c $(WRITER_DIR)/writer.o $(TREE_DECOMPRESS_DIR)/decompress.o list/list.o queue/queue.o quad_tree/tree.o
	gcc decompress/main.c $(WRITER_DIR)/writer.o $(TREE_DECOMPRESS_DIR)/decompress.o list/list.o queue/queue.o quad_tree/tree.o -o decompress.exe

$(WRITER_DIR)/writer.o: $(WRITER_DIR)/writer.c $(WRITER_DIR)/writer.h
	gcc -c $(WRITER_DIR)/writer.c -o $(WRITER_DIR)/writer.o

$(TREE_DECOMPRESS_DIR)/decompress.o: $(TREE_DECOMPRESS_DIR)/decompress.c $(TREE_DECOMPRESS_DIR)/decompress.h
	gcc -c $(TREE_DECOMPRESS_DIR)/decompress.c -o $(TREE_DECOMPRESS_DIR)/decompress.o



clean:
	del /S /Q *.o
	del /S /Q *.exe
	
compress_test:
	make
	./compress.exe test.bmp test.kon

decompress_test:
	make
	./decompress.exe test.kon test2.bmp

test:
	make
	./compress.exe test.bmp test.kon
	./decompress.exe test.kon test2.bmp