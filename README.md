# Decentralized UDP Chat Application

![GitHub License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)

Welcome to the Decentralized UDP User Texting Application! This C++-based application created in QT Development Framework enables users to send and receive text messages to all active users on the network. It encompasses a basic peer-to-pper architecture, empowering users to interact in real-time.

## Features

- **Peer-to-Peer Architecture**: The application follows a peer-to-pper model, allowing all available users to chat with each other like in any group chat channel.

- **Text Messaging**: Logged-in users can send and receive text messages after discovery. Messages are relayed in real-time, facilitating interactive communication.

- **Signal-Slot pattern**: Application takes advantage of QT Framework Signal-Slot implementation for seamless object communication and smooth flow of action between threads

## How to Use

1. **Deploy**: Deploy using windeployqt.
   ```bash
   Open terminal
   <Path to windeployqt>windeployqt.exe --dir <Path to deployment directory> <Path to QT App .exe file>QTApp.exe
   Copy the QTApp.exe to Deployment directory
   ```
2. **Run**: Run the application with command line parametar
   ```bash
   ./QTApp.exe <username>
   ```
3. **Chat**: Chat with other users in the group
   ```bash
   Track status of other users
   Send messages
   ```
 4. **Quit**: End the session 
   ```bash
   quit (typing this the app will close and notify other users)
   ```
