/*Chido Nguyen
931506965
nguychid@oregonstate.edu
Project 1: Socket Programming Chat Serve/Client via Java and C++
This is the chat client which prompts user for a handle, handshakes the server, and then trade off chatting with the chat server.
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::cerr;

#define SECRET	"HANDSHAKE"
#define PRIVATE	"HIGHFIVE"

/*
Function gets user's choice of handle and appends > to use in client prompt
*/
void grabHandle(string &user){
			//loop to make sure chars < 10 count
	while(user.length() <= 0 || user.length() > 10){
		cout << "Please enter a user handle , 10 characters max: " << endl << std::flush;
		cin >> user;
	}
	user+= ">\0";
}

/*
Sets Up the first connection between client and server pre-handshake
*/
int talk_to_me(int &portNumber, int &socketFD, struct sockaddr_in &serverAddress, struct hostent* serverHostInfo){
	
	std::memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // clears
	serverAddress.sin_family = AF_INET; // network capable socketFD
	serverAddress.sin_port = htons(portNumber);// store port # for server
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char *)serverHostInfo->h_addr, serverHostInfo->h_length); // copy in address fro dns look up
	
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFD < 0){
		std::cerr << "Socket setup failed" <<endl;
		return 1;
	}
	
	//connect to server//
	if( connect ( socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr <<"Client: Error Connecting" << endl;
		return 1;
	}
	return 0;
	
}

/*
Establish a handshake with the server 
Sends its secret code , receives server code , if everything looks good sends OK
*/
int partners(int socket){
	char buffer[20] = {'\0'};
	int x = send(socket,SECRET, 9, 0);
	recv(socket, buffer, 8 , 0);
	if(string(buffer) != PRIVATE){			// MIGHT NEED REWORKING
		cout << buffer << endl;
		cerr << "bad handshake" << endl;
		return 1;
	}
	send(socket, "OK", 2,0);
	return 0;
}

/*
Get user input for message to be sent to server
*/
string message_out(string &handle, string &user_input, int socket){
	cout << handle << std::flush; // print out handle prompt
	getline(cin, user_input);		// get user input
	// converting strings to char array to  send //
	// https://www.geeksforgeeks.org/convert-string-char-array-cpp/
	string combined = handle + user_input;	// append handle + input
	int buff_size = combined.length();
	char arr[buff_size+1];
	strcpy(arr, combined.c_str());
	send(socket, arr, buff_size, 0);
	return combined;
}

/*
get the server message back
*/
int message_in(int &status,int socket){
	char arr2[1024] = {'\0'};
	char arr3[] = "\\quit";
	//http://www.cplusplus.com/reference/string/string/copy/
	char arr4[1024]={'\0'}; // copy our server message w/o the handle
	recv(socket, arr2, 1024, 0);
	int x = 10;
	int y = 0;
	while(arr2[x] != '\0'){ // hard coded chatserver handle this eliminates the handle portion to see if \quit is typed
		arr4[y] = arr2[x];
		x++;
		y++;
	}
	if(strcmp(arr4,arr3)== 0){
		cout << "Server has ended chat" << endl;
		return -1;
	}
	else{
		cout << arr2 << endl << std::flush;
		return 1;
	}
}



// command line for port # //
int main(int argc, char* argv[]){
	//			Get User Handle			//
	string handle;
	grabHandle(handle);
	/////////////////////////////////////
	
	//Setting Up The Socket//
	//Sources: old code from Brewster 344 Program 4//
	//http://beej.us/guide/bgnet/html/multi/ipstructsdata.html#structs//
	//https://www.geeksforgeeks.org/socket-programming-cc/ //
	
	int portNumber;
	int socketFD; //socket file descriptor
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	string user_input;
	portNumber = atoi(argv[2]);
	const char *tmp = argv[1];
	serverHostInfo = gethostbyname(tmp);
	//setup our socket //
	int con_stat = talk_to_me(portNumber,socketFD,serverAddress,serverHostInfo);
	if( con_stat == 0){
		cout << "connection established" << endl << std::flush;
	}
	else
		return 1;
	
	// Establish a handshake //
	int couple = 0;
	// Successful now to handle "chat" i.e. a loop of text //
	
	
	if(!couple){
			cin.ignore(100,'\n');
			int status = 1;
		while (status > 0){
			// message user wants to send //
			string storage = message_out(handle,user_input,socketFD);
			
			// http://www.cplusplus.com/reference/string/string/find/ //
			// find the string \quit if it follows the handle then we quit out of loop and close socket //
			string tapout = "\\quit"; 
			int pos_quit = storage.find(tapout);
			if(handle.length() == pos_quit){
				break;
			}
			// server returned message //
			status = message_in(status, socketFD);

		}
		close(socketFD);
	}
	
	return 0;
	
}