default:  directorywalk.c xmlwrite.c buildindex.c
	gcc -o invertedindex directorywalk.c xmlwrite.c buildindex.c
debug:  directorywalk.c xmlwrite.c buildindex.c
	gcc -g -o invertedindex directorywalk.c xmlwrite.c buildindex.c
clean:
	$(RM) invertedindex *.o *~