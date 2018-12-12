/*
 * network_f.c
 *
 *  Created on: Oct 25, 2014
 *      Author: tarus
 */

#include <stdio.h>    //printf
#include <string.h>   //strncpy
#include <sys/socket.h>
#include <time.h>
#include <sys/ioctl.h>
#include <net/if.h>   //ifreq
#include <unistd.h>   //close
#include <stdlib.h>
#include "jsonread.h"
#include <posapi.h>
#include <arpa/inet.h>
#include "ppp.h"
#include "../src/utilities/send_online_request.h"
#include "../src/utilities/keyboart.h"
#include "../src/utilities/lcd.h"


void change_password(void );
int getRandoms();
extern int kb_getString(uint8_t ucMode, uint8_t ucMinlen, uint8_t ucMaxlen,
		char *pszOut, uint8_t ucTimeOut, const char *ch);

int funt_get_mac_address() {
	int fd;
	struct ifreq ifr;
	char *iface = "eth0";
	unsigned char *mac;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);

	ioctl(fd, SIOCGIFHWADDR, &ifr);

	close(fd);

	mac = (unsigned char *) ifr.ifr_hwaddr.sa_data;

	//display mac address
	//sprintf(mac_address, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", mac[0], mac[1],
	//		mac[2], mac[3], mac[4], mac[5]);
	sprintf(mac_address,"%s","00:25:7E:01:C1:A0");
	/*sprintf(mac_address1, "%.2X%.2X%.2X%.2X%.2X%.2X", mac[0], mac[1], mac[2],
			mac[3], mac[4], mac[5]);
*/
	//sprintf();
	memset(serial_num,0,sizeof(serial_num)+1);
	sys_get_sn(serial_num,100);
	printf("Serial num : %s\n\n\n\n\n\n\n", serial_num);
	sprintf(mac_address,"%s",serial_num);
	//strcpy(serial_num,"sn : ")

	return 0;
}

char *getdate_mine(void) {
	int SIZE = 256;
	char timebuffer[SIZE];
	char buffer[256];
	time_t curtime;
	struct tm *loctime;
	/* Get the current time. */
	curtime = time(NULL);
	/* Convert it to local time representation. */
	loctime = localtime(&curtime);
	/* Print out the date and time in the standard format. */
	fputs(asctime(loctime), stdout);
	memset(timebuffer, 0, sizeof(timebuffer));
	memset(buffer, 0, sizeof(buffer));
	strftime(time_for_receipt, SIZE, "%y%m%d", loctime);
	strftime(timebuffer, SIZE, "%Y-%m-%d %H:%M:%S", loctime);
	return timebuffer;
}

char *getdate_mine_for_transaction(char * rcpt_num_entered) {


	int SIZE = 256;
	char timebuffer[SIZE];
	char buffer[256];
	char rnum[256];
	time_t curtime;
	struct tm *loctime;
	/* Get the current time. */
	curtime = time(NULL);
	/* Convert it to local time representation. */
	loctime = localtime(&curtime);
	//getRandoms();
	/* Print out the date and time in the standard format. */
	fputs(asctime(loctime), stdout);
	memset(timebuffer, 0, sizeof(timebuffer));
	memset(buffer, 0, sizeof(buffer));
//	strftime(time_for_receipt, SIZE, "%y%m%d", loctime);

	if(strlen(rcpt_num_entered)>0)
	{
		strftime(timebuffer, SIZE, "%y%m%d-", loctime);
		strcat(timebuffer,rcpt_num_entered );
	}
	else
		strftime(timebuffer, SIZE, "%y%m%d-%H%M", loctime);
		sprintf(rnum, "%d",getRandoms());
		printf("###randoms# %s", rnum);
		sprintf(buffer,"%s%s%s",serial_num,timebuffer, rnum);

		strcpy(recptnum, buffer);
	return recptnum;
}

int getRandoms(){

	srand(time(NULL));//calls srand only once
	int lower=100, upper=999;


		return lower+rand()%(upper-lower);
	}

char *getdate_mine_for_z(void) {
	int SIZE = 256;
	char timebuffer[SIZE];
	char buffer[256];
	time_t curtime;
	struct tm *loctime;
	/* Get the current time. */
	curtime = time(NULL);
	/* Convert it to local time representation. */
	loctime = localtime(&curtime);
	/* Print out the date and time in the standard format. */
	fputs(asctime(loctime), stdout);
	memset(timebuffer, 0, sizeof(timebuffer));
	memset(buffer, 0, sizeof(buffer));
	strftime(time_for_receipt, SIZE, "%y%m%d", loctime);
	strftime(timebuffer, SIZE, "%y%m%d%H%M%S", loctime);
	sprintf(buffer,"%s%s",serial_num,timebuffer);
	return buffer;
}

