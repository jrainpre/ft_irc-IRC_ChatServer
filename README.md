``bash
IIIIIIIIII  RRRRRRRRRRRRRRRRR            CCCCCCCCCCCCC
I::::::::I  R::::::::::::::::R        CCC::::::::::::C
I::::::::I  R::::::RRRRRR:::::R     CC:::::::::::::::C
II::::::II  RR:::::R     R:::::R   C:::::CCCCCCCC::::C
  I::::I      R::::R     R:::::R  C:::::C       CCCCCC
  I::::I      R::::R     R:::::R C:::::C
  I::::I      R::::RRRRRR:::::R  C:::::C
  I::::I      R:::::::::::::RR   C:::::C
  I::::I      R::::RRRRRR:::::R  C:::::C
  I::::I      R::::R     R:::::R C:::::C
  I::::I      R::::R     R:::::R C:::::C
  I::::I      R::::R     R:::::R  C:::::C       CCCCCC
II::::::II  RR:::::R     R:::::R   C:::::CCCCCCCC::::C
I::::::::I  R::::::R     R:::::R    CC:::::::::::::::C
I::::::::I  R::::::R     R:::::R      CCC::::::::::::C
IIIIIIIIII  RRRRRRRR     RRRRRRR         CCCCCCCCCCCCC
```
# FT_IRC - 42 Project
 
## Table of Contents
 
- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
 
## Introduction
 
FT_IRC is a collaborative project developed as part of the curriculum at 42, a coding school. This project aims to create a simple Internet Relay Chat (IRC) server implementation. IRC is a widely used protocol for real-time text-based communication, allowing users to participate in group discussions or private conversations over the internet.
 
## Features
 
FT_IRC comes with the following features:
 
- Multiple users can connect to the server simultaneously.
- Users can create or join different channels to participate in group conversations.
- Public messages sent to a channel will be visible to all its members.
- Users can send private messages to other users connected to the same server.
- Moderators can kick or ban users from a channel.
- Built-in admin commands for server management.
- Password-protected channels for private discussions.
- Nickname management: users can change their nicknames while connected.
 
## Requirements
 
To run FT_IRC, you need the following prerequisites:
 
- C compiler (e.g., GCC)
- Unix-based operating system (Linux, macOS, or similar)
- A terminal emulator
 
## Installation
 
Clone the FT_IRC repository from GitHub:
 
```bash
git clone https://github.com/JakobKlocker/IRC_Server.git
```
 
Navigate to the project directory:
```bash
cd ft_irc
```
Compile the server executable:
```bash
make
```
 
## Usage
Start the server:
```bash
./ircserv [PORT] [PASSWORD]
```
 
Now you can connect to the server with a Client of your choice.
 
Colaboration with https://github.com/jakobklocker
