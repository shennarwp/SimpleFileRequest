# SimpleFileRequest
Simple server - client based file request testprogram, work only in localhost.
The server will search for file with names requested by client in the startfolder.
Default startfolder is the folder the server program currently located in, defined as "." in findfile.c
All the source file is located inside /src


for Server:
1. compile:
				`gcc -o server server.c findfile.c`

2. run:
				`./server [PORTNUMBER]`


for Client:
1. compile:
				`gcc -o client client.c`

2. run:
				`./client localhost [PORTNUMBER]`