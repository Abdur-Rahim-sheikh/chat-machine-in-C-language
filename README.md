# chat-machine-in-C-language with two interesting feature

## Introduction
This project implements the idea of <br>
  1) making a chat machine where client server can send massage to each other as many time as they want without waiting for other's to reply.
  2) server side(my_side) can save important massage if a keyword is passed to the server from the client at the begining of the massage
  3) Server side can detect if the client (friend_side) is really the client or any imposter using the friends ID</i>
### Description
## Chapter 1
  <b>chatting:</b> Both of the machine interact with each other using named pipe. Each of them has one pipe for writting massage. But we know that if we write anything in pipe next code will not run until it has been read. So I ran both server and client in multithreaded way. Where one thread deals with sending massage and another to recieve.
## Chapter 2
  <b>Saving important massage:</b> My server machine can save massage in a text file with the exact date and time. I followed the article to implement as my requirement [1]. I could implement this code in both side. But as one side is enough to prove the fact of the project so I did it in just one side. So if the client (friend) wants to send a massage which is important he thinks, he will type a keyword at the beginning of the text. This keyword is saved at server side and it will load each time the server is run. So they can change the keyword if they want. For example let the keyword is <em>important:</em> then if the client writes
  important: please come to my birthday party in 31 october.
  The server will automatically save in a text file for my case "important_msg.txt" file. as
  (Sun Sep 26 19:18:16 2021): please come to my birthday party in 31 october.
  note: the date will be the exact local time.
  For this feature, important massage will remain saved in a text file after the session is done.
## Chapter 3
  <b>Detect imposter:</b> This part is very tricky and fun. I implemented this feature with a statistics. As most of us talks in social site in bangla but writes the word in english. We all know people uses different alphabet to write same word word. Also in english we can depict our same statement in different words or sentence. But it is true that each people follows the same style almost everytime. for example, to know how my friend is (in bangllish) I write, "kire kemon achis,," but my friend rea writes "kire kmn achos?" so there is a change in kemon and kmn, and achos and achis. And people rarely changes there style of writting.
  So i implemented a hashmap (as there is no map library in C) ignoring the collision(very very rare possibility). And everytime we talk to each other, I save each of my friends word using hashcode. So if in any session my friends write 5 different word which he/she did not spoke ever. My program will suspect suspicius. And it will notify me. So that i can ask my friend to give secret key word. In my case it's saved in(secret_word.txt). If he/she fails to send the secrect keyword 5time. My machine will request to stop chatting with the imposter. This secret word can be changed in the secret_word.txt file, it load's the word everytime we run the session. To save each word from the text I had to split all word then fed to hashmap. This article [2] helped me to split word efficiently.
There is one confusion though when we just start talking all word are new word so it'll suspect me without reason. To overcome this problem I set a variable which detects if we talked enough to be old.The variable name is "min_word_to_be_old_friend" I set the value to zero for test purpose.  This variable is located in my_side directory in machine0.c file at line 11. You can change it as you deem fit.


# How to run this project
Before I proceed i would like to write some command to run this code, 
to run a simple c file, open a terminal where the file is located, then
compile: gcc file_name.c -o file_name
run: ./file_name
If the file contains thread we need an extra parameter just at the time of compiling, run command is same
compile: gcc file_name.c -o file_name -pthread
Now my project contains, 
1) my_side(directory)
    i) chat_machine0.c
    ii) important_msg.txt
    iii) magic_word.txt
    iv) secret_word.txt
    v) history_word_cnt.txt
2) friends_side (directory)
    i) chat_machine1.c
3) pipe_creator.c
4) reset_default.c

<strong>steps to run the project</strong>
1) First compile and run the reset_default.c file, it will prepare all the txt file as needed like fresh start. Every time you run it it'll reset every log you made in the project like, chat history,important massage, changed important file keyword, changed secret word. (Note: default secret word is "secret" without quote. And default magic word is "important:" without quote.
3) Then compile and run pipe_creator.c. It will create two named pipe at the same directory
4) After that open a terminal in my_side directory, then compile and run chat_machine0.c. (contains threading compile with -pthread)
5) Open another terminal in friends_side directory, then compile and run chat_machine1.c. (contains threading compile with -pthread) Also there is no order in step 4 or 5 you can run chat_machine1 before chat_machine0
6) After you finish chatting session enter "-999" (without quote) to end your side of session. Same command for both side.

*NOTE* 
1) If you want to save a massge from client side (friends_side or chat_machine1) you have to type the magic word as last update (default important:)at the beginning of the msg followed by a space. Then the server will save it with exact time.
2) If you want to prove that you are not imposter from client side you just have to send the secret word as last update (defalut secret)

## Appendix
[1] https://www.tutorialspoint.com/c_standard_library/c_function_localtime.htm <br>
[2] https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split#:~:text=To%20split%20a%20string%20we,words%20are%20separated%20by%20space.

