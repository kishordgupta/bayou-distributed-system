//============================================================================
// Name        : test.cpp
// Author      : kishor
// Version     :2
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
using namespace std;

#define dept 0
#define replica "replica.csv"
#define deptnumber 10
#define roomnumber 8
#define timeslots  9

/////////////////////////////opengl//////////////////

// Global variables
// Window/viewport
const int startwinsize = 800; // Starting window width & height, in pixels
int winw, winh; // Window width & height, in pixels, saved by reshape
string serverstatus = "";
// Mouse
bool mouseleftdown = false; // True if mouse LEFT button is down.
// Saved by mouse.
int mousex, mousey; // Mouse x,y coords, in GLUT format (pixels from upper-left corner).
int xad = 100;
int yad = 100; // Only guaranteed to be valid if a mouse button is down.
int currentselectednumber = 100; // Saved by mouse, motion.
string data = "";
bool serverclos = false;
bool listenothers = false;
// Keyboard
const int ESCKEY = 27; // ASCII value of escape character
const int BOOKKEY = 42;
const int BOOKKEYS = 98;
// Other
const double pointsize = 40; // Size of point
////////////////

int parsedata(string line, int i);
int getdatafromreplica();
string timeslotsprint(int i);
int deptserverports(int i);
string forservermsg();
string forserverversion();
int leghtfile();
int roomclicked(int x, int y);
string gettime();

////////////////////////////////////////////////////////
std::string timeslotsa[] = { "08:00 09:00", "09:00 10:00", "10:00 11:00",
		"11:00 12:00", "12:00 13:00", "13:00 14:00", "14:00 15:00",
		"15:00 16:00", "16:00 17:00" };

int deptports[] =
		{ 5000, 5001, 5002, 5003, 5004, 5005, 5006, 5007, 5008, 5009 };
std::string deptips[] = { "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1",
		"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1",
		"127.0.0.1" };
std::string secretarynames[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I",
		"J" };
std::string timestamps[72];
std::string roomstatus[72];
string currentversion;
int datalength;
string deptserverip(int i) {
	return deptips[i];
}
string timeslotsprint(int i) {
	return timeslotsa[i];
}
int deptserverports(int i) {
	return deptports[i];
}
int leghtfile() {
	return roomnumber * timeslots;
}
string roombookedby(int roomid, int timeslot) {
	roomid--;
	return roomstatus[roomid * (timeslots) + timeslot];
}
string bookingtimestamps(int roomid, int timeslot) {
	roomid--;
	return timestamps[roomid * (timeslots) + timeslot];
}
string forserverversion() {
	return currentversion;
}
string InttoString(int a) {
	ostringstream temp;
	temp << a;
	return temp.str();
}
int StringtoInt(string a) {
	int result;
	stringstream(a) >> result;
	return result;
}
string gettime() {

	time_t now = time(0);
	tm *ltm = localtime(&now);
	string s1, s2, s3;
	if (ltm->tm_hour / 10 == 0)
		s1 = "0" + InttoString(ltm->tm_hour);
	else
		s1 = InttoString(ltm->tm_hour);
	if (ltm->tm_min / 10 == 0)
		s2 = "0" + InttoString(ltm->tm_min);
	else
		s2 = InttoString(ltm->tm_min);
	if (ltm->tm_sec / 10 == 0)
		s3 = "0" + InttoString(ltm->tm_sec);
	else
		s3 = InttoString(ltm->tm_sec);

	return s1 + s2 + s3;

}

int getdatafromreplica() {
	ifstream myfile(replica);
	if (!myfile) {
		cout << "ERROR REPLICA NOT FOUND\n";
		return -1;
	}
	getline(myfile, currentversion);

	string buf;
	getline(myfile, buf);
	std: string line;
	int i = 0;
	while (getline(myfile, line)) {
		parsedata(line, i);
		i++;

	}
	myfile.close();
	return 1;
}
int parsedata(string line, int i) {
	string buf;
	std::stringstream stream(line);
	std::getline(stream, buf, ','); //roomid
	std::getline(stream, buf, ','); // slotid
	std::getline(stream, roomstatus[i], ',');
	std::getline(stream, timestamps[i], ',');
	return 1;
}

