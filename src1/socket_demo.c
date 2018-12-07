#include <posapi.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netif.h>

#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>



#include "../src/utilities/lcd.h"

#define MAX_CONNECTIONS 1
#define SERVER_PORT 9000
#define SERVER_ADDR "192.168.1.233"
void tcp_init(void);
void   tcp_close(int isocket);
int tcp_server(unsigned int uiPort, unsigned int MaxConnect);
int tcp_accept(int iSvrSocket, char szClientIP[16], unsigned int * puiClientPort);
int tcp_connect(const char * pServerAddr, unsigned int port);
int tcp_read(int isocket, void * pvbuff, unsigned int * pisize);
int tcp_write(int isocket, const void * pvdata, unsigned int isize);



void socket_server_demo(void){
	int iserver_socket = -1;
	int iclient_socket = -1;
	unsigned char bTemp[1024];
	unsigned int  iTemp = 0;
	int i;
	int active_net = 0;
	char client_addr[255];
	unsigned int client_port = 0;
	char iflist[16][IFNAMSIZ];
	struct ifreq ifr;
	int error;
	
	lcd_clean();
	lcd_printf(ALG_CENTER, "socket server");
	iTemp = netif_get_list(iflist, sizeof(iflist)/IFNAMSIZ);
	for (i = 0; i < iTemp; i++){
		memset(&ifr, 0, sizeof(ifr));
		netif_get_flags(iflist[i], &ifr);
		if (ifr.ifr_flags & IFF_RUNNING){
			netif_get_ipaddr(iflist[i], &ifr);
			lcd_printf(ALG_CENTER, "Address:%s:%d", inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr), SERVER_PORT);
			active_net++;
		}
	}
	
	if (active_net == 0){
		lcd_printf(ALG_CENTER, "NO Active net work");	
		lcd_flip();
		kb_getkey();
		return;
	}
	
	lcd_flip();
	iserver_socket = tcp_server(SERVER_PORT, 1);
	if (iserver_socket >=0){
		lcd_printf(ALG_LEFT, "Wait connection on %d...", SERVER_PORT);
		lcd_flip();
		while (1){
			if (kb_hit()){
				if (0x1B == kb_getkey())
					break;
			}

			iclient_socket = tcp_accept(iserver_socket, client_addr, &client_port);
			if (iclient_socket < 0)
				continue;
			else {
				lcd_printf(ALG_LEFT, "Connected from %s:%d...", client_addr, client_port);
				while (1){
					if (kb_hit()){
						if (0x1B == kb_getkey())
						break;
					}
					iTemp = sizeof(bTemp);
					error =  tcp_read(iclient_socket, bTemp, &iTemp);
					
					if (error == 0){
						lcd_flip();
						if (iTemp > 0)
						{
							lcd_printf(ALG_LEFT, "Readed %d bytes", iTemp);
							lcd_flip();
							error = tcp_write(iclient_socket, bTemp, iTemp);
							if (error != 0){
								lcd_printf(ALG_LEFT, "connection was broken");
								lcd_flip();
								break;
							}
						}
						else {
							usleep(100000);
						}
					}else {
						lcd_printf(ALG_LEFT, "connection was broken");
						lcd_flip();
						break;
					}
				}
				tcp_close(iclient_socket);
				iclient_socket = -1;
			}
			lcd_printf(ALG_LEFT, "Wait connection on %d...", SERVER_PORT);
			lcd_flip();
		}
		tcp_close(iserver_socket);
		iserver_socket = -1;
	}else {
		lcd_flip();
		kb_getkey();
	}
}

