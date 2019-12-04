# Superchat

Superchat is a text messaging application built for desktop.


You can create chatrooms, Add people to chatrooms and collaborate activley through text messages.


Features Include but not limited to:

Login/Signup functionality,Spellcheck, history of previous messages, Room for 50 users and upto 10 chatrooms.

Take a look at the documentation for a mockup, Requirements, Design, and Testing reports.


# Documentation


1-[Requirements](https://github.com/nishad10/superchat/blob/master/Requirements-1552250188000.pdf)


>A list of all features with detailed description.


2-[Design](https://github.com/nishad10/superchat/blob/master/Design.pdf)


>Includes a mockup of how the app looks like.


3-[Testing](https://github.com/nishad10/superchat/blob/master/Testing%20Document.pdf)


>Testing and final report based on what we were able to implement properly.

# Compile/Build


To compile you need to only change the

CPPFLAGS=-I/Users/shiba/Desktop/asio-1.12.2/include

line from the makefile, change the CPPFLAGS="..." to the directory where asio is located for you and go till include in asio so you can see the asio.hpp file.

## Client
The client folder is for client side files.
Go to the client folder and type make to compile all the files.

## Server
The server folder is for server side files.
Go to the server folder and type make to compile all the files.

## Basefiles
This contains all the .cpp and .h files except makefiles for both client and server.
There is no real need for this folder, except for backup purposes.
