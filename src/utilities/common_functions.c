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
#include "JSON_checker.h"
#include "math.h"
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

void get_date_and_receipt (int required_output,  char ** returned_date , char ** unformatted_date , char ** returned_value) {
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
	memset(buffer, 0, sizeof(buffer));
	strftime(buffer, SIZE, "%Y%m%d", loctime);
	*unformatted_date = buffer;
	*returned_value  = id_buffer;

	printf("unformatted_date : %s\n" , *unformatted_date);
	strftime(timebuffer, SIZE, "%Y-%m-%d %H:%M:%S", loctime);
	*returned_date  = timebuffer;

}



void get_request_id (int required_output,  char ** returned_request_id) {
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
		strftime(buffer, SIZE, "%y%m%d%H%M%S",  loctime);
		sprintf(id_buffer ,"REQ-%s-%s" ,pos_serial_number ,buffer );

	memset(buffer, 0, sizeof(buffer));
	strftime(buffer, SIZE, "%Y%m%d", loctime);
//	*unformatted_date = buffer;
	*returned_request_id  = id_buffer;
	printf("Val :  %s" , *returned_request_id);



}





char ** getcollumndata(int collumn_number , int dataCount , int collums_in_rs , char * db_values[])
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


char ** getrowofdata(int rowNumber , int dataCount , int collums_in_rs , char * db_values[]){
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
		return "";

	return out;
}




#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define DICT_LEN 256

int *create_delim_dict(char *delim)
{
    int *d = (int*)malloc(sizeof(int)*DICT_LEN);
    int i;
    memset((void*)d, 0, sizeof(int)*DICT_LEN);


    for(i=0; i< strlen(delim); i++) {
        d[delim[i]] = 1;
    }
    return d;
}



char *my_strtok(char *str, char *delim)
{

    static char *last, *to_free;
    int *deli_dict = create_delim_dict(delim);

    if(!deli_dict) {
        return NULL;
    }

    if(str) {
        last = (char*)malloc(strlen(str)+1);
        if(!last) {
            free(deli_dict);
        }
        to_free = last;
        strcpy(last, str);
    }

    while(deli_dict[*last] && *last != '\0') {
        last++;
    }
    str = last;
    if(*last == '\0') {
        free(deli_dict);
        free(to_free);
        return NULL;
    }
    while (*last != '\0' && !deli_dict[*last]) {
        last++;
    }

    *last = '\0';
    last++;

    free(deli_dict);
    return str;
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


char * remove_quotes(char * string){
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

int jcheck(char* mystr){
 /*
     Read STDIN. Exit with a message if the input is not well-formed JSON text.

     jc will contain a JSON_checker with a maximum depth of 20.
     {"This":45, "and":97}
  */
 JSON_checker jc = new_JSON_checker(20);
 //char * mystr;
 //malloc();
 int c =0;
 for (;;) {
  int next_char = mystr[c];

  if (next_char <= 0) {
   break;
  }
  // printf("%d",c);
  if (!JSON_checker_char(jc, next_char)) {
   fprintf(stderr, "JSON_checker_char: syntax error\n");
   //exit(1);
   return 0;
   break;
  }

  else{
   //printf("json\n");
  }

  c++;

 }

 if (!JSON_checker_done(jc))
 {
  fprintf(stderr, "JSON_checker_end: syntax error\n");
  //exit(1);
  return 0;
 }
 else{
  printf("Good json\n");
  return 1;
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



char *hextodecimalconverter(char *hexstring){
	  int i;
	  int j = 0;
	  double x = 0;
	  char output[16];
	  char iccid[16];
	  printf("%s\n",hexstring);
	  printf("%d\n",(int)strlen(hexstring));
	  for(i=strlen(hexstring)-1;i>=0;i--){
	    x += getdecimalvalueforhex(hexstring[i],j);
	    j++;
	  }
	  printf("decimal value %f\n",x);
	  sprintf(output,"%f",x);
	  printf("%s\n",strtok(output,"."));
	  if(strlen(strtok(output,".")) == 8){
		  sprintf(iccid,"21100000%s",strtok(output,"."));
		  printf("Final iccid %s",iccid);
		  return iccid;
	  }
	  return strtok(output,".");
}
double getdecimalvalueforhex(char i,int position){

	  printf("%c %d\n",i,position);
	  switch(i){
	    case '0':
	      return (0*pow(16,position));
	    case '1':
	      return (1*pow(16,position));
	    case '2':
	      return (2*pow(16,position));
	    case '3':
	      return (3*pow(16,position));
	    case '4':
	      return (4*pow(16,position));
	    case '5':
	      return (5*pow(16,position));
	    case '6':
	      return (6*pow(16,position));
	    case '7':
	     return (7*pow(16,position));
	    case '8':
	      return (8*pow(16,position));
	    case '9':
	      return (9*pow(16,position));
	    case 'A':
	      return (10*pow(16,position));
	    case 'B':
	      return (11*pow(16,position));
	    case 'C':
	      return (12*pow(16,position));
	    case 'D':
	      return (13*pow(16,position));
	    case 'E':
	      return (14*pow(16,position));
	    case 'F':
	      return (15*pow(16,position));
	    default:
	      printf("invalid character passed\n");
	      return 0;

	  }

}


void pretty_printf(char  * str ,  int print_length)
{
	if(print_length  > 0 )
	{
		int i;
		char *  paint =  malloc(print_length+3);
		strcpy(paint, "");
		for(i = 0 ;  i< print_length ;  i++)
			strcat(paint , "-");
		//strcat(paint , "\0");
		printf("%s\n" , paint);
		for(i = 0 ;  i< (strlen(str)/print_length) ;  i++)
		{
			printf ("%.*s\n", print_length, str +  (print_length*i));
		}
		if((strlen(str)%print_length) !=0)
		{
			printf ("%.*s\n", print_length, str +  (print_length*i));
		}
		printf("%s\n", paint);
		free(paint);
	}
}


int number_length(int number){
  //1

  int totalDigits = 0;

  //3
  while(number!=0){
    //4
	  number = number/10;
    totalDigits ++;
  }
  //5
  return totalDigits;
}
//
void remove_all_chars(char* str, char c)
{
char *pr = str, *pw = str;
while (*pr)
    {
    *pw = *pr++;
    pw += (*pw != c);
    }
*pw = '\0';
}


const char * get_date_from_string(char * date) {
static char date_string[30];

	int hh, mm;
	float ss;
	time_t time_value;

	   int year = 0, month = 0, day = 0, hour = 0, min = 0, sec =0 ;

	   if (sscanf(date, "%4d%2d%2d%2d%2d%2d", &year, &month, &day, &hour, &min, &sec) == 6)
	   {
		   struct tm breakdown = {0};
		         breakdown.tm_year = year - 1900; /* years since 1900 */
		         breakdown.tm_mon = month - 1;
		         breakdown.tm_mday = day;
		         breakdown.tm_hour = hour;
		         breakdown.tm_min = min;

			time_value = mktime(&breakdown);
			if (time_value == -1) {
			    printf ("unable to make time\n");
			}
			else {

			  sprintf(date_string, "%4d/%2d/%2d %2d:%2d:%2d",breakdown.tm_year, breakdown.tm_mon, breakdown.tm_mday, breakdown.tm_hour, breakdown.tm_min, breakdown.tm_sec);
			  strftime(date_string, sizeof(date_string), "%Y-%m-%d %H:%M:%S", &breakdown);
			  puts(date_string);
			}


	   }
return date_string;
}
