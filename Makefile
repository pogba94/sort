sort.exe: sort.c
	gcc -W -Wall -g sort.c -o sort.exe

clean:
	rm sort.exe 