//Function used to get current time in Good formate
void get_current_time(void) {

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1,
			tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

}

int read_config() {
	FILE* config_fp;
	char line[1024 + 1];
	char *s = 0;
	int num = 0;
	char name[1024 - 1], value[1024 - 1];
	int counter;
	char buffer_int[100];
	int changes_made =0;
/*	FILE *f;
	//system("mv config.cfg config.cfg.bak");
	f = fopen("config.cfg", "w");*/
	myConfigurations = (ConfigFile *) malloc(sizeof(ConfigFile));

	if (!myConfigurations)
		return -1;
	config_fp = fopen("config.cfg", "r");

	if (config_fp == NULL)
	{
		printf("Error Openning the file\n");
		set_innitial_configuration();
		return 1;
	}
	counter = 0;
	while (fgets(line, 1024, config_fp) != NULL) {
		num++;
		printf("line : %d :%s\n", num, line);
		//memset(name, 0, sizeof(name));
		//memset(value, 0, sizeof(value));
		if (line[0] == '\n' || line[0] == '#')
			if (line[0] == '#')
				continue;
		s = strtok(line, "=");
		if (s == NULL)
			continue;
		else
			strcpy(name, s);
		//trim(name);

		s = strtok(NULL, "=");
		if (s == NULL)
			continue;
		else
			strcpy(value, s);
		//trim(value);

		if (num < 100)
			printf("%d %s\n %s\n", num, name, value);
		strcpy(value,trim(value));

		if (strcmp(name, "IpAddress") == 0)
		{

			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "IP address must be set");
				lcd_printf(ALG_CENTER, "Please enter IP address");
				lcd_flip();
				kb_getkey();
				change_configuration(0);
				changes_made =1;
			}
			else if(!isValidIpAddress(value))
			{

				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid IP address");
				lcd_printf(ALG_CENTER, "Please change IP address");
				lcd_flip();
				kb_getkey();
				change_configuration(0);
				changes_made =1;
			}
			else
			strcpy(myConfigurations->IpAddress, value);

		}

		else if (strcmp(name, "portNumber") == 0)
			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Port number must be set");
				lcd_printf(ALG_CENTER, "Please enter IP address");
				lcd_flip();
				kb_getkey();
				change_configuration(1);
				changes_made =1;
			}
			else if(!is_valid_int((value)))
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid port number");
				lcd_printf(ALG_CENTER, "Please change port number");
				lcd_flip();
				kb_getkey();
				change_configuration(1);
				changes_made =1;
			}
			else
			strcpy(myConfigurations->portNumber, value);

		else if (strcmp(name, "apn_username") == 0)
		{
			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "APN username must be set");
				lcd_printf(ALG_CENTER, "Please configure APN username");
				lcd_flip();
				kb_getkey();
				change_configuration(2);
				changes_made =1;
			}

			else
				strcpy(myConfigurations->apn_username, value);
		}

		else if (strcmp(name, "apn_password") == 0)
		{
/*			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "APN password must be set");
				lcd_printf(ALG_CENTER, "Please configure APN password");
				lcd_flip();
				kb_getkey();
				change_configuration(3);
				changes_made =1;
			}
			else
			strcpy(myConfigurations->apn_password, value);*/

			strcpy(myConfigurations->apn_password, value);
		}

		else if (strcmp(name, "ppp_timeout") == 0)
		{
			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Port number must be set");
				lcd_printf(ALG_CENTER, "Please enter IP address");
				lcd_flip();
				kb_getkey();
				change_configuration(4);
				changes_made =1;
			}
			else if(!is_valid_int((value)))
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid port number");
				lcd_printf(ALG_CENTER, "Please change port number");
				lcd_flip();
				kb_getkey();
				change_configuration(4);
				changes_made =1;
			}
			else
			strcpy(myConfigurations->ppp_timeout, value);
		}

		else
			printf("WARNING: %s/%s: Unknown name/value pair!\n", name, value);
	}

	if(changes_made){
		save_configarations();
	}

	strcpy(url ,"http://" );
	snprintf(buffer_int,strlen(myConfigurations->IpAddress)+1 ,myConfigurations->IpAddress );
	strcat(url , buffer_int);
	snprintf(buffer_int,strlen(myConfigurations->portNumber)+1 ,myConfigurations->portNumber );
	strcat(url ,":");
	strcat(url , buffer_int);
	//strcat(url , "/victoriaswitch/rest/service/");
	strcat(url , "/fkfapi/rest/service/");
	printf("The URL : %s \n",url);
	return 0;
}