string forservermsg() {

	string data = currentversion+ "\n";;
	for (int i = 0; i < datalength; i++) {
		if (timestamps[i].size() > 2) {
			data = data + InttoString(i) + "," + roomstatus[i] + ","
					+ timestamps[i] + "\n";

		}
	}
	serverstatus = data;
	return data;

}
void newversion() {
	int i = dept*2;
	char ae = currentversion[i];

	char b = currentversion[i+1];
	stringstream as;
	as << ae;
	as << b;
	string a;
	as >> a;
	int aa = StringtoInt(a);
	aa = aa + 1;
	a = InttoString(aa);

	if (a.size() == 2) {
		currentversion[i] = a[0];
		currentversion[i+1] = a[1];
	} else
		currentversion[i + 1] = a[0];

}
void savedata() {
	newversion();
	ofstream myfile;
cout<<currentversion;
	myfile.open(replica);
	myfile << currentversion << endl;
	myfile << "roomno,timeslot,depts,time" << endl;
	myfile << InttoString(1) + +"," + InttoString(1) + "," + roomstatus[0]
			+ "," + timestamps[0] << endl;

	for (int i = 1; i < datalength; i++) {

		int d = i / timeslots + 1;
		int k = i % timeslots + 1;
		myfile << InttoString(d) + +"," + InttoString(k) + "," + roomstatus[i]
				+ "," + timestamps[i] << endl;

	}
	myfile.close();
	forservermsg();
}
//////////////////////////GL RELATED FUNCTIONS
void printbitmap(const string msg, double x, double y) {
	glRasterPos2d(x, y);
	for (string::const_iterator ii = msg.begin(); ii != msg.end(); ++ii) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *ii);
	}
}

// display
// The GLUT display function
int xid(double x) {
	x = x * 100;
	int id = 100;
	if (x > 12 && x < 20)
		return 1;
	if (x > 21 && x < 28)
		return 2;
	if (x > 29 && x < 36)
		return 3;
	if (x > 37 && x < 45)
		return 4;
	if (x > 46 && x < 54)
		return 5;
	if (x > 55 && x < 63)
		return 6;
	if (x > 64 && x < 72)
		return 7;
	if (x > 73 && x < 80)
		return 8;
	if (x > 81 && x < 87)
		return 9;

	return id;
}
int yid(double x) {
	x = x * 100;
	int id = 100;
	if (x < 88 && x > 83)
		return 1;
	if (x < 82 && x > 77)
		return 2;
	if (x < 77 && x > 72)
		return 3;
	if (x < 72 && x > 67)
		return 4;
	if (x < 67 && x > 62)
		return 5;
	if (x < 62 && x > 57)
		return 6;
	if (x < 57 && x > 52)
		return 7;
	if (x < 52 && x > 47)
		return 8;
	if (x < 47 && x > 42)
		return 9;

	return id;
}

int roomclicked(int x, int y) {
	return x + (y - 1) * timeslots;
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Make a small box at the mouse position, if the LEFT button is down
	if (mouseleftdown) {
		// Convert mouse position to OpenGL's coordinate system
		double oglx = double(mousex) / winw;
		double ogly = 1 - double(mousey) / winh;
		char buffer[32];
		xad = xid(oglx);
		yad = yid(ogly);

		if (xad != 100 || yad != 100)
			currentselectednumber = roomclicked(xad, yad);
		else
			currentselectednumber = 100;

		glColor3d(0.9, 0.1, 0.1);
		glPointSize(pointsize);
		glBegin(GL_POINTS);
		glVertex2d(oglx, ogly);
		glEnd();
	}
	string printsw = "";//InttoString(currentselectednumber);

	// Draw the box
	printsw = "Select room by mousclick";
	if (currentselectednumber == 100)
		printsw = "Select room by mousclick";
	if (currentselectednumber != 100)
		printsw = "Selected Room " + InttoString(yad) + " For timeslot "
				+ timeslotsa[xad - 1] + ". Press 'B' for confirm";

	printbitmap(printsw, 0.05, 0.35);
	printbitmap(data, 0.05, 0.25);
	// Documentation
	glColor3d(0.0, 0.0, 0.0); // Black text
	printbitmap(currentversion, 0.05, 0.10);
	printsw="Daily Scheduler for Department " + InttoString(dept+1)+ " FOR  MAY 1 2017 (SELECT ANOTHER DATE BY PRESS T)",
	printbitmap(
			printsw,
			0.05, 0.95);
	printbitmap(
			"RoomNo  08-09 || 09-10 || 10-11 || 11-12 || 12-13 || 13-14 || 14-15 || 15-16 || 16-17",
			0.05, 0.9);
	float setx = 0.05;
	float sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(InttoString(i), setx, sety);
		sety = sety - 0.05;
	}

	setx = 0.15;
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 0), setx, sety);
		sety = sety - 0.05;
	}

	setx = 0.24;

	//setx =0.20
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 1), setx, sety);
		sety = sety - 0.05;
	}
	setx = 0.32;
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 2), setx, sety);
		sety = sety - 0.05;
	}
	setx = 0.40;
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 3), setx, sety);
		sety = sety - 0.05;
	}

	setx = 0.50;
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 4), setx, sety);
		sety = sety - 0.05;
	}
	setx = 0.58;
	// setx =0.20
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 5), setx, sety);
		sety = sety - 0.05;
	}

	setx = 0.66;
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 6), setx, sety);
		sety = sety - 0.05;
	}

	setx = 0.74;
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 7), setx, sety);
		sety = sety - 0.05;
	}

	setx = 0.82;
	sety = 0.85;
	for (int i = 1; i < 9; i++) {
		printbitmap(roombookedby(i, 8), setx, sety);
		sety = sety - 0.05;
	}

	glutSwapBuffers();
}

