#include "StompProtocol.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <c++/10/iomanip>
 
StompProtocol:: StompProtocol(): handler(nullptr), username(""), connected(false), idCounter(0), channelToId(), idToChannel(), channel_touser_toevent(), outputMap(), mtx(){}
 
int StompProtocol:: getIdCounter() {
    idCounter.fetch_add(1);
    return idCounter.load();
}
 
string StompProtocol::headerValue(string frame, string header) {
    vector<string> inputs = splitInput(frame, '\n');
    for (string line : inputs) {
        if (line.find(":")) {
            vector<string> keys = splitInput(line, ':');
            if (keys.size() < 2) {
                continue;
            }
            string key = keys.at(0);
            string value = keys.at(1);
            if (key == header) {
                return value;
            }
        }
        if (line.empty()) {
            break;
        }
    }
    return nullptr;
}    
 
void StompProtocol:: processCommands(string input) {
    vector<string> inputs = splitInput(input, ' ');
 
    if (inputs.at(0) != "login" && !connected.load())
        {
            cout << "please login first" << endl;
        }
        else if (inputs.at(0) == "login" && inputs.size() == 4)
        {
            if (connected.load())
            {
                cout << "The client is already logged in, log out before trying again" << endl;
            }
            else{
                vector<string> hostPort = splitInput(inputs.at(1), ':');
                mtx.lock();
                handler = new ConnectionHandler(hostPort.at(0), stoi(hostPort.at(1)));
                if(handler->connect()) {
                    connected.exchange(true);
                    username = inputs.at(2);
                    handler->sendFrameAscii("CONNECT\n" "accept-version:1.2\n" "host:stomp.cs.bgu.ac.il\n"  "login:" + inputs.at(2) + "\npasscode:" + inputs.at(3) + "\n\n\0", '\0');
                }else{
                    cout << "Cannot connect to " + inputs.at(1) + " please try to login again" << endl;
                    delete handler;
                    handler = nullptr;
                    cout << "Failed to login, try again" << endl;
                }
                mtx.unlock();
            }
        }
        else if (inputs.at(0) == "login" && inputs.size() != 4)
        {
            cout << "login coomand needs 3 args: {host:port} {username} {password}" << endl;
        }
        else if (inputs.at(0) == "join" && inputs.size() == 2)
        {
            mtx.lock();
            if (channelToId.find(inputs.at(1)) == channelToId.end()) {
                int joinId(getIdCounter());
                outputMap.insert({joinId, "Joined channel " + inputs.at(1)});      
                handler->sendFrameAscii("SUBSCRIBE\ndestination:" + inputs.at(1) + "\nid:" + to_string(joinId) + "\nreceipt:" + to_string(joinId) + "\n\n\0", '\0');
            }else {
                cout << "you are aleady Joined to channel " + inputs.at(1) << endl;
            }
            mtx.unlock();
        }
        else if (inputs.at(0) == "join" && inputs.size() != 2)
        {
            cout << "join coomand needs 1 args: {channel_name}" << endl;
        }
        else if (inputs.at(0) == "exit" && inputs.size() == 2)
        {
            mtx.lock();
            if(channelToId.find(inputs.at(1)) == channelToId.end()) {
                cout << "you are not subscribed to channel " + inputs.at(1) << endl;
            }else{
                int exitId(getIdCounter());
                outputMap.insert({exitId, "Exited channel " + inputs.at(1)});
                handler->sendFrameAscii("UNSUBSCRIBE\nid:" + to_string(channelToId.at(inputs.at(1))) + "\nreceipt:" + to_string(exitId) + "\n\n\0", '\0');
            }
            mtx.unlock();
        }
        else if (inputs.at(0) == "exit" && inputs.size() != 2)
        {
            cout << "exit coomand needs 1 args: {channel_name}" << endl;
        }
        else if (inputs.at(0) == "report" && inputs.size() == 2)
        {
            names_and_events  namesEvents = parseEventsFile(inputs.at(1));
            mtx.lock();
            channel_touser_toevent.insert({namesEvents.channel_name, {{username, namesEvents.events}}});
            if(channelToId.find(namesEvents.channel_name) == channelToId.end()) {
                cout << "You are not registered to channel " + namesEvents.channel_name << endl;
            }
            else{
                sort(namesEvents.events.begin(), namesEvents.events.end(), [](const Event& a, const Event& b) {
                    return a.get_name() < b.get_name();
                });
                sort(namesEvents.events.begin(), namesEvents.events.end(), [](const Event& a, const Event& b) {
                    return a.get_date_time() < b.get_date_time();
                });
                for(Event event: namesEvents.events){
                    handler->sendFrameAscii("SEND\ndestination:/" + event.get_channel_name() + "\n\nuser:" + username + "\ncity:" + event.get_city() + "\nevent name:" + event.get_name() + "\ndate time:" + to_string(event.get_date_time()) + "\ngeneral information:\n" "  active:" + event.get_general_information().at("active") + "\n" "  forces arrival at scene:" + event.get_general_information().at("forces_arrival_at_scene") + "\ndescription:\n" + event.get_description() + "\n\0" , '\0');
                }
                cout << "reported" << endl;
            }
            mtx.unlock();
        }
        else if (inputs.at(0) == "report" && inputs.size() != 2)
        {
            cout << "report coomand needs 1 args: {file}" << endl;
 
        }else if (inputs.at(0) == "summary" && inputs.size() == 4){

            ofstream out(inputs.at(3));
 
            if (!out.is_open()) {
                cout << "Failed to open the file" << endl;
                return;
            }
            mtx.lock();
            vector<Event> events;
            if(channel_touser_toevent.find("/" + inputs.at(1)) != channel_touser_toevent.end() && channel_touser_toevent.at("/" + inputs.at(1)).find(inputs.at(2)) != channel_touser_toevent.at("/" + inputs.at(1)).end()) {
                events = channel_touser_toevent.at("/" + inputs.at(1)).at(inputs.at(2));
            }
            mtx.unlock();
            int active(0);
            int forces_arrival_at_scene(0);
            for(Event event: events){
                if (event.get_general_information().at("  active") == "true"){
                    active++;
                }
                if(event.get_general_information().at("  forces arrival at scene") == "true"){
                    forces_arrival_at_scene++;
                }
            }
 
            out << "Channel " << inputs.at(1) << endl;
            out << "Stats: " << endl;
            out << "Total: " << events.size() << endl;
            out << "active: " << to_string(active) << endl;
            out << "forces arrival at scene: " << to_string(forces_arrival_at_scene) << endl;
            out << "Event Reports:" << endl << endl;
            atomic<int> counter(1);
            for(Event event: events) {
                out << "Report " << counter.load() << ":" << endl;
                out << "  city : " << event.get_city() << endl;
                out << "  date time : " << epoch_to_date(event.get_date_time()) << endl;
                out << "  event name : " << event.get_name() << endl;
                if (event.get_description().size() <= 27) {
                    out << "  summary : " << event.get_description() << endl << endl;
                }else{
                    out << "  summary : " << event.get_description().substr(0,27) << "..." << endl << endl;
                }
                counter.fetch_add(1);
            }
            out.close();
        }
        else if (inputs.at(0) == "summary" && inputs.size() != 4)
        {
            cout << "summary coomand needs 3 args: {channel_name} {user} {file}" << endl;
        }
        else if (inputs.at(0) == "logout" && inputs.size() == 1)
        {
            mtx.lock();
            int logoutId(getIdCounter());
            outputMap.insert({logoutId, "Logged out"});
            handler->sendFrameAscii("DISCONNECT\nreceipt:" + to_string(logoutId) + "\n\n\0", '\0');
            mtx.unlock();
        }
        else if (inputs.at(0) == "logout" && inputs.size() != 1)
        {
            cout << "logout coomand needs 0 args" << endl;
        }
        else
        {
            cout << "Illegal command, please try a different one" << endl;
        }
}
 
