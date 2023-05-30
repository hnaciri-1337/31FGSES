all:
	c++ Connection.cpp -o Register
	c++ authServer.cpp -o Auth
	c++ gameServer.cpp -o Game

fclean:
	rm -rf *.txt Game Auth Register
