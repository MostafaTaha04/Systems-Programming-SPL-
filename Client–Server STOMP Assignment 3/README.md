# Systems Programming Lab (SPL) – Assignment 3  
# Emergency Messaging Platform (STOMP 1.2) — Java Server + C++ Client (TCP)

## Student Information
Mostafa Taha – ID: 326524675

## Project Overview
This project implements Assignment 3 of the Systems Programming Lab (SPL) course.  
It builds an Emergency Service messaging system using a client–server architecture over TCP, implementing the STOMP 1.2 protocol to support publish–subscribe communication.

The system includes:
- A Java server that handles multiple concurrent clients, manages subscriptions, and routes messages by topic/channel.
- A C++ client that connects to the server, sends STOMP frames, receives real-time updates, and supports emergency event reporting and summaries.

## Main Goals of Assignment 3
- Implement STOMP 1.2 messaging over TCP
- Support multiple concurrent clients
- Provide topic/channel-based publish–subscribe
- Implement correct STOMP workflow using frames and receipts
- Support required client commands: login, join, exit, report, summary, logout
- Ensure correct connection lifecycle and resource cleanup

## STOMP Protocol Support (STOMP 1.2)

### Client → Server Frames
- CONNECT
- SUBSCRIBE
- UNSUBSCRIBE
- SEND
- DISCONNECT

### Server → Client Frames
- CONNECTED
- MESSAGE
- RECEIPT
- ERROR

Receipts are used to confirm operations such as subscribe/unsubscribe/logout when required.

## System Architecture

### Server (Java)
The server implements the server-side STOMP logic:
- User registration/authentication
- Active session management
- Topic subscription mapping
- Routing SEND messages to all subscribers as MESSAGE frames
- Proper cleanup of subscriptions and connections on disconnect

The server supports concurrent client connections and maintains thread-safe shared state for users, active connections, and topic subscriptions.

### Client (C++)
The client is a command-driven CLI application that communicates with the server using STOMP frames.

The client uses a two-thread design:
- Keyboard thread reads user commands and sends frames
- Socket thread continuously reads server frames and updates local state

The client is responsible for:
- Building and parsing STOMP frames
- Tracking subscription IDs and receipts
- Managing local storage of received events per channel/user
- Generating summary reports to output files

## Project Structure
```text
Client–Server STOMP Assignment 3/
├─ client/
│  ├─ include/
│  ├─ src/
│  ├─ data/        (example JSON files)
│  ├─ bin/         (output executable)
│  └─ makefile
└─ server/
   ├─ src/main/java/...
   └─ pom.xml


Build and Run Instructions
Prerequisites

Linux / Unix environment

Java 8+ and Maven

C++11 compatible compiler + make

Boost.System + pthread (client links with -lboost_system -lpthread)

Server (Java)

From the server/ directory:

mvn compile
mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="<port> tpc"
# or
mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="<port> reactor"


Example:

mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="7777 tpc"

Client (C++)

From the client/ directory:

make
./bin/StompEMIClient

Supported User Commands (Client)

login {host:port} {username} {password}

join {channel_name}

exit {channel_name}

report {json_file}

summary {channel_name} {user} {output_file}

logout

Command Behavior

login opens a TCP connection and sends CONNECT

join sends SUBSCRIBE to the requested channel

exit sends UNSUBSCRIBE for the channel

report loads JSON events and sends them as SEND messages to the channel

summary writes a formatted summary file for a given user/channel

logout sends DISCONNECT and closes the connection cleanly

JSON Event Reporting

The client/data/ folder in the template contains example JSON files (e.g., events1.json).
The report command loads a JSON file and publishes its events to the relevant channel.

Data Handling (Client)

The client stores received events organized by:

channel

user

The summary command exports aggregated information and events sorted by time as required by the assignment format.

Thread Safety & Cleanup

The client coordinates its threads so that:

The socket listener stops safely on logout/disconnect

No background thread remains after termination

The server ensures:

Correct removal of subscriptions on disconnect

Proper connection cleanup and consistent state updates

Notes

Server entry point: bgu.spl.net.impl.stomp.StompServer

Client executable: ./bin/StompEMIClient


And **delete** the `::contentReference...` line. After that, yes — it’s correct.
::contentReference[oaicite:0]{index=0}