// mouse
// The GLUT mouse function
void mouse(int button, int state, int x, int y) {
	// Save the left button state
	if (button == GLUT_LEFT_BUTTON) {
		mouseleftdown = (state == GLUT_DOWN);
		glutPostRedisplay(); // Left button has changed; redisplay!
	}

	// Save the mouse position
	mousex = x;
	mousey = y;
}

// motion
// The GLUT motion function
void motion(int x, int y) {
	// We only do anything if the left button is down
	if (mouseleftdown)
		glutPostRedisplay();

	// Save the mouse position
	mousex = x;
	mousey = y;
}

// idle
// The GLUT idle function
void idle() {
	// Print OpenGL errors, if there are any (for debugging)
	if (GLenum err = glGetError()) {
		cerr << "OpenGL ERROR: " << gluErrorString(err) << endl;
	}
}
void booktheroom() {

	timestamps[currentselectednumber - 1] = gettime();
	roomstatus[currentselectednumber - 1] = secretarynames[dept];
	savedata();

}
void bookingroom() {
	string a = "0";
	if (currentselectednumber != 100) {
		if (a.compare(roomstatus[currentselectednumber - 1]) != 0) {
			data = "SELECTED ROOM IS ALREADY OCCUPIED";
		} else {
			booktheroom();
			data = "SELECTED ROOM IS BOOKED " + gettime();
			currentselectednumber = 100;
		}
	}

	else {
		data = "NO ROOM SELECTED FOR ENTRY";
	}
	glutPostRedisplay();

}

// keyboard
// The GLUT keyboard function
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case BOOKKEY:
		bookingroom();
		break;
	case BOOKKEYS:
		bookingroom();
		break;
	case ESCKEY:
		serverclos = true;
		listenothers = true;
                savedata();
		sleep(2);
		exit(0);
		break;
	}
}

// reshape
// The GLUT reshape function
void reshape(int w, int h) {
	// Set the viewport to be the entire drawing area of the window
	glViewport(0, 0, w, h);

	// Save the window size
	winw = w;
	winh = h;

	// Set up the projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW); // Always go back to model/view mode
}

// init
// Initializes GL states
// Called by main after window creation
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
//////////////////////////////////////////////network///////////////////////


int sendotherdept(int portno) {

	struct sockaddr_in cli_addr, serv_addr;
	int port;
	char strings[2048];
	char cers[2] = { '0', '\0' };

	port = portno;
	socklen_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
	/* open a TCP socket (an Internet stream socket) */
	if (sockfd < 0) {
		perror("can't open stream socket");
		exit(1);
	}

	/* bind the local address, so that the cliend can send to server */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("can't bind local address");
		return 0;
	} else {
		string status = "0";

		/* listen to the socket */
		listen(sockfd, 10);

		for (;;) {

			/* wait for a connection from a client; this is an iterative server */
			socklen_t clilen = sizeof(cli_addr);
			socklen_t newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
					&clilen);

			if (newsockfd < 0) {
				perror("can't bind local address");

				close(newsockfd);
			} else {
				/* read a message from the client */
				int len = read(newsockfd, strings, sizeof(strings));
				strings[len] = '\0';
				string str(strings);
				cout<<"Data from clients  "<<str<<endl<<currentversion<<endl;

				if (str.compare(currentversion) == 0)
					send(newsockfd, cers, sizeof(cers), 0);
				else {
					char myArray[serverstatus.size() + 1];//as 1 char space for null is also required
					strcpy(myArray, serverstatus.c_str());
					//cout<<"Data sents  "<<serverstatus<<endl;
					send(newsockfd, myArray, strlen(myArray), 0);
				}

				close(newsockfd);
			}
			if (serverclos)
				break;
		}
		close(sockfd);
		serverclos = false;
		return 0;
	}
}

