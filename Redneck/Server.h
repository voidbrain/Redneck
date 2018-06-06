#ifndef Server_h
#define Server_h

#include "Arduino.h"

class Server {
    public:
        Server(Stream &stream = Serial1);
        bool begin(Stream &stream = Serial1);
        void receiveWebRequests();
    private: 
        Stream *_stream;
};

#endif