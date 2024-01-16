# PipeMan

### LeLana

Shwetlana Jha, Mary Lee

### Project Description:

Let's play hangman! A hangman game where a person can play solo or several people can play together! In multiplayer mode,a host can create a join code for their friends to use, and come up with a phrase for their friends to guess. When a player makes a guess, they must wait for the others to make a guess (if there are any other players) before they can make another guess. 

### Instructions:

How does the user install/compile/run the program.
(If not on a Linux, remove struct union semun code)
"make compile" to compile code
"./server" to start solo player games
"./client solo" to guess random phrases
"./client multi_create" to become a host and create a join code so friends can join, as well as a phrase to be guessed upon
"./client multi_guess" to join a game made by a friend (user's friend should send this code to them) and make guesses


How does the user interact with this program?
CTRL+C to end programs
If guessing on a multiplayer game, a guess consisting of one character will allow update the board 
(meaning user will see where that character appears in the phrase)
If the guess consists of more than one character, it is assumed user is guessing the entire phrase (from the first letter) 
and will update the board to show user up to where they are correct (so once one corresponding character does not match, update ends)