void socket_client_demo(void){
	int iclient_socket = -1;
	unsigned char bTemp[1024];
	unsigned int  iTemp;
	lcd_clean();
	lcd_printf(ALG_CENTER, "socket client");

	lcd_printf(ALG_LEFT, "connect to %s:%d...", SERVER_ADDR, SERVER_PORT);
	lcd_flip();
	iclient_socket = tcp_connect(SERVER_ADDR, SERVER_PORT);
	if (iclient_socket < 0){
		lcd_printf(ALG_LEFT, "Connect failed.");
		lcd_flip();
		kb_getkey();
	}else {
		while (1){
			if (kb_hit()){
				if (0x1B == kb_getkey())
					break;
			}
			strcpy((char*)bTemp, "HELLO WORLD");	
			iTemp = strlen((char*)bTemp);
			if (tcp_write(iclient_socket , bTemp, iTemp) < 0){
				lcd_printf(ALG_LEFT, "connection was broken");
				lcd_flip();
				kb_getkey();
				break;
			}else{
				usleep(500000);
				iTemp = sizeof(bTemp);
				if (tcp_read(iclient_socket, bTemp, &iTemp) < 0){
					lcd_printf(ALG_LEFT, "connection was broken");
					lcd_flip();
					kb_getkey();
					break;
				}else if (iTemp > 0){
					lcd_printf(ALG_LEFT, "Readed %d bytes", iTemp);
				}else {
										
				}
			}
		}
		tcp_close(iclient_socket);
	}
	
	
}	


#define CA_CERT     "CA.crt.pem"

#define SERVER_CERT "SERVER.crt.pem" 
#define SERVER_KEY  "SERVER.key.pem"     

#define CLIENT_CERT "CLIENT.crt.pem"
#define CLIENT_KEY  "CLIENT.key.pem"