void StompProtocol::socketReader() {
 
    string answer;
    if(handler == nullptr || !connected.load() || !handler->getFrameAscii(answer, '\0')){
        return;
    }
    vector<string> inputs = splitInput(answer, '\n');
 
    if(inputs.at(0) == "CONNECTED")
    {
        cout << "Login successful" << endl;
    }
    else if(inputs.at(0) == "RECEIPT"){
        int receiptId = stoi(splitInput(inputs.at(1), ':').at(1));
        vector<string> receipts = splitInput(outputMap.at(receiptId), ' ');
        if(receipts.at(0) == "Joined"){
            mtx.lock();
            channelToId.insert({receipts.at(2), receiptId});
            idToChannel.insert({receiptId, receipts.at(2)});
            channel_touser_toevent.insert({receipts.at(2), {{username, vector<Event>()}}});
            cout << outputMap.at(receiptId) << endl;
            outputMap.erase(receiptId);
            mtx.unlock();
        }
        else if (receipts.at(0) == "Exited") {
            mtx.lock();
            channel_touser_toevent.at(receipts.at(2)).clear();
            channel_touser_toevent.erase(receipts.at(2));
            int chanelId = channelToId.at(receipts.at(2));
            channelToId.erase(receipts.at(2));
            idToChannel.erase(chanelId);
            cout << outputMap.at(receiptId) << endl;
            outputMap.erase(receiptId);
            mtx.unlock();
        }
        else if (receipts.at(0) == "Logged"){
            mtx.lock();
            cout << outputMap.at(receiptId) << endl;
            this->close();
            mtx.unlock();
        }
    }
    else if (inputs.at(0) == "MESSAGE") {
        string channel_name = headerValue(answer, "destination");
        string user_name = headerValue(answer, "user");
        string city = headerValue(answer, "city");
        string event_name = headerValue(answer, "event name");
        int date_time = stoi(headerValue(answer, "date time"));
        map<string, string> general_information = map<string, string>() ;
        general_information.insert({"  active", headerValue(answer, "  active")});
        general_information.insert({"  forces arrival at scene", headerValue(answer, "  forces arrival at scene")});
        string description = "";
        vector<string> splitedString = splitInput(answer,'\n');
        bool isDescriptionNext = false;
        for (string element: splitedString) {
            if (isDescriptionNext) {
                description = element;
                break;
            }
            if(element.substr(0, 11) == "description"){
                isDescriptionNext = true;
            }
        }
        Event event(channel_name, city, event_name, date_time, description, general_information);
       
        mtx.lock();
        if (channel_touser_toevent[channel_name][user_name].empty()) {
            channel_touser_toevent[channel_name][user_name] = std::vector<Event>();
        }
        channel_touser_toevent[channel_name][user_name].push_back(event);
 
        mtx.unlock();
    }
    else if (inputs.at(0) == "ERROR") {
        mtx.lock();
        cerr << "ERROR FROM THE SERVER:\n" << answer << endl;
        this->close();
        mtx.unlock();
    }
}
 