void parsedatafromserver(string line) {
	string timestampserver;
	string row;
	string deptar;
	std::stringstream stream(line);

	std::getline(stream, row, ',');
	std::getline(stream, deptar, ',');
	std::getline(stream, timestampserver, ',');

	int rowid = StringtoInt(row);
     
	int timeid = StringtoInt(timestampserver);
	int timeid2 = StringtoInt(timestamps[rowid]);
      if (timeid2==0) {
		timestamps[rowid] = timestampserver;
		roomstatus[rowid] = deptar;
	}
	else if (timeid < timeid2) {
		timestamps[rowid] = timestampserver;
		roomstatus[rowid] = deptar;
	}
 else	if (timeid == timeid2) {
		if (roomstatus[rowid] < deptar) {
			timestamps[rowid] = timestampserver;
			roomstatus[rowid] = deptar;
		}
	}
}
void updateversion(string ver) {
  //0 3 5 7 9 11 13 15 17 19
// 00 00 00 00 00 00 00 00 00 00
	for (int i = 0; i < currentversion.size() ; ) {
           
 
         if(currentversion[i]<ver[i]){
		currentversion[i]=ver[i];
                currentversion[i+1]=ver[i+1];
                
                }
	else if(currentversion[i]==ver[i]){
		if(currentversion[i+1]<ver[i+1])
                currentversion[i+1]=ver[i+1]; 
                }
         else if(currentversion[i]>ver[i]){
		 
                }
         i=i+2;
 }


cout<<" "<<currentversion<<" "<<endl;

}


void datacompare(string buffercv) {
	string buf;
	std::istringstream stream(buffercv);
	getline(stream, buf);

	updateversion(buf);
	while (std::getline(stream, buf)) {
		cout<<buf<<endl;
		parsedatafromserver(buf);
	}

}

int syncwithothers(int portno, string ip) {
	int returnstatus = 0;
	int clientSocket;
	// char buffer[18];
	char buffercv[2048];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	char myArray[ip.size() + 1];//as 1 char space for null is also required
	strcpy(myArray, ip.c_str());
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portno);
	serverAddr.sin_addr.s_addr = inet_addr(myArray);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	addr_size = sizeof serverAddr;
	/* open a TCP socket */
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("can't open stream socket");
		//exit(1);
	}

	/* connect to the server */
	if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0) {
		close(clientSocket);
		return 0;
	} else {
		char versa[currentversion.size() + 1];//as 1 char space for null is also required
		strcpy(versa, currentversion.c_str());
		/* write a message to the server */
		write(clientSocket, versa, sizeof(versa));

		int len;
		/*---- Read the message from the server into the buffer ----*/
		len = recv(clientSocket, buffercv, sizeof(buffercv), 0);
		buffercv[len]='\0';
		if (len > 2) {
			string str(buffercv);
			//cout<<"Data from servers "<<str<<endl;
			datacompare(str);

		}
		else
			cout<<"NO NEED"<<endl;
		close(clientSocket);
		return returnstatus;
	}
}
///////////////////////////////Threading/////////////////////////
void* tellother(void* arg) {
	cout << "telling" << endl;
	sendotherdept(deptports[dept]);
}
void* listenother(void* arg) {
	cout << "Listening" << endl;
	while (listenothers == false) {
		for (int i=0; i < deptnumber; i++) {
			if (i != dept)
				syncwithothers(deptports[i], deptips[i]);
		}
		sleep(2);
	}
	listenothers = false;
}

/////////////////////////////////////////////////////////////////
int main(int argc, char ** argv) {
	datalength = leghtfile();
	getdatafromreplica();
	//savedata();

	
	pthread_t thr;
	pthread_t thr2;
	pthread_create(&thr, NULL, tellother, NULL);
	pthread_create(&thr2, NULL, listenother, NULL);
	// Initialize OpenGL/GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Make a window
	glutInitWindowSize(startwinsize, startwinsize);
	glutInitWindowPosition(50, 50);
	string s ="Advanced Operating system "+InttoString(dept);
	glutCreateWindow("Advanced Operating system ");

	// Initialize GL states & register callbacks
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// Do something
	glutMainLoop();

	//cout << "!!!Hello World!!!" << forservermsg() << endl; // prints !!!Hello World!!!
	return 0;
}
