#include "network_core/barista_server.h"
#include "network_core/espresso_client.h"
#include "../decafs_barista/decafs_barista.h"

#include <stdio.h>
#include <thread>
#include "network_core/decafs_client.h"


// ----------------------- ESPRESSO STORAGE FAKE FUNCTIONS ------------------

ssize_t read_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count) {

  return count;
}

ssize_t write_chunk(int fd, int file_id, int stripe_id, int chunk_num,
    int offset, void *buf, int count) {

  return count;
}

int delete_chunk(int fd, int file_id, int stripe_id, int chunk_num) {

  return 0;
}

// ----------------------- BARISTA CORE FAKE FUNCTIONS ----------------------
// set_node_down
extern "C" uint32_t set_node_down (uint32_t node_number) {
 return 0;
}

// set_node_up
extern "C" uint32_t set_node_up (uint32_t node_number) {
  return 0;
}

// open_file 
extern "C" void open_file (const char *pathname, int flags, struct client client) {

  send_open_result(client, 1); 
}

extern "C" void read_file (int fd, size_t count, struct client client) {

  char fake_buffer[2500];
  send_read_result(client, fd, count, fake_buffer);
}

extern "C" void write_file (int fd, const void *buf, size_t count, struct client client) {

  send_write_result(client, fd, count);
}

extern "C" void close_file (int fd, struct client client) {

  send_close_result(client, -1);
}

// read_response_handler
extern "C" void read_response_handler (ReadChunkResponse *read_response) {

}

// write_response_handler
extern "C" void write_response_handler (WriteChunkResponse *write_response) {

}

// delete_response_handler
extern "C" void delete_response_handler (DeleteChunkResponse *delete_response) {

}

int main(int argc, char** argv) {

  int port = 12345;
  BaristaServer* barista_server = BaristaServer::init(port);
  std::thread barista_thread(&BaristaServer::run, barista_server);

  EspressoClient espresso("localhost", port, 1);
  std::thread espresso_thread(&EspressoClient::run, &espresso);

  // hostname, port, user_id
  DecafsClient client("localhost", port, 2);
  client.openConnection();

  sleep(1);

  // OPEN
  std::cout << "------------ DECAFS CLIENT OPEN TEST ----------" << std::endl;
  int fd = client.open("testfile", O_RDONLY);
  std::cout << "open returned: " << fd << std::endl;
  sleep(1);

  // WRITE
  std::cout << "------------ DECAFS CLIENT WRITE TEST ----------" << std::endl;
  char testwrite[] = "testing network write.";
  int bytes_written = client.write(fd, testwrite, strlen(testwrite));
  std::cout << "write returned: " << bytes_written << std::endl;
  sleep(1);

  // READ
  std::cout << "------------ DECAFS CLIENT READ TEST ----------" << std::endl;
  char testread[100];
  int bytes_read = client.read(fd, testread, 100);
  std::cout << "read returned: " << bytes_read << std::endl;
  sleep(1);

  // CLOSE
  std::cout << "------------ DECAFS CLIENT CLOSE TEST ----------" << std::endl;
  int close = client.close(fd);
  std::cout << "close returned: " << close << std::endl;
  sleep(1);

  barista_server->close();
  barista_thread.join();
  espresso_thread.join();

  return 0;
}