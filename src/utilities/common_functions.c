/*
 * split_string.c
 *
 *  Created on: Dec 8, 2018
 *      Author: linux
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common_functions.h"
char** str_split(char* a_str, const char a_delim)
{
	char** result    = 0;
	size_t count     = 0;
	char* tmp        = a_str;
	char* last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	/* Count how many elements will be extracted. */
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	/* Add space for trailing token. */
	count += last_comma < (a_str + strlen(a_str) - 1);

	/* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char*) * count);

	if (result)
	{
		size_t idx  = 0;
		char* token = strtok(a_str, delim);

		while (token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
}



char **  pointer_array_copy(char ** original_pointer ,int i ,char * str){
	original_pointer = realloc(original_pointer, (i + 1) * sizeof(char *));
	original_pointer[i] = malloc(strlen(str) + 1);
	strcpy(original_pointer[i], str);
	return original_pointer;
}

char ** get_pos_network_data(char * command , int * x){
	char buf[1000];
	FILE *output;
	char **outputdata = 0;
	int i = 0;

	output= popen(command, "r");
	while (fgets(buf, sizeof(buf), output) != 0) {
		outputdata = pointer_array_copy(outputdata , i ,buf);
		i++;
	}
	*x = i;
	pclose(output);
	return outputdata;
}


//





/// Device Services
/*int funt_get_mac_address() {
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
	sprintf(mac_address,"%s","00:25:7E:01:C1:A0");
	//sprintf();
	memset(serial_num,0,sizeof(serial_num)+1);
	sys_get_sn(serial_num,100);
	printf("Serial num : %s\n\n\n\n\n\n\n", serial_num);
	sprintf(mac_address,"%s",serial_num);
	//strcpy(serial_num,"sn : ")

	return 0;
}*/

void get_date_and_receipt (int required_output,  char ** returned_date , char ** returned_value) {
	int SIZE = 256;
	char timebuffer[SIZE];
	char id_buffer[256];
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
	//Beneficiary Transaction
	if(required_output == 0){
		strftime(buffer, SIZE, "%y%m%d%H%M%S",  loctime);
		sprintf(id_buffer ,"BT-%s-%s" ,pos_serial_number ,buffer );
	}
	//Merchant Transaction
	if(required_output == 1){
		strftime(buffer, SIZE, "%y%m%d%H%M%S", loctime);
		sprintf(id_buffer ,"MT-%s-%s" ,pos_serial_number ,buffer );
	}
	*returned_value  = id_buffer;

	strftime(timebuffer, SIZE, "%Y-%m-%d %H:%M:%S", loctime);
	*returned_date  = timebuffer;

}





char ** getcollumndata(int collumn_number , int dataCount , int collums_in_rs , char  db_values[])
{
	char ** collumn_data = 0;
	int i = 0 , x=0;
	if(dataCount==0)
		return 0;
	else
	{
		i = collumn_number-1;
		while(i < dataCount){
			collumn_data = realloc(collumn_data, (  x + 1 ) * sizeof( char * ));
			collumn_data[x] = malloc( strlen(db_values[i]) + 1 );
			strcpy( collumn_data[x], db_values[i]);
			i = i + collums_in_rs;
			x++;
		}

		return collumn_data;
	}

}


char ** getrowofdata(int rowNumber , int dataCount , int collums_in_rs , char  db_values[]){
	int i = 0, x = 0;
	char ** rowdata = 0;
	rowNumber = rowNumber-1;
	if(rowNumber< 0 || rowNumber>=dataCount){
		return 0;
	}
	else{

		for (i = rowNumber; i < (rowNumber+collums_in_rs); i++) {
			printf("%d\n",i);
			printf(db_values[i]);
			printf("\n\n");
			rowdata = realloc(rowdata, (  x + 1 ) * sizeof( char * ));
			rowdata[x] = malloc( strlen(db_values[i]) + 1 );
			strcpy( rowdata[x], db_values[i]);
			x ++;
		}
		return rowdata;
	}

}




char * get_string_from_jason_object(char * string){
	char * out;

/*	if(string != 0)
	{
		return "";
	}*/
/*	if(strlen(string) > 0)
	{
		return "";
	}*/
	out = malloc(strlen(string));
	trim(string);
	if(strlen(string) >2)
	snprintf(out , strlen(string)-1, "%s",string +1);
	else
		return string;

	return out;
}
