#ifndef __IPERFAPP_H__
#define __IPERFAPP_H__


/* these are the ports for this application */

#define IPERF_APP_SERVER_PORT (5001)
#define kIperfAppID (1100)

#define IPERF_GET_MSEC_TICK_COUNT() TickGet()

extern void IperfAppInit(void);
extern void IperfAppCall(void);

#endif /* __IPERFAPP_H__ */
