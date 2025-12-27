#ifndef STOMP_PROTOCOL_H
#define STOMP_PROTOCOL_H
 
#include "../include/ConnectionHandler.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <atomic>
#include "../include/event.h"
using namespace std;
 
class StompProtocol {
private:
    ConnectionHandler *handler;
    string username;
    atomic<bool> connected;
    atomic<int> idCounter;
    map<string, int> channelToId; 
    map<int, string> idToChannel;
    map<string, map<string, vector<Event>>> channel_touser_toevent;
    map<int, string> outputMap;
    mutex mtx;


    int getIdCounter();
    string headerValue(string frame, string headerKey);
    string epoch_to_date(int date_time);
    void close();
 
public:
    StompProtocol();
    void processCommands(string input);
    void socketReader();


    StompProtocol(const StompProtocol &other) = delete;
    StompProtocol& operator = (const StompProtocol &other) = delete;
    StompProtocol(StompProtocol &&other);
    StompProtocol& operator = (const StompProtocol &&other) = delete;
    ~StompProtocol();
};


vector<string> splitInput(string input, char separator);
 
#endif