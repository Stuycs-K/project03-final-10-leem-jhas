# Final Project Proposal

## Group Members:

Shwetlana Jha, Mary Lee
       
# Intentions:

Multiplayer hangman : A hangman game where several people can play together! Once one player (client) makes a guess, they must wait for the others to make a guess (if there are any other players) before they can make another guess. Either one player will succeed or the max number of wrong answers will have been reached. 
    
# Intended usage:

User will enter "./client" to run their end
The server will run the game and include hangman and the correct/incorrect guesses that have been made by platers
Multiple clients will be able to connect to the game and make guesses in turns. Their guesses will show up on the interface of the server.
  
# Technical Details:

Technical Design - How you will be using the topics covered in class in the project.
    * Semaphores - Used to make clients take turns when making guesses
    * Shared memory - Used to keep track of the guesses made by all players and display the game
    * Signals - Used so clients can log out of the game
    * Allocating Memory -  Used to store client guesses and send messages between clients and server
    * Pipe - Used to create client-server connection

How you are breaking down the project and who is responsible for which parts. (very tentative distribution)
Shwetlana
    - Setting one word for hangman game (for now), get the server to display dashes (indicating length of word), and only give positive response when client sends back that exact word. The dashes will be stored in shared memory. 
    - If the client sends back exactly one char and that char is in the word, the server should replace corresponding dashes with that char. If char is a wrong guess, the server will display wrong answers on the side. The server and client communication will use sockets, while the wrong guess will be stored in shared memory.
    - Server should keep track of the number of wrong guesses, and end the game when the number exceeds max wrong guesses. A conditional loop could be used. 
Adding an array of words that the server can choose from randomly
Mary
    - Create client-server connection using a forking server
    - If the client sends back exactly one char and that char is in the word, the server should replace corresponding dashes with that char. If char is a wrong guess, the server will display wrong answers on the side. The server and client communication will use sockets, while the wrong guess will be stored in shared memory.
    - If the client sends back exactly one char, the server will check if that char is the word, and give an appropriate response. This will be communicated with using sockets
    - Server should prompt if you want to play again and will restart/end game accordingly

What algorithms and / data structures you will be using and how they will be used.
    char* - To store guesses made by clients and the hangman word
    char** - To store a list of guesses for the hangman word
    Sockets - To facilitate communication between server and clients
    Primitive data types as variables
    Handshake - To establish a connection between client and server
    
# Intended pacing:

A timeline with expected completion dates of parts of the project.
    1/4/24 - Create client-server connection
    1/5/24 - Add shared memory and sockets to facilitate simple communication
    1/8/24 - Allow multiple clients to communicate to server at the same time and incorporate semaphores
    1/9/24 - Add server communication to client for all cases of what a client has guessed
    1/10/24 - Update shared memory to hold information for that will be displayed on server and have server continuously update
    1/11/24 - Add the end game, max number of wrong guesses functionality to server
    1/11/24 - Add the play again/restart/end feature to client that will affect server
    1/15/24 - Add wordbank to server and any other additional features, as well as debugging