void save_configarations() {
	FILE *f;
	system("mv config.cfg config.cfg.bak");
	f = fopen("config.cfg", "w");
	if (f) {
		fprintf(f, "IpAddress=%s\n", myConfigurations->IpAddress);
		fprintf(f, "apn_username=%s\n", myConfigurations->apn_username);
		fprintf(f, "apn_password=%s\n", myConfigurations->apn_password);
		fprintf(f, "portNumber=%s\n", myConfigurations->portNumber);
		fprintf(f, "ppp_timeout=%s\n", myConfigurations->ppp_timeout);

		fclose(f);
		strcpy(url ,"");
		strcpy(url ,"http://" );
		strcat(url , myConfigurations->IpAddress);
		strcat(url ,":");
		strcat(url , myConfigurations->portNumber);
		//strcat(url , "/victoriaswitch/rest/service/$$");
		//strcat(url , "/victoriaswitch/rest/service/$$");http://197.220.114.46:8121/fkfapi/rest/service/
		strcat(url , "/fkfapi/rest/service/");
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER,"Configurations change effected successfully. \nPress any key to continue");

		lcd_flip();
		kb_getkey();
	}
}
void change_configuration(int type) {
	int selected , ret;
	int change_made =0;
	int ret_val;
	const char menu[][100] = { "Server IP Address   ", "Server port number  ",
			"Network APN username", "Network APN password",
			"Network ppp Timeout " , "Turn Online On/Off" };
	const char menu_on_off[][100] = { "Online Mode  ", "Offline Mode "};
	char getCharacters[40],  getCharacters1[40];
	char name[100],  name1[100];
	if(type == -1)
	selected = lcd_menu("Network Settings", menu, sizeof(menu) / 100, selected);
	else
		selected = type;
	switch (selected) {
	case 0:
		do
		{
		strcpy(name, "Enter IP address");
		strcpy(name1, "Current Ip address");
		strcpy(getCharacters1,myConfigurations->IpAddress);
		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

		if( isValidIpAddress((getCharacters)))
		{
			change_made =1;
			strcpy(myConfigurations->IpAddress, (getCharacters));
		}

		else if(ret==-1 && strlen((myConfigurations->IpAddress))>0 )
			break;
		else if(ret==-1 && strlen((myConfigurations->IpAddress)) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "IP address must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid IP address");
			lcd_printf(ALG_CENTER, "Please check IP\n and try again");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen((myConfigurations->IpAddress))==0 );

		break;
	case 1: {
		do
		{
			strcpy(name, "Enter port number");
			strcpy(name1, "Current port number");
			strcpy(getCharacters1,myConfigurations->portNumber);
			ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if( is_valid_int(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->portNumber, getCharacters);
		}
		else if(ret==-1 &&  strlen(myConfigurations->portNumber)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->portNumber) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Port number must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid port number");
			lcd_printf(ALG_CENTER, "Please enter a numeric value");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen(myConfigurations->portNumber)==0 );
	}
		break;

	case 2: {

		do
		{
			strcpy(name, "Enter APN username");
			strcpy(name1, "Current APN username");
			strcpy(getCharacters1,myConfigurations->apn_username);
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if(ret !=-1 )
		{
			change_made =1;
			strcpy(myConfigurations->apn_username, getCharacters);
		}

		if(ret==-1 &&strlen(myConfigurations->apn_username)>0 )
		{
			break;
		}
		else if(ret==-1 &&strlen(myConfigurations->apn_username) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "APN username must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}

		}while(strlen(myConfigurations->apn_username)==0 );

	}
		break;

	case 3: {

		do
		{
		strcpy(name, "Enter APN password");
				strcpy(name1, "Current APN password");
				strcpy(getCharacters1,myConfigurations->apn_password);
				ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

		if(ret != -1)
		{
			if(strlen((getCharacters)) == 0)
				continue;
			else{
				change_made =1;
				strcpy(myConfigurations->apn_password, getCharacters);
				break;
			}

		}

		else if(ret==-1 && strlen(myConfigurations->apn_password)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->apn_password) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "APN password must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}

		}while(strlen(myConfigurations->apn_password)==0 );
	}
		break;

	case 4: {

		do
		{
			strcpy(name, "Enter IP PPP time out");
			strcpy(name1, "Current PPP time out");
			strcpy(getCharacters1,myConfigurations->ppp_timeout);
			ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

		if( is_valid_int(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->ppp_timeout, getCharacters);
		}
		else if(ret==-1 && strlen(myConfigurations->ppp_timeout)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->ppp_timeout) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "PPP timeout number must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid PPP timeout value");
			lcd_printf(ALG_CENTER, "Please enter a numeric value");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen(myConfigurations->ppp_timeout)==0 );
	}

		break;
	case 5: {
		selected = lcd_menu("Network Operation Mode", menu_on_off, sizeof(menu_on_off) / 100, selected);
		if(selected == -1)
		{

		}
		else if(selected == 0)
		{
			ret_val = ppp_check("/var/mux1");
			if (0 == ret_val) {
				flag_online = 1;
				flag_offline_login = 0;
				login_successful = 0 ;
				logged_offline = 0;
				message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
				kb_getkey();
				return ;
			}

			fag_start_ppp_session = 1;
			flag_online = 0;
			flag_offline_login = 0;
			login_successful = 0 ;
			logged_offline = 0;
			power_on_modem_device(myConfigurations->apn_username,myConfigurations->apn_password,myConfigurations->ppp_timeout);
			if(flag_online)
			{
				message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
				kb_getkey();


			}
			else
			{
				message_display_function(1,"","Network Mode  ", "The POS shall operate in can not operate in Online mode. Please check your connectivity and try again", (char *)NULL);
				kb_getkey();
			}
			return ;
		}
		else if(selected == 1)
		{
			message_display_function(1,"","Network Mode  ", "The POS shall operate in offline mode .", (char *)NULL);
			kb_getkey();
			flag_offline_login = 0;
			login_successful = 0 ;
			logged_offline = 0;
			flag_online = 0;
			return ;
		}
	}
	break;
	default:
		break;

	}
	if(type == -1)
	{
		if(change_made)
			save_configarations();
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "No configuration have \n been changed");

			lcd_flip();
			kb_getkey();
		}
	}


}

