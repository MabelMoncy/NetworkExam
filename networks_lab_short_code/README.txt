NETWORKS LAB - SHORT, LEARNABLE C PROGRAMS
========================================

The code keeps meaningful variable names and the lab-manual behaviour while
removing unnecessary lines. Run commands from this folder.

Compile all:
  gcc Chatserver.c -o chatserver -pthread
  gcc Chatclient.c -o chatclient -pthread
  gcc Timeserver.c -o timeserver
  gcc Timeclient.c -o timeclient
  gcc Fileserver.c -o fileserver
  gcc Fileclient.c -o fileclient
  gcc packet.c -o packet

EXP 6 - MULTI-USER TCP CHAT
Terminal 1: ./chatserver
Terminal 2 and 3 (and more): ./chatclient
Messages are broadcast to the other connected clients. Ctrl+D ends client input.

EXP 7 - UDP TIME SERVER
Terminal 1: ./timeserver
Terminal 2: ./timeclient
The client sends TIME and prints the server's system time.

EXP 8 - CONCURRENT TCP FILE SERVER
Create a sample file in this directory:
  printf 'welcome to networks lab\\n' > sample.txt
Terminal 1: ./fileserver
Terminal 2: ./fileclient
Enter sample.txt when prompted. Enter a missing filename to test the error response.
The server sends its child process PID and file contents/error.

EXP 9 - RAW SOCKET PACKET CAPTURE
Compile: gcc packet.c -o packet
Run: sudo ./packet
Stop with Ctrl+C. Raw socket capture requires root privileges. It reports IPv4
packets, packet size, source IP and destination IP.

All network programs use port 8080. Run only one server on port 8080 at a time.
If "Address already in use" appears, stop the other server first (Ctrl+C).
