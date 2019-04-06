# superchat

Superchat for SE 

(Both client and server are missing spellCheck cpp and h files need to add them!)

To compile you need to only change the

CPPFLAGS=-I/Users/shiba/Desktop/asio-1.12.2/include

line from the makefile, change the CPPFLAGS="..." to the directory where asio is located for you and go till include in asio so you can see the asio.hpp file.

# Client
The client folder is for client side files.
Go to the client folder and type make to compile all the files.

# Server
The server folder is for server side files.
Go to the server folder and type make to compile all the files.

# Basefiles
This contains all the .cpp and .h files except makefiles for both client and server.
There is no real need for this folder, except for backup purposes.