void change_password() {
	int selected;
	int change_made =0;
	const char menu[][100] = { "Server IP Address   ", "Server port number  ",
			"Network APN username", "Network APN password",
			"Network ppp Timeout " };
	char getCharacters[40];
	char name[100];
	selected = lcd_menu("Network Settings", menu, sizeof(menu) / 100, selected);
	switch (selected) {
	case 0:
		do
		{
		strcpy(name, "Enter IP address");
		kb_getString(ALPHA_IN, 1, 16,getCharacters , NULL, name);
		if( isValidIpAddress(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->IpAddress, getCharacters);
		}

		else if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->IpAddress)>0 )
		{
			break;
		}
		else if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->IpAddress) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Ip addrees must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid IP address");
			lcd_printf(ALG_CENTER, "Please check IP\n and try again");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen(myConfigurations->IpAddress)==0 );

		break;
	case 1: {
		do
		{
			strcpy(name, "Enter port number");
		kb_getString(ALPHA_IN, 1, 16,getCharacters , NULL, name);
		if( is_valid_int(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->portNumber, getCharacters);
		}
		else if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->portNumber)>0 )
		{
			break;
		}
		else if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->portNumber) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Port number must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid port number");
			lcd_printf(ALG_CENTER, "Please enter a numeric value");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen(myConfigurations->portNumber)==0 );
	}
		break;

	case 2: {

		do
		{
			strcpy(name, "Enter APN username");
		kb_getString(ALPHA_IN, 1, 16,getCharacters , NULL, name);
		if(strcmp(getCharacters,"CANCEL_PRESSED")!=0)
		{
			change_made =1;
			strcpy(myConfigurations->apn_username, getCharacters);
		}

		if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->apn_username)>0 )
		{
			break;
		}
		else if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->apn_username) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "APN username must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}

		}while(strlen(myConfigurations->apn_username)==0 );

	}
		break;

	case 3: {
		strcpy(name, "Enter APN password");
		do
		{
		kb_getString(ALPHA_IN, 1, 16,getCharacters , NULL, name);
		if(strcmp(getCharacters,"CANCEL_PRESSED")!=0)
		{
			change_made =1;
			strcpy(myConfigurations->apn_username, getCharacters);
		}

		if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->apn_password)>0 )
		{
			break;
		}
		else if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->apn_password) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "APN password must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}

		}while(strlen(myConfigurations->apn_password)==0 );
	}
		break;

	case 4: {
		strcpy(name, "Enter IP PPP time out");
		do
		{
		kb_getString(ALPHA_IN, 1, 16,getCharacters , NULL, name);
		if( is_valid_int(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->ppp_timeout, getCharacters);
		}
		else if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->ppp_timeout)>0 )
		{
			break;
		}
		else if(strcmp(getCharacters,"CANCEL_PRESSED")==0 && strlen(myConfigurations->ppp_timeout) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "PPP timeout number must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid PPP timeout value");
			lcd_printf(ALG_CENTER, "Please enter a numeric value");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen(myConfigurations->ppp_timeout)==0 );
	}

		break;
	default:
		break;

	}
