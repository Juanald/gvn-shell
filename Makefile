lex_luther: lexical_analyser.o
	gcc lexical_analyser.o -o lex_luther

lexical_analyser.o: lexical_analyser.c
	gcc -c lexical_analyser.c

shell: shell.o
	gcc shell.o -o shell

shell.o: shell.c
	gcc -c shell.c

clean:
	del *.o lex_luther.exe shell.exe