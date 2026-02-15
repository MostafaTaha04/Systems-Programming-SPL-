#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>

class Event
{
private:

    std::string channel_name;

    std::string city;

    std::string name;

    int date_time;

    std::string description;

    std::map<std::string, std::string> general_information;
    std::string eventOwnerUser;

public:
    Event(std::string channel_name, std::string city, std::string name, int date_time, std::string description, std::map<std::string, std::string> general_information);
    Event(const std::string & frame_body);
    virtual ~Event();
    void setEventOwnerUser(std::string setEventOwnerUser);
    const std::string &getEventOwnerUser() const;
    const std::string &get_channel_name() const;
    const std::string &get_city() const;
    const std::string &get_description() const;
    const std::string &get_name() const;
    int get_date_time() const;
    const std::map<std::string, std::string> &get_general_information() const;
};


struct names_and_events {
    std::string channel_name;
    std::vector<Event> events;
};


names_and_events parseEventsFile(std::string json_path);
void split_str(std::string line, char character, std::vector<std::string> &lineArgs);