if(change_made)
	save_configarations();
else
{
	lcd_clean();
	screen_header();
	lcd_printf(ALG_CENTER, "No configuration have \n been changed");

	lcd_flip();
	kb_getkey();
}

}

int isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result;
}
int isValidAmmount(int *ammount)
{
   if(ammount>0)

	   return ammount;
}

bool is_valid_int(const char *str)
{
   // Handle negative numbers.
   //
   if (*str == '-')
      ++str;

   // Handle empty string or just "-".
   //
   if (!*str)
      return false;

   // Check for non-digit chars in the rest of the string.
   //
   while (*str)
   {
      if (!isdigit(*str))
         return false;
      else
         ++str;
   }

   return true;
}

bool is_valid_positive_int(const char *str)
{
   // Handle negative numbers.
   //
  /* if (*str == '-')
      ++str;*/

   // Handle empty string or just "-".
   //
   if (!*str)
      return false;

   // Check for non-digit chars in the rest of the string.
   //
   while (*str)
   {
      if (!isdigit(*str))
         return false;
      else
         ++str;
   }

   return true;
}

void set_innitial_configuratio() {
	int selected , ret;
	int change_made =0;

	char getCharacters[40];
	char getCharacters1[40];
	char name1[30];
	strcpy(name1, "");
	char name[100];
//Setting IP address
		do
		{
		strcpy(name, "Enter First Name");
		//kb_getString(ALPHA_IN, 1, 16,getCharacters , NULL, name);
		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Regester Beneficiary", 1);

		if(! strcmp(getCharacters,""))
		{
			change_made =1;
			strcpy(myBenf.firstname, getCharacters);
		}

		else if(ret==-1 && strlen(myBenf.firstname)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myBenf.firstname) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Ip address must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid IP address");
			lcd_printf(ALG_CENTER, "Please check IP\n and try again");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen(myBenf.firstname)==0 );

//Setting port number
		do
		{
			strcpy(name, "Enter port number");
			ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if( is_valid_int(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->portNumber, getCharacters);
		}
		else if(ret==-1 && strlen(myConfigurations->portNumber)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->portNumber) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Port number must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid port number");
			lcd_printf(ALG_CENTER, "Please enter a numeric value");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen(myConfigurations->portNumber)==0 );

//Setting APN username
		do
		{
			strcpy(name, "Enter APN username");
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if(ret !=-1)
		{
			change_made =1;
			strcpy(myConfigurations->apn_username, getCharacters);
		}

		if(ret==-1 && strlen(myConfigurations->apn_username)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->apn_username) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "APN username must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}

		printf("in username\n");
		}while(strlen(myConfigurations->apn_username)==0 );


		printf("After username\n");
	//Setting  APN password
		strcpy(name, "Enter APN password");
		do
		{
			printf("in password\n");
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if(ret != -1)
		{
			change_made =1;
			strcpy(myConfigurations->apn_password, getCharacters);
		}

		if(ret==-1 && strlen(myConfigurations->apn_password)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->apn_password) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "APN password must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}

		}while(strlen(myConfigurations->apn_password)==0 );
		printf("after username\n");

		//setting PP timeout

		strcpy(name, "Enter IP PPP time out");
		do
		{
			ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if( is_valid_int(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->ppp_timeout, getCharacters);
		}
		else if(ret==-1 && strlen(myConfigurations->ppp_timeout)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->ppp_timeout) == 0 )
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "PPP timeout number must be set");
			lcd_printf(ALG_CENTER, "Please try again");
			lcd_flip();
			kb_getkey();
		}
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "Invalid PPP timeout value");
			lcd_printf(ALG_CENTER, "Please enter a numeric value");
			lcd_flip();
			kb_getkey();
		}
		}while(strlen(myConfigurations->ppp_timeout)==0 );


	if(change_made)
		save_configarations();
	else
	{
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER, "No configuration have \n been changed");

		lcd_flip();
		kb_getkey();
	}


}
char *trim(char *s)
{
    size_t size;
    char *end;

    size = strlen(s);

    if (!size)
    	return s;

    end = s + size - 1;
    while (end >= s && isspace(*end))
    	end--;
    *(end + 1) = '\0';

    while (*s && isspace(*s))
    	s++;

    return s;
}
