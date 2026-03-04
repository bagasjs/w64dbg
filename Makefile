CC := clang
CFLAGS := -Wall -Wextra -D_CRT_SECURE_NO_WARNINGS -g
LFLAGS := -ldbghelp

w64dbg.exe: w64dbg.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
