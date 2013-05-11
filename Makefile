all:  main.cpp raytrace.cpp
	g++ main.cpp raytrace.cpp -o raytrace -lpthread -lrt

clean:
	rm -f *~
