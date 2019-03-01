Chido Nguyen
cs372 Project 1 Read me:


1) Compile chat client with "g++ -o chatclient chatclient.cpp"
2) "chmod +x chatserve" Start the chat server with "./chatserve 12004" , feel free to change out the port number.
3) Start chat client with "./chatclient localhost 12004", feel free to change port number to match server. Leave localhost as is.
4)Follow on screen prompt for chat client first step should be to enter user handle.
5)Screen should say connection established.
6) Chatting should start from client and then chat sending/receiving will take turns between the client server:
		i.e. first client messages server
		now server messages client
		rinse and repeat.
7) Enter '\quit' without tick marks to end connection from either server or client
8) ctrl-c or sigint to kill server 