# Dev Log:

## Player 1: Shwetlana

### 2024-01-02 - Brief description
Expanded description including how much time was spent on task.

### 2024-01-03 - Brief description
Expanded description including how much time was spent on task.

### 2024-01-04 - Client guesses
- Added files to keep track of current state of the guessed word
- Added function to check if a guess made by a client is correct and to modify the current state
(only works for one pass)
- more than 1 hr was spent on task 

### 2024-01-05 - Client guesses
- Added server.c setting up initial dashes
- Added read and write to puth client and server
- issues with reading/writing - not done
- more than ~1hr 30min was spent on task 

### 2024-01-06 - Client guesses
- fixed reading/writing to files
- stores client guesses
- ~50 min spent in total 

### 2024-01-08 - Shared memory and dashes
- Added shared memory to keep track of rounds
- tried to get dashes to work correctly - unsuccessful 
- ~1hr 30min spent in total

### 2024-01-09 - Correct guess
- Added victory after client guesses word correctly
- tried to get dashes to work correctly - unsuccessful 
- ~1hr spent in total

### 2024-01-10 - Shared memory and victory 
- both server and client indicate when you have won
- tried to get server to exit after client guessed - kind of unsuccessful 
- shared memory indicates if there has been a win
- client exits after winning - kind of successful?
- ~2hr spent in total

### 2024-01-11 -  Codewords to file
- made funciton to add all possible codewords to a file
- ~45 min spent in total

### 2024-01-12 - Codewords from file
- started playing with files to see if I could extract a codeword
- ~45 min spent in total

### 2024-01-14 - Sharedmemory and files for codeword
- got a random codeword from file
- added shared memory for codeword
- fixed victory ending
- ~2hrs spent in total

### 2024-01-15 - Sharedmemory and files for codeword
- client can add to wordbank
- ~45 spent in total

## Player 2: Mary

### 2024-01-02 - Brief description
Expanded description including how much time was spent on task.

### 2024-01-03 - Brief description
Expanded description including how much time was spent on task.

### 2024-01-04 - Client-Server and code word
- File setup for client-server connection
- Have server send code word to client
- Created function that replaces code word with dashes
- ~1hrs spent in total

### 2024-01-05 - Dashes
- Tried to get dashes to work consistently and correctly - unfinished
- ~2hrs spent in total

### 2024-01-08 - Dashes and client guesses
- Dashes appear on both client and server without extra characters
- modified check_guess() so server now accepts client guesses with length greater than 1
- ~2hrs spent in total

### 2024-01-09 - String issues on lab machines
- Attempted to resolve string issues/seg faults that appeared on lab machine
- ~1hr spent in total

### 2024-01-10 - String issues on lab machines
- finally fixed char* process() function to work on lab machine
- ~1hr spent in total

### 2024-01-11 - Dashes and client guesses
- Started working on allowing multiple clients using pipes
- ~50 min spent in total

### 2024-01-12/13 - sick

### 2024-01-14 - Dashes and client guesses
- Attempted to create multiplayer mode using pipes
- Transitioned into using semaphores instead
- ~3hrs spent in total

### 2024-01-15 - Dashes and client guesses
- Created "solo" and "multi" modes on client ("multi" uses semaphores)
- Looped "solo" mode so client doesn't have to repeatedly run commands
- Client (host) can create a game room and choose a word/phrase (to be guessed)
- Client (player) can join a game room using a join code, and make guesses
    - current state includes the guesses of other clients (players)
- Have program send victory message and delete shared memory and files when the phrase
  is correctly guessed
- ~14hrs spent in total
