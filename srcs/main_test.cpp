/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:41:58 by marvin            #+#    #+#             */
/*   Updated: 2024/12/09 15:41:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h> //socket functions 
#include <netinet/in.h> //data type sockaddr
#include <iostream>
#include <errno.h>
#include <cstring>
#include <unistd.h> // to close fds
#include <arpa/inet.h> //function inet_ntoa

#define MAX_CONNEXIONS 5
#define BUFFER_MAX 1048

int   main(void)
{
  //1 - Opening a server socket of type stream (= TCP) using IPv4 protocol
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  //Checking if the socket has been successfully created
  if (serverSocket < 0)
  {
    std::cerr << "Error creating the socket" << std::endl;
    return (1);
  }
  std::cout << "Socket serverSocket has successfully been created " << std::endl;
  //2 - Defining server address
      //sin_family = defining type (IPv4)
      //sin_port = converting the port to make it compatible w/ the IP protocol
      //sin_addr = INADDR_ANY . allowing the server to accept connexions from any host IP addresses
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  //3 - Binding the server socket to a port and an address, as defined in sockaddr_in + checking the return value of bind
  if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "bind: " <<strerror(errno) << std::endl;
        close(serverSocket);
        return (1);
    }
    std::cout << "Socket serverSocket is bound to the port " << ntohs(serverAddress.sin_port) << std::endl;
    if (listen(serverSocket, MAX_CONNEXIONS) == -1)
    {
        std::cerr << "listen: " << strerror(errno) << std::endl;
        close(serverSocket);
        return (1);
    }
    std::cout << "Socket serverSocket is now listening to connexions" << std::endl;
    //4 - Accepting connexions
    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
    if (clientSocket == -1)
    {
      std::cerr << "accept: " << strerror(errno) << std::endl;
      close(serverSocket);
      return (1);
    }
    std::cout << "A new connexion has been established, clientSocket created , address from client = " << inet_ntoa(clientAddress.sin_addr) << " on port: " << ntohs(clientAddress.sin_port) << std::endl;
    //5 - Receiving data from the client (in a loop to allow discussions)
    char buffer[BUFFER_MAX];
    while (1)
    {
      memset(buffer, 0, BUFFER_MAX);
      ssize_t bytes_received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0); //Taking into account the null termination
      if (bytes_received <= 0)
      {
        std::cerr << "recv: " << strerror(errno) << std::endl;
        break;
      }
      //Null terminate the string, in case we have a nl or not 
      // if (buffer[bytes_received] == '\n')
        buffer[bytes_received - 1] = '\0';
      // else
      //   buffer[bytes_received] = '\0';
      std::cout << "Message from client: " << buffer << std::endl;
      if (strcmp(buffer, "exit") == 0)
      {
        std::cout << "The client wants to cut the connexion" << std::endl;
        break;
      }
      const char *reply = "Copy that °>\n";
      send(clientSocket, reply, strlen(reply), MSG_NOSIGNAL);
    }
    close(clientSocket);
    close(serverSocket);
    return (0);
}