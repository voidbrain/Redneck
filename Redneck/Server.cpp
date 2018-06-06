#include "Server.h"
#ifdef Server_h

String readString, text;

Server::Server(Stream &stream) : _stream(&stream) {}

bool Server::begin(Stream &stream){
    _stream = &stream;
    Serial.println("server begin");
}

void Server::receiveWebRequests() {
    String text="";
    if (_stream->available())  {
        char c = _stream->read(); 
        if (c == ',') { text = readString; readString=""; } else { readString += c; }
        if(text!=""){ Serial.println(text); }
    }
}
#endif