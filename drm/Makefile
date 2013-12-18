INCDIR = -I.
DBG    = -g
OPT    = -O3
CPP    = g++
CFLAGS = $(DBG) $(OPT) $(INCDIR)
LINK   = -lm 

.cpp.o:
	$(CPP) $(CFLAGS) -c $< -o $@

all: drm

drm: main.cpp Segmentation.cpp CPoint.cpp gb/segment.cpp
	$(CPP) $(CFLAGS) -o drm main.cpp Segmentation.cpp CPoint.cpp gb/segment.cpp $(LINK) 

clean:
	/bin/rm -f gb *.o

clean-all: clean
	/bin/rm -f *~ 
