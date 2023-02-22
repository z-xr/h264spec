hello : main.o H264ParseCommon.o H264ParseSPS.o 
	gcc -o hello main.o H264ParseSPS.o H264ParseCommon.o -lm 

main.o : main.c H264ParseSPS.h 
	gcc -c main.c H264ParseSPS.h H264ParseCommon.c

H264ParseSPS.o : H264ParseSPS.c H264ParseSPS.h
	gcc -c H264ParseSPS.c H264ParseSPS.h

H264ParseCommon.o : H264ParseCommon.c H264ParseCommon.h
	gcc -c H264ParseCommon.c H264ParseCommon.h

clean:
	rm -rf *.o \
	rm -rf *.gch \
	rm -rf hello