void ssl_server_demo(void){
	int retval = 0;
	int iserver_socket = -1;
	int iclient_socket = -1;
	unsigned char bTemp[1024];
	unsigned int  iTemp = 0;
	int i;
	int active_net = 0;
	char client_addr[255];
	unsigned int client_port = 0;
	char iflist[16][IFNAMSIZ];
	struct ifreq ifr;
	int error;
		
	SSL_CTX* ctx = NULL;
	SSL* ssl = NULL;
	const SSL_METHOD *meth;

	
	lcd_clean();
	

	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms(); 
	meth = TLSv1_server_method(); 
	
	
	ctx = SSL_CTX_new (meth); 
	ssl = SSL_new (ctx); 
	
	if ((NULL == ctx) ||
		(NULL == ssl)){
		lcd_printf(ALG_LEFT, "SSL_CTX_new() or SSL_new() failed");
		lcd_flip();
		kb_getkey();
		retval = -1;
	}else{
		SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);
		SSL_CTX_set_verify_depth(ctx,1);
		
		ctx->default_passwd_callback_userdata = NULL;
		if (SSL_CTX_load_verify_locations(ctx,CA_CERT,NULL)<=0)
		{
			lcd_printf(ALG_LEFT, "SSL_CTX_load_verify_locations() failed");
			ERR_print_errors_fp(stderr);
			retval = -1;
			lcd_flip();
			kb_getkey();
		}
		else if (SSL_CTX_use_certificate_file(ctx, SERVER_CERT, SSL_FILETYPE_PEM) <= 0) {
			   lcd_printf(ALG_LEFT, "SSL_CTX_use_certificate_file() failed");
				ERR_print_errors_fp(stderr);
				retval = -1;
				lcd_flip();
				kb_getkey();
		}else if (SSL_CTX_use_PrivateKey_file(ctx, SERVER_KEY, SSL_FILETYPE_PEM) <= 0) {
			ERR_print_errors_fp(stderr);
			retval = -1;
		}else if (!SSL_CTX_check_private_key(ctx)) {
			printf("Private key does not match the certificate public key\n");
			lcd_printf(ALG_LEFT, "SSL_CTX_check_private_key() failed");
			lcd_flip();
			kb_getkey();
			retval = -1;
		}else
			SSL_CTX_set_cipher_list(ctx,"RC4-MD5"); 
	}
	
	

	if (0 == retval){
		lcd_printf(ALG_CENTER, "ssl server");
		iTemp = netif_get_list(iflist, sizeof(iflist)/IFNAMSIZ);
		for (i = 0; i < iTemp; i++){
			memset(&ifr, 0, sizeof(ifr));
			netif_get_flags(iflist[i], &ifr);
			if (ifr.ifr_flags & IFF_RUNNING){
				netif_get_ipaddr(iflist[i], &ifr);
				lcd_printf(ALG_CENTER, "Address:%s:%d", inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr), SERVER_PORT);
				active_net++;
			}
		}
		
		if (active_net == 0){
			lcd_printf(ALG_CENTER, "NO Active net work");	
			lcd_flip();
			kb_getkey();
			retval = -1;
		}
		
		lcd_flip();
	}





	if (0 == retval) {
		iserver_socket = tcp_server(SERVER_PORT, 1);
		if (iserver_socket >=0){
			lcd_printf(ALG_LEFT, "Wait connection on %d...", SERVER_PORT);
			lcd_flip();
			while (1){
				if (kb_hit()){
					if (0x1B == kb_getkey())
						break;
				}

				iclient_socket = tcp_accept(iserver_socket, client_addr, &client_port);
				if (iclient_socket < 0)
					continue;
				else {
					lcd_printf(ALG_LEFT, "Connected from %s:%d...", client_addr, client_port);
					lcd_printf(ALG_LEFT, "start to ssl hand shake...");
					SSL_set_fd (ssl, iclient_socket);
					if ( SSL_accept (ssl) < 0){
						tcp_close(iclient_socket);
						iclient_socket = -1;
						lcd_printf(ALG_LEFT, "SSL_accept() failed.");	
						lcd_flip();
						continue;
					}else if (X509_V_OK==SSL_get_verify_result(ssl)){
						int flags = fcntl(iclient_socket, F_GETFL, 0);
						fcntl(iclient_socket, F_SETFL, flags | O_NONBLOCK);

						printf("Certificate verify success\n");
						
					}else {
						printf("Certificate verify fail\n");
						tcp_close(iclient_socket);
						iclient_socket = -1;
						lcd_printf(ALG_LEFT, "SSL_get_verify_result() failed.");	
						lcd_flip();
						continue;
					}
				}
					

				//Receive and send data
				while (1){
					if (kb_hit()){
						if (0x1B == kb_getkey())
						break;
					}
					iTemp = sizeof(bTemp);
					error =  SSL_read(ssl, bTemp, iTemp);						
					if (error >= 0){
						iTemp = error;
						if (iTemp > 0)
						{
							lcd_printf(ALG_LEFT, "Readed %d bytes", iTemp);
							lcd_flip();
							error = SSL_write(ssl, bTemp, iTemp);
							if (error != 0){
								lcd_printf(ALG_LEFT, "connection was broken");
								lcd_flip();
								break;
							}
						}
						else {
							usleep(100000);
						}
					}else {
						lcd_printf(ALG_LEFT, "connection was broken");
						lcd_flip();
						break;
					}
				}
				if (iclient_socket >=0){
					tcp_close(iclient_socket);
					iclient_socket = -1;
				}
				lcd_printf(ALG_LEFT, "Wait connection on %d...", SERVER_PORT);
				lcd_flip();
			}
				

			tcp_close(iserver_socket);
			iserver_socket = -1;
		}else {
			lcd_printf(ALG_LEFT, "Create server socket faile.");
			lcd_flip();
			kb_getkey();
		}

	}

	if (NULL != ssl){
		SSL_free (ssl);
	}
		
	if (NULL != ctx){
		SSL_CTX_free (ctx);
	}
	
}
void ssl_client_demo(void){
	int retval = 0;
	int iclient_socket = -1;
	unsigned char bTemp[1024];
	unsigned int  iTemp;
	SSL_CTX* ctx = NULL;
	SSL* ssl = NULL;
	
	const SSL_METHOD *meth;
	int error;

	
	lcd_clean();
	

	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms(); 
	meth = TLSv1_client_method(); 
	
	
	ctx = SSL_CTX_new (meth); 
	ssl = SSL_new (ctx); 
	
	if ((NULL == ctx) ||
		(NULL == ssl)){
		lcd_printf(ALG_LEFT, "SSL_CTX_new() or SSL_new() failed");
		lcd_flip();
		kb_getkey();
		retval = -1;
	}else{
		SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);
		SSL_CTX_set_verify_depth(ctx,1);
		
		ctx->default_passwd_callback_userdata = NULL;
		if (SSL_CTX_load_verify_locations(ctx,CA_CERT,NULL)<=0)
		{
			lcd_printf(ALG_LEFT, "SSL_CTX_load_verify_locations() failed");
			ERR_print_errors_fp(stderr);
			retval = -1;
			lcd_flip();
			kb_getkey();
		}
		else if (SSL_CTX_use_certificate_file(ctx, CLIENT_CERT, SSL_FILETYPE_PEM) <= 0) {
			   lcd_printf(ALG_LEFT, "SSL_CTX_use_certificate_file() failed");
				ERR_print_errors_fp(stderr);
				retval = -1;
				lcd_flip();
				kb_getkey();
		}else if (SSL_CTX_use_PrivateKey_file(ctx, CLIENT_KEY, SSL_FILETYPE_PEM) <= 0) {
			ERR_print_errors_fp(stderr);
			retval = -1;
		}else if (!SSL_CTX_check_private_key(ctx)) {
			printf("Private key does not match the certificate public key\n");
			lcd_printf(ALG_LEFT, "SSL_CTX_check_private_key() failed");
			lcd_flip();
			kb_getkey();
			retval = -1;
		}else
			SSL_CTX_set_cipher_list(ctx,"RC4-MD5"); 
	}

	if (0 == retval){
		lcd_clean();
		lcd_printf(ALG_CENTER, "ssl client");

		lcd_printf(ALG_LEFT, "connect to %s:%d...", SERVER_ADDR, SERVER_PORT);
		lcd_flip();
		iclient_socket = tcp_connect(SERVER_ADDR, SERVER_PORT);
		if (iclient_socket < 0){
			lcd_printf(ALG_LEFT, "Connect failed.");
			lcd_flip();
			kb_getkey();
			retval = -1;
		}else {
			lcd_printf(ALG_LEFT, "start to ssl hand shake...");
			SSL_set_fd (ssl, iclient_socket);
			if ( SSL_connect (ssl) < 0){
				tcp_close(iclient_socket);
				iclient_socket = -1;
				lcd_printf(ALG_LEFT, "SSL_connect() failed.");	
				lcd_flip();
				retval = -1;
				kb_getkey();
			}else if (X509_V_OK==SSL_get_verify_result(ssl)){
				int flags = fcntl(iclient_socket, F_GETFL, 0);
				fcntl(iclient_socket, F_SETFL, flags | O_NONBLOCK);

				printf("Certificate verify success\n");
				
			}else {
				printf("Certificate verify fail\n");
				tcp_close(iclient_socket);
				iclient_socket = -1;
				lcd_printf(ALG_LEFT, "SSL_get_verify_result() failed.");	
				lcd_flip();
				retval = -1;
				kb_getkey();
			}
		}
		

		while (0 == retval){
			if (kb_hit()){
				if (0x1B == kb_getkey())
					break;
			}
			strcpy((char*)bTemp, "HELLO WORLD");	
			iTemp = strlen((char*)bTemp);
			if (SSL_write(ssl , bTemp, iTemp) < 0){
				lcd_printf(ALG_LEFT, "connection was broken");
				lcd_flip();
				kb_getkey();
				break;
			}else{
				usleep(500000);
				iTemp = sizeof(bTemp);
				error = SSL_read(ssl, bTemp, iTemp);
				if (error < 0){
					lcd_printf(ALG_LEFT, "connection was broken");
					lcd_flip();
					kb_getkey();
					break;
				}else if (error > 0){
					lcd_printf(ALG_LEFT, "Readed %d bytes", error);
				}else {
										
				}
			}
		}
		tcp_close(iclient_socket);
		iclient_socket = -1;
	}
	if (NULL != ssl){
		SSL_free (ssl);
	}
		
	if (NULL != ctx){
		SSL_CTX_free (ctx);
	}
}


