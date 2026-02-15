#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include <vector>
#include "../include/StompProtocol.h"
using namespace std;
 
void socketReader(StompProtocol &protocol) {
    while (true){
        protocol.socketReader();
    }
}

void processCommands(StompProtocol &protocol){
    while (true){
        const short bufsize = 1024;
        char buf[bufsize];
        cin.getline(buf, bufsize);
        string input(buf);
        protocol.processCommands(input);
    }
}
 
int main(int argc, char *argv[]) {
    StompProtocol protocol = StompProtocol();
    thread socketThread(socketReader, ref(protocol));
    processCommands(protocol);
    socketThread.join();
}