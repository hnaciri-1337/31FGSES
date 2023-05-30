# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include <map>
# include <fcntl.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <filesystem>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <fstream>

class teamInfo
{
	public:
		int													level;
		std::vector <std::pair<std::string, std::string> >	_gameData;
		std::vector <std::string>							teamMembers;
		std::string											teamName;
		std::string											teamPassWord;
		int													socket;
		teamInfo(std::string teamName, std::vector <std::string> names, std::string teamPassWord)
		{
			this->level = 0;
			this->teamName = teamName;
			this->teamMembers = names;
			socket = -1;
			this->teamPassWord = teamPassWord;
			setChallenges();
			for (int i = 1; i < 10; i++) {
				_gameData.push_back (std::make_pair ("This is question number: " + std::to_string(i) + "\nAnswer is: " + std::to_string(i), std::to_string(i)));
			}
		}
	~teamInfo() {}
	private:
		void setChallenges()
		{
      // 1 + 1
			_gameData.push_back (std::make_pair ("How to list files in a directory", "ls"));
			_gameData.push_back (std::make_pair ("How to display a file content", "cat"));
			_gameData.push_back (std::make_pair ("You Have to find your team Password", this->teamPassWord));
			_gameData.push_back (std::make_pair ("How to list hidden files ", "ls -a"));
      // permission !
			_gameData.push_back (std::make_pair ("Now search for a file called rot13 and read it\nSo you can solve the next problem\nEnter the flag you find at the file !", "Uryyb STFRF. Jrypbzr Gb 1337 <3"));
			_gameData.push_back (std::make_pair ("What is ascii code of character 'a'", "97"));
			_gameData.push_back (std::make_pair ("Now u need to search About how root13 encryption Work\nWhat is decryption of this message 'Uryyb STFRF. Jrypbzr Gb 1337 <3' Please try To Decrypt it By yourself", "Hello FGSES. Welcome To 1337 <3"));
		}	
};