void tcp_init(void)
{
	static int inited = 0;
	if (!inited){
		signal(SIGPIPE, SIG_IGN);
		inited = 1;
	}
}


int  tcp_server(unsigned int uiPort, unsigned int MaxConnect)
{
	#define MAX_SOCKETS 1000
	int isocket = -1;
	struct sockaddr_in  servaddr;
	tcp_init();
	if (MaxConnect == 0)
		MaxConnect = MAX_SOCKETS;
	isocket = socket(AF_INET, SOCK_STREAM, 0);
	if (isocket >= 0){
		 memset(&servaddr, 0, sizeof(servaddr));    
		 servaddr.sin_family = AF_INET;    
		 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);    
		 servaddr.sin_port = htons(uiPort);    
		 if( bind(isocket, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		 	tcp_close(isocket);
		 	isocket = -2;
		 }else if( listen(isocket,MaxConnect ) == -1){
			tcp_close(isocket);
			isocket = -3;
		 }
	}		
	return isocket;
}

int tcp_accept(int iSvrSocket, char szClientIP[16], unsigned int * puiClientPort)
{
	int clientSocket = -1;
	struct timeval tv;
     struct sockaddr_in addr;
     #if defined(WIN32)
        int addrlen;
     #else
    unsigned int addrlen;
    #endif
	fd_set r;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;
	FD_ZERO(&r);
	FD_SET(iSvrSocket, &r);
	if (select(iSvrSocket + 1, &r, NULL, NULL, &tv)>0)
	{
        addrlen = sizeof(struct sockaddr);

		clientSocket=accept(iSvrSocket,  (struct sockaddr *)&addr, &addrlen);
        if (clientSocket >=0)
        {
            if (szClientIP != NULL)
            {
                	sprintf(szClientIP, inet_ntoa( addr.sin_addr));
            }
            if (NULL != puiClientPort)
            {
               	*puiClientPort = ntohs(addr.sin_port);
            }
        }
	}
	return clientSocket;
}

