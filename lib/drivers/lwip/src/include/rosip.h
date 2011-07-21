#ifndef _ROS_IP_H_
#define _ROS_IP_H_

#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"
#include "tcpip.h"

typedef struct tcp_pcb* PTCP_PCB;

/* External TCP event handlers */
extern void TCPConnectEventHandler(void *arg, const err_t err);
extern void TCPAcceptEventHandler(void *arg, struct tcp_pcb *newpcb);
extern void TCPSendEventHandler(void *arg, const u16_t space);
extern void TCPFinEventHandler(void *arg, const err_t err);
extern u32_t TCPRecvEventHandler(void *arg, struct pbuf *p);

/* TCP functions */
PTCP_PCB    LibTCPSocket(void *arg);
err_t       LibTCPBind(PCONNECTION_ENDPOINT Connection, struct ip_addr *const ipaddr, const u16_t port);
PTCP_PCB    LibTCPListen(PCONNECTION_ENDPOINT Connection, const u8_t backlog);
err_t       LibTCPSend(PCONNECTION_ENDPOINT Connection, void *const dataptr, const u16_t len, const int safe);
err_t       LibTCPConnect(PCONNECTION_ENDPOINT Connection, struct ip_addr *const ipaddr, const u16_t port);
err_t       LibTCPShutdown(PCONNECTION_ENDPOINT Connection, const int shut_rx, const int shut_tx);
err_t       LibTCPClose(PCONNECTION_ENDPOINT Connection, const int safe);

err_t       LibTCPGetPeerName(PTCP_PCB pcb, struct ip_addr *const ipaddr, u16_t *const port);
err_t       LibTCPGetHostName(PTCP_PCB pcb, struct ip_addr *const ipaddr, u16_t *const port);
void        LibTCPAccept(PTCP_PCB pcb, struct tcp_pcb *listen_pcb, void *arg);

/* IP functions */
void LibIPInsertPacket(void *ifarg, const void *const data, const u32_t size);
void LibIPInitialize(void);
void LibIPShutdown(void);

#endif