vector<string> splitInput(string input, char separator)
{
    vector<string> output = vector<string>();
    string s;
    stringstream stream(input);
    while (getline(stream, s, separator))
    {
        output.push_back(s);
    }
    return output;
}
 
void StompProtocol::close(){
    username = "";
    connected.exchange(false);
    idCounter.exchange(0);
    channelToId.clear();
    idToChannel.clear();
    channel_touser_toevent.clear();
    outputMap.clear();
    handler->close();
    delete handler;
    handler = nullptr;
}
 
string StompProtocol::epoch_to_date(int date_time)
{
    time_t time = date_time;
    struct tm tm_info;
 
    if (localtime_r(&time, &tm_info) == nullptr) {
        return "";
    }
 
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d/%m/%y %H:%M", &tm_info);
    return string(buffer);
}
 
StompProtocol::StompProtocol(StompProtocol &&other) :handler(other.handler), username(std::move(other.username)), connected(std::move(other.connected.load())),
                                                    idCounter(std::move(other.idCounter.load())),
                                                    channelToId(std::move(other.channelToId)),
                                                    idToChannel(std::move(other.idToChannel)),
                                                    channel_touser_toevent(std::move(other.channel_touser_toevent)),
                                                    outputMap(std::move(other.outputMap)),
                                                    mtx(){
    other.handler = nullptr;
    }
 
StompProtocol::~StompProtocol(){
    close();
}