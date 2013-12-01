OBJECT=yuv_player.o
CC=g++
CFLAGS= -g
TFLAGS=-s -fPIC -pipe -pthread -O3  
LDLIBS = -Wl,-Bsymbolic -lpthread 

All : clean test clean1

test : $(OBJECT)
	$(CC)  $(CFLAGS) -o yuv_player test.cpp $(OBJECT) -L../../m_stdio.h/ -lmstdio `sdl-config --cflags --libs` 

$(OBJECT) : %.o : %.cpp
	$(CC) -c  $(CFLAGS) $< -o $@   
                             
clean: 
	rm -rf test $(OBJECT) 
clean1:
	rm -rf $(OBJECT)
