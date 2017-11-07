#include <iostream>
#include <boost\asio.hpp>
#include <thread>
#include <conio.h>

using namespace boost::asio;

#define FIELD_SIZE 25

////////////////////////struct user////////////////////////////////////
struct user
{
	int x, y;
	char model;

	user(int _x, int _y) : x(_x), y(_y) {};
	user(const user &_user);
	user(const user *_user);
	user();

	void setPosition(int *);
	void setPosition(int _x, int _y);
	void setPosition(int *, int *);
	void setPosition(int *, user &_user);
	
	int* getPosition();
	void getPosition(int *);
	void getPosition(int &_x, int &_y);

	void printPosition();
	void printPosition(int *);
	void printPosition(int _x, int _y);

	friend std::ostream& operator <<(std::ostream &os,user &_user);
	friend std::ostream& operator <<(std::ostream &os, user *_user);
};

////////////////////////////// user implementation begin ///////////////////////////////////////////

user::user()
{
	x = y = 0;
}

user::user(const user &_user)
{
	x = _user.x;
	y = _user.y;
}

user::user(const user *_user)
{
	x = _user->x;
	y = _user->y;
}


void user::setPosition(int *position)
{
	x = position[0];
	y = position[1];
}

void user::setPosition(int _x, int _y)
{
	x = _x;
	y = _y;
}

int* user::getPosition()
{
	int *position = new int[2];
	position[0] = x;
	position[1] = y;

	return position;
}

void user::getPosition(int *position)
{
	position[0] = x;
	position[1] = y;
}

void user::getPosition(int &_x, int &_y)
{
	_x = x;
	_y = y;
}

void user::printPosition()
{
	std::cout << " x = " << x << " | y = " << y << std::endl;
}

void user::printPosition(int *position)
{
	std::cout << " x = " << position[0] << " | y = " << position[1] << std::endl;
}

void user::printPosition(int _x, int _y)
{
	std::cout << " x = " << _x << " | y = " << _y << std::endl;
}

std::ostream& operator<<(std::ostream &os, user &_user)
{
	os << " x = " << _user.x << " | y = " << _user.y << std::endl;

	return os;
}

std::ostream& operator <<(std::ostream &os, user *_user)
{
	os << " x = " << _user->x << " | y = " << _user->y << std::endl;

	return os;
}

void user::setPosition(int *pos1, int *pos2)
{
	pos1[0] = pos2[0];
	pos1[1] = pos2[1];

}

void user::setPosition(int *pos, user &_user)
{
	pos[0] = _user.x;
	pos[1] = _user.y;
}


void swap_position(int *p1, int *p2)
{
	p1[0] = p2[0];
	p1[1] = p2[1];
	p1[2] = p2[2];
	p1[3] = p2[3];
}

user *u1, *u2;

////////////////////////////// user implementation end///////////////////////////////////////////
////////////////////////////// end struct user /////////////////////////////////////////////////

////////////////////////////// Class Socket /////////////////////////////////////////////////////

enum SOCKET_STATUS
{
	SOCKET_CONNECTED = 1,
	SOCKET_DISCONNECTED = 0
};

class Socket
{
private:

	ip::tcp::socket *_socket;

	SOCKET_STATUS status;

public:

	Socket(io_service &service);
	Socket(ip::tcp::socket *_socket);
	Socket(Socket &_sock);
	Socket(Socket *_sock);

	~Socket();

	void read(int *buff, int size_message, boost::system::error_code &er);
	void read(int *buff, boost::system::error_code &er);
	void read(int &, boost::system::error_code &er);

	void write(int *buff, int size_message, boost::system::error_code &er);
	void write(int *buff, boost::system::error_code &er);
	void write(int &, boost::system::error_code &er);

	void set_socket(ip::tcp::socket *_socket);

	void connect(ip::tcp::endpoint &_endpoint);

	void close();

	void set_socket_status(SOCKET_STATUS _status);

	SOCKET_STATUS sock_status();
	
	ip::tcp::socket* get_socket();
};

////////////////////////////// Socket implementation end ///////////////////////////////////////////

Socket::Socket(io_service &service)
{
	_socket = new ip::tcp::socket(service);
}

Socket::Socket(ip::tcp::socket *_sock)
{
	_socket = _sock;
}

Socket::Socket(Socket &_sock)
{
	_socket = _sock.get_socket();
}

Socket::Socket(Socket *_sock)
{	
	_socket = _sock->get_socket();
}

Socket::~Socket()
{
	if (_socket)
	{
		this->close();
		delete _socket;
	}
}


void Socket::read(int *buff, int size_buffer, boost::system::error_code &er)
{
	if (_socket && !status)
	{	
		int position[3] = { 0,0,0 };
		_socket->read_some(buffer(position, size_buffer), er);

		swap_position(buff, position);
	}
}

void Socket::read(int *buff, boost::system::error_code &er)
{
	if (_socket && status == SOCKET_CONNECTED)
	{
		int position[4] = { 0,0,0,0 };
		_socket->read_some(buffer(position), er);

		swap_position(buff, position);
	}
}

void Socket::read(int &i, boost::system::error_code &er)
{
	if (_socket && status)
	{
		int key_code[1] = { i };
		_socket->write_some(buffer(key_code), er);
	}
}

