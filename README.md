# chat-machine-in-C-language with two interesting feature

## Introduction
This project implements the idea of <b>
  1) making a chat machine where client server can send massage to each other as many time as they want without waiting for other's to reply.
  2) server side(my_side) can save important massage if a keyword is passed to the server from the client at the begining of the massage
  3) Server side can detect if the client (friend_side) is really the client or any imposter using the friends ID</i>
## Description
## Chapter 1
  <b>chatting:</b> Both of the machine interact with each other using named pipe. Each of them has one pipe for writting massage. But we know that if we write anything in pipe next code will not run until it has been read. So I ran both server and client in multithreaded way. Where one thread deals with sending massage and another to recieve.<br>
  <b>Saving important massage:</b> My server machine can save massage in a text file with the exact date and time. I followed the article to implement as my requirement [1]. I could implement this code in both side. But as one side is enough to prove the fact of the project so I did it in just one side. So if the client (friend) wants to send a massage which is important he thinks, he will type a keyword at the beginning of the text. This keyword is saved at server side and it will load each time the server is run. So they can change the keyword if they want. For example let the keyword is <em>important:</em> then if the client writes
  important: please come to my birthday party in 31 october.
  The server will automatically save in a text file for my case "important_msg.txt" file. as
  (Sun Sep 26 19:18:16 2021): please come to my birthday party in 31 october.
  note: the date will be the exact local time.
  For this feature, important massage will remain saved in a text file after the session is done.
  <b>Detect imposter:</b> This part is very tricky and fun. I implemented this feature with a statistics. As most of us talks in social site in bangla but writes the word in english. We all know people uses different alphabet to write a single word. Also in english we can depict our same statement in different word or sentence. But it is true that each people follows the same style almost everytime. for example, to know how my friend is in bangla I write, "kire kemon achis,," but my friend rea writes "kire kmn achos?" so there is a change in kemon and kmn, and achos and achis. And people rarely changes there style of writting.





## Appendix
[1] https://www.tutorialspoint.com/c_standard_library/c_function_localtime.htm