int tcp_connect(const char * pServerAddr, unsigned int port)
{
	int clientsocket = -1;
	struct sockaddr_in addr;
	struct hostent *pHE;
	tcp_init();
	clientsocket = socket(AF_INET,SOCK_STREAM,0);
	if (-1 != clientsocket){

			memset(&addr, 0, sizeof(addr));
			addr.sin_addr.s_addr=inet_addr(pServerAddr);
			if (INADDR_NONE == addr.sin_addr.s_addr)
			{
				pHE=gethostbyname(pServerAddr);
				if(pHE)
				{
					addr.sin_addr.s_addr = *(u_long *) pHE->h_addr_list[0];
				}
			}

			addr.sin_family=AF_INET;
			addr.sin_port=htons(port);

			if(connect(clientsocket,(struct sockaddr *)&addr, sizeof(addr))==0)
			{

			}
			else
			{
				tcp_close(clientsocket);
				clientsocket = -1;
			}

	}
	return clientsocket;
}

void   tcp_close(int isocket)
{
	if (isocket >=0)
		close(isocket);
}

int tcp_read(int isocket, void * pvbuff, unsigned int * pisize)
{
	int retval = 0;
	int error;
	struct timeval tv;
	fd_set r;


	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	FD_ZERO(&r);
	FD_SET(isocket, &r);
	error= select(isocket+1,&r,NULL, NULL, &tv);
	
	if ( error > 0)
	{
		
		if (*pisize > 0)
		{
			error =  recv(isocket,(char*)pvbuff, *pisize, 0);
			if (error > 0)
			{
				*pisize = (unsigned int)error;
			}
			else
			{
				retval = -1;
			}
		}
	}
	else if (0 != error)
	{
		retval = -1;
	}
	else
    {
		*pisize = 0;
        retval = 0;
    }

	return retval;
}

int tcp_write(int isocket, const void * pvdata, unsigned int isize)
{
	int retval = 0;
	struct timeval tv;
	fd_set r;
	
  	tv.tv_sec = 0;
	tv.tv_usec = 1000;
	FD_ZERO(&r);
	FD_SET(isocket, &r);

    if (select(isocket+1,NULL,&r,NULL,&tv) > 0)
    {
        if(isize != (unsigned int)send(isocket,(char*)pvdata, isize, 0))
		{
			retval = -1;
		}
    }
    else
	{
	   retval = -1;
	}

	return retval;
}