void Socket::write(int *buff, int size_buffer, boost::system::error_code &er)
{
	if (_socket && status == SOCKET_CONNECTED)
	{	
		int position[3] = { buff[0],buff[1],buff[2] };
		_socket->write_some(buffer(position, size_buffer), er);
	}
}
void Socket::write(int *buff, boost::system::error_code &er)
{
	if (_socket && status == SOCKET_CONNECTED)
	{
		int position[3] = { buff[0],buff[1],buff[2] };
		_socket->write_some(buffer(position), er);
	}
}

void Socket::write(int &i, boost::system::error_code &er)
{	
	if (_socket && status == SOCKET_CONNECTED)
	{	
		int key_code[1] = { i };
		_socket->write_some(buffer(key_code), er);
	}
}

void Socket::set_socket(ip::tcp::socket *_sock)
{
	_socket = _sock;
}

void Socket::connect(ip::tcp::endpoint &_endpoint)
{
	boost::system::error_code er;

	if (_socket)
	{
		_socket->connect(_endpoint, er);
		if (er)
		{
			std::cout << "Error connection : ip =  and port =  " << std::endl;
			return;
		}
	}

	status = SOCKET_CONNECTED;
}

void Socket::close()
{
	_socket->close();
	status = SOCKET_DISCONNECTED;
}

SOCKET_STATUS Socket::sock_status()
{
	return status;
}

void Socket::set_socket_status(SOCKET_STATUS _status)
{
	status = _status;
}

ip::tcp::socket* Socket::get_socket()
{
	return _socket;
}
////////////////////////////// Socket implementation end ///////////////////////////////////////////
////////////////////////////// End Class Socket         ///////////////////////////////////////////


//////////////////////////////////////// Network Functions ///////////////////////////////////////////////////

////////////////////////////////        Listen         /////////////////////////////////////////////

/////Help function
void print_positions(int *positions)
{
	std::cout << "u1->x = " << positions[0] << " u1->y = " << positions[1] << " | u2->x = " << positions[2] << " u2->y = " << positions[3] << std::endl;
}

void listen_server(Socket *_socket,user *_u1,user *_u2)
{	
	int server_data[4];
	boost::system::error_code er;

	while (1)
	{
		_socket->read(server_data,er);

		if (er || _socket->sock_status() == SOCKET_DISCONNECTED)
		{
			std::cout << "Server is die" << std::endl;
			_socket->set_socket_status(SOCKET_DISCONNECTED);

			break;
		}
		
		_u1->x = server_data[0];
		_u1->y = server_data[1];

		_u2->x = server_data[2];
		_u2->y = server_data[3];

		//print_positions(server_data);
	}
}

////////////////////////////////        Write          /////////////////////////////////////////////

/*
void write(Socket *_socket, user *_user)
{
	int position[2];
	boost::system::error_code er;

	while (1)
	{	
		_user->setPosition(position, *_user);
		_socket->write(position, 2, er);

		if (er || _socket->sock_status() == SOCKET_DISCONNECTED)
		{
			std::cout << "Server is die" << std::endl;
			_socket->set_socket_status(SOCKET_DISCONNECTED);
			break;
		}
	}
}
*/

void session(Socket *_socket, user *_u1, user *_u2)
{
	std::thread t(listen_server, _socket, _u1, _u2);
	t.detach();
}

/////////////////////////////////// End Network functions /////////////////////////////////////////

/////////////////////////////////// Game functions ////////////////////////////////////////////////

//////////////////////////////////////// DrawField /////////////////////////////////////////////////
void drawField()
{
	system("cls");

	for (short y = 0; y < FIELD_SIZE; y++)
	{
		for (short x = 0; x < FIELD_SIZE; x++)
		{
			if (u1->x == x && u1->y == y)
			{
				std::cout << u1->model;
				continue;
			}
			else if (u2->x == x && u2->y == y)
			{
				std::cout << u2->model;
				continue;
			}
			std::cout << "*";
		}
		std::cout << std::endl;
	}
}

//////////////////////////////////////// Key ///////////////////////////////////////////////// 
void key(Socket *_socket)
{
	int key = _getch();

	boost::system::error_code er;
	_socket->write(key, er);

	if (er)
	{
		std::cout << "Server is die" << std::endl;

		_socket->set_socket_status(SOCKET_DISCONNECTED);
	}
}


//////////////////////////////////////// GameLoop ///////////////////////////////////////////////// 
void gameLoop(Socket *_socket)
{
	while (1)
	{
		key(_socket);
		drawField();
	}
}

//////////////////////////////////////// Chek /////////////////////////////////////////////////
void chek()
{
	//chek
}

int main()
{
	setlocale(LC_ALL, "Russian");

	io_service service;
	ip::tcp::endpoint _endpoint(ip::address_v4::from_string("127.0.0.1"),1234);

	Socket *_socket = new Socket(service);
	_socket->connect(_endpoint);
	
	u1 = new user();
	u2 = new user();

	u1->model = '#';
	u2->model = '%';

	if (_socket->sock_status())
	{

		int number_me = 0;

		boost::system::error_code er;

		if (er)
		{
			std::cout << "Server is die(from main)" << std::endl;
			_getch();
		}

		session(_socket, u1, u2);

		gameLoop(_socket);
	}
	else
	{
		std::cout << "Socket disconnected...?" << std::endl;
	}

	_getch();

	delete u1, u2;

	return 0;
}