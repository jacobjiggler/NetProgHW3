#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc , char *argv[])
{
  std::string port;
  std::string host;
  std::string path;
  if (argc !=  2){
    std::cout << "bad arguments" << std::endl;
    return 1;
  }
  std::string input = argv[1];
  int index = input.find(":",5);
  int index2 = input.find("/",7);
  if (index2 == std::string::npos){
    index2 = input.length();
  }
  if (index == std::string::npos){
    port = "80";
    index = index2;
  }
  else {
    port = input.substr(index+1,index2 - index - 1);
  }
  host = input.substr(7,index - 7);
  std::cout << host << ":" << port << std::endl;

  path = input.substr(index2+1,input.length() - index2 - 1);



  struct addrinfo hints;
  struct addrinfo * res = 0;

  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype=SOCK_STREAM;
  hints.ai_protocol=IPPROTO_TCP;
  hints.ai_flags = (AI_ADDRCONFIG | AI_ALL);
  int err=getaddrinfo(host.c_str(),port.c_str(),&hints,&res);
  if (err!=0) {
      std::cout << "failed to resolve remote socket address " << gai_strerror(err) << std::endl;
      exit(1);
  }
  int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if (fd==-1) {
      perror("socked creation failed");
  }
  if (connect(fd,res->ai_addr,res->ai_addrlen)==-1) {
      perror("Failed to connect");
  }

}
