#include <arpa/inet.h>
#include <curses.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "reactor/handle.h"
#include "utils/logging.h"
#include "utils/net_ops.h"

#define NAME_SIZE 20
#define BUFFER_SIZE 1024
using namespace std;
using namespace reactor;

char name[NAME_SIZE];

WINDOW *top, *bottom;
int input_line = 1, output_line = 1;
int maxx, maxy, bottom_height = 4, top_height;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Handle CreateAndConnect(const char* ip, in_port_t port);
void* SendMsg(void*);
void* RecvMsg(void*);
void CreateWindows();

int main(int argc, char* argv[]) {
  if (argc != 4) {
    LOG_FATAL("Usage %s <ip> <port> <name>", argv[0]);
  }

  Handle clnt_handle = CreateAndConnect(argv[1], atoi(argv[2]));
  CreateWindows();
  snprintf(name, BUFFER_SIZE, "[%s]", argv[3]);

  pthread_t send_thread, recv_thread;
  pthread_create(&send_thread, NULL, SendMsg, (void*)&clnt_handle);
  pthread_create(&recv_thread, NULL, RecvMsg, (void*)&clnt_handle);
  pthread_join(send_thread, NULL);
  pthread_join(recv_thread, NULL);
  endwin();
  pthread_mutex_destroy(&mutex);
  return 0;
}

Handle CreateAndConnect(const char* ip, in_port_t port) {
  Handle sockfd = net::CreateSocket();
  net::EndPoint remote_side;
  if (net::Str2EndPoint(ip, port, remote_side)) {
    LOG_FATAL("Invalid ip or port");
  }
  int re;
  do {
    re = Connect(sockfd, remote_side);
  } while (re == -1 && (errno == EAGAIN || errno == EINTR));
  if (re == -1) {
    LOG_FATAL("connect() system error, %s", strerror(errno));
  }
  return sockfd;
}

void* SendMsg(void* arg) {
  int sockfd = *((int*)arg);
  char name_msg[NAME_SIZE + BUFFER_SIZE], msg[BUFFER_SIZE], prefix1[] = ">> ",
                                                            prefix2[] = "|-->";
  mvwprintw(bottom, input_line, 1, "%s ", prefix1);
  while (1) {
    wrefresh(top);
    wrefresh(bottom);
    mvwgetstr(bottom, input_line, sizeof(prefix1), msg);
    if (!strcmp(msg, "exit")) {
      if (::shutdown(sockfd, SHUT_WR) == -1) {
        LOG_FATAL("close() error, %s", strerror(errno));
      }
      return (void*)0;
    }
    if (strlen(msg) == 0) {
      continue;
    }
    snprintf(name_msg, NAME_SIZE + BUFFER_SIZE, "%s %s", prefix2, msg);
    mvwprintw(top, output_line, 2, name_msg);
    snprintf(name_msg, NAME_SIZE + BUFFER_SIZE, "%s %s", name, msg);
    if (::write(sockfd, name_msg, strlen(name_msg)) < -1) {
      LOG_FATAL("write() error, %s", strerror(errno));
    }
    scroll(bottom);
    wborder(bottom, '|', '|', '-', '-', '|', '|', '+', '+');
    pthread_mutex_lock(&mutex);
    if (output_line != top_height - 2) {
      ++output_line;
    } else {
      scroll(top);
      wborder(top, '|', '|', '-', '-', '+', '+', '|', '|');
    }
    pthread_mutex_unlock(&mutex);
    mvwprintw(bottom, input_line, 1, "%s", prefix1);
  }
  return 0;
}

void* RecvMsg(void* arg) {
  int sockfd = *((int*)arg);
  char name_msg[NAME_SIZE + BUFFER_SIZE];
  int str_len;
  while (1) {
    wrefresh(top);
    wrefresh(bottom);
    str_len = ::read(sockfd, name_msg, NAME_SIZE + BUFFER_SIZE - 1);
    if (str_len == 0) {
      ::shutdown(sockfd, SHUT_RD);
      return (void*)0;
    } else if (str_len == -1) {
      return (void*)-1;
    }
    name_msg[str_len] = 0;
    mvwprintw(top, output_line, 2, name_msg);
    pthread_mutex_lock(&mutex);
    if (output_line != top_height - 2)
      ++output_line;
    else {
      scroll(top);
      wborder(top, '|', '|', '-', '-', '+', '+', '|', '|');
    }
    pthread_mutex_unlock(&mutex);
  }
  return 0;
}

void CreateWindows() {
  initscr();
  getmaxyx(stdscr, maxy, maxx);
  top_height = maxy - bottom_height;
  top = newwin(top_height, maxx, 0, 0);
  bottom = newwin(bottom_height, maxx, top_height, 0);
  scrollok(top, TRUE);
  scrollok(bottom, TRUE);
  wborder(top, '|', '|', '-', '-', '+', '+', '|', '|');
  wborder(bottom, '|', '|', '-', '-', '|', '|', '+', '+');
  wrefresh(top);
  wrefresh(bottom);
  wsetscrreg(top, output_line, top_height - 2);
  wsetscrreg(bottom, input_line, bottom_height - 2);
}