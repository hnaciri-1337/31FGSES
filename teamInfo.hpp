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
		teamInfo(std::string teamName, std::vector <std::string> names)
		{
			this->level = 0;
			this->teamName = teamName;
			this->teamMembers = names;
			for (int i = 0; i < 10; i++) {
				_gameData.push_back (std::make_pair ("This is question number: " + std::to_string(i) + "\nAnswer is: " + std::to_string(i), std::to_string(i)));
			}
		}
		~teamInfo() {}
};
