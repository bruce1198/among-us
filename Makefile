CC = clang++
CFLAGS = -std=c++11 -O2
ALLEGRO_LIBRARIES = allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5
ALLEGRO_FLAGS = $(shell pkg-config --cflags --libs $(ALLEGRO_LIBRARIES)) -lallegro_main

OUT = main
SOURCE = main.cpp models/*.cpp
OBJ = *.o
all:
	$(CC) -c -g $(CFLAGS) $(SOURCE) $(ALLEGRO_FLAGS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ) $(ALLEGRO_FLAGS)
	rm $(OBJ)

.PHONY: clean
clean:
	rm $(OUT)

