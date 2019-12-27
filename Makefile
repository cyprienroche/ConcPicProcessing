all: concurrent_picture_lib blur_opt_exprmt

concurrent_picture_lib: ConcMain.o Utils.o Picture.o PicProcess.o PicStore.o Transforms.o Index.o Commands.o CommandQueue.o
	gcc sod_118/sod.c ConcMain.o Utils.o Picture.o PicProcess.o PicStore.o Transforms.o Index.o Commands.o CommandQueue.o -I sod_118 -lm -lpthread -o concurrent_picture_lib

blur_opt_exprmt: BlurExprmt.o Utils.o Picture.o PicProcess.o List.o
	gcc sod_118/sod.c BlurExprmt.o Utils.o Picture.o PicProcess.o List.o -I sod_118 -lm -lpthread -o blur_opt_exprmt

Utils.o: Utils.h Utils.c

Picture.o: Utils.h Picture.h Picture.c

PicProcess.o: Utils.h Picture.h PicProcess.h PicProcess.c

PicStore.o: Utils.h Picture.h PicStore.h PicStore.c

ConcMain.o: ConcMain.c Utils.h Picture.h PicProcess.h PicStore.h Commands.h Transforms.h

BlurExprmt.o: BlurExprmt.c Utils.h Picture.h PicProcess.h

Transforms.o: Transforms.c Transforms.h

Index.o: Index.c Index.h

Commands.o: Commands.c Commands.h

CommandQueue.o: CommandQueue.c CommandQueue.h

List.o: List.c List.h

%.o: %.c
	gcc -c -I sod_118 -lm -lpthread $<

clean:
	rm -rf concurrent_picture_lib blur_opt_exprmt *.o

.PHONY: all clean

