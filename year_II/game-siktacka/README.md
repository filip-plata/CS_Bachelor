## Simplified clon of Achtung Die Kurve

#### Project for university course

Instructions for people from my CV link. Running the game can take a few minutes and requires compiling from source, so beware.

In worst case scenario you would have to download 1GB of libraries and install cmake.

Instructions are for **Ubuntu** or **Debian**.

I have set up the game server on public IPv4 **52.29.24.148** on default port 12345.

GUI was not created by me, it was provided to us by university teacher, I included it for completness. GUI program is very simple(handles one client) and it is hard to play on one computer, but for getting the feel it is enough.

## Instructions to run

So, demo instructions for apt based LINUX distro. If you don't have one, you would have to check libraries installing for yourself. If you do not have libraries it will eat up a lot of disk space temporarily.

I provided convenience script **start.sh**. Script to run requires two binaries, gui3 and siktacka-client. **Build instructions are below and have to be done first**. Usage:

```start.sh port_number player_name```

Will start client instance for you. Make sure port_number is free. Example value is 12346. If you wish to play with yourself start two clients, eg.:
```
start.sh client 12346 "player_name1"
start.sh client 12347 "player_name2"
```

After clicking buttons in both clients, or using arrows, the game will be started!

After you have had enogh fun, you might consider deleting GTK+ 3.0 and boost development packages. You can do so with

```sudo apt-get remove  libgtk-3-dev libboost-all-dev```


## Instructions to build

You certainly need cmake and a C++ compiler.

First, compile the GUI:
```
cd project_root_dir
cd gui3
make
```

You need GTK+ 3.0 for this. You can install it with

```sudo apt-get install libgtk-3-dev```

or refer to [GTK install](https://www.gtk.org/download/)

Then compile game client. It communicates with server and provides simple interface for GUI.

```
cd project_root_dir
mkdir build
cd build; cmake ..; make
```

You need boost for this. You can install it with

```sudo apt-get install libboost-all-dev```

or refer to [boost install](http://www.boost.org/doc/libs/1_55_0/more/getting_started/)

Thats all! You can play now with yourself, or with anybody else on the internet.

### Trivia

Client and GUI communicate over TCP, so thats why a port number is needed.

Communication with server is done via UDP. Server can handle one game at the time, though it is unlikely you will run into any other person :) And even if, you can just play with some other unknown person!
