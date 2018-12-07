#include <posapi.h>
#include <wnet.h>
#include <ppp.h>
#include <sys/time.h>
#include <curl/curl.h>
#include <stdlib.h>

#include "../src/utilities/keyboart.h"
#include "../src/utilities/lcd.h"
#include "receipt.h"

#include "jsonread.h"
#include "main_old.h"

void start_ppp_sess(void);
int starting_time= 1,retval;
pthread_t tidx,tidx1;
char gprs_chat_file[] = { "ABORT          'NO CARRIER'" "\n"
		"ABORT          'NO DIALTONE'" "\n"
		"ABORT          'ERROR'" "\n"
		"ABORT          'NO ANSWER'" "\n"
		"ABORT          'BUSY'" "\n"
		"ABORT          'Invalid Login'" "\n"
		"ABORT          'Login incorrect'" "\n"
		"TIMEOUT        '60'" "\n"
		"''             'ATZ'" "\n"
		"'OK'           'AT+CGDCONT=1,\"IP\",\"4.2.2.2\"'" "\n"
		"'OK'           'ATDT*99***1#'" "\n"
		"'CONNECT'      ''" "\n" };

CURL *curl;
CURLcode res;
FILE *out_fd = (FILE *) 0;
FILE *head_fd = (FILE *) 0;
char errmsg[256];
struct timeval start, cur, diff, end;
struct timezone tz;

//FPT
struct FtpFile {
  const char *filename;
  FILE *stream;
};


static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
{
  (void)ptr;
  (void)data;
  /* we are not interested in the headers itself,
     so we only return the size we would have saved ... */
  return (size_t)(size * nmemb);
}

int check_remote_service_size(void)
{
 // char ftpurl[] = "ftp://compulynx:coMpulYX008@103.241.181.153:2112/newpos/NewFinal.dat";
  CURL *curl;
  CURLcode res;
  long filetime = -1;
  double filesize = 0.0;
  char ftp_url[200];
  //const char *filename = strrchr(ftpurl, '/') + 1;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if(curl) {
	  strcpy(ftp_url,"ftp://compulynx:coMpulYX008@103.241.181.153:2112/newpos/");
	  strcat(ftp_url, CurrentUser.latest_version_number);
    curl_easy_setopt(curl, CURLOPT_URL, ftp_url);
    /* No download if the file */
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    /* Ask for filetime */
    curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
    /* No header output: TODO 14.1 http-style HEAD output for ftp */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    /* Switch on full protocol/debug output */
    /* curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */

    res = curl_easy_perform(curl);

    if(CURLE_OK == res) {
      /* http://curl.haxx.se/libcurl/c/curl_easy_getinfo.html */
      res = curl_easy_getinfo(curl, CURLINFO_FILETIME, &filetime);
      if((CURLE_OK == res) && (filetime >= 0)) {
        time_t file_time = (time_t)filetime;
        printf("filetime %s: %s", CurrentUser.latest_version_number, ctime(&file_time));
      }
      res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &filesize);
      if((CURLE_OK == res) && (filesize>0.0))
        printf("filesize %s: %0.0f bytes\n", CurrentUser.latest_version_number, filesize);
    } else {
      /* we failed */
      fprintf(stderr, "curl told us %d\n", res);
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return filesize;
}



static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out=(struct FtpFile *)stream;
  if(out && !out->stream) {
    /* open file for writing */
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */
  }
  return fwrite(buffer, size, nmemb, out->stream);
}


int ftp_service(void)
{
  CURL *curl;
  char ftp_url[200];
  CURLcode res;
  struct FtpFile ftpfile={
    CurrentUser.latest_version_number, /* name to store the file as if successful */
    NULL
  };

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if(curl) {
    /*
     * You better replace the URL with one that works!
     * ftp://compulynx:coMpulYX008@103.241.181.153:2112/newpos/NewFinal.dat
     */
	// strcpy(ftp_url,CurrentUser.update_url);
	//  strcat(ftp_url, CurrentUser.latest_version_number);
	           // ftp://compulynx:coMpulYX008@103.241.181.153:2112/newpos/NewFinal.dat
  strcpy(ftp_url,"ftp://compulynx:coMpulYX008@103.241.181.153:2112/newpos/");
  strcat(ftp_url, CurrentUser.latest_version_number);
	//  strcat(ftp_url, "NewFinal.dat");
	  printf("ftp URL : %s\n", ftp_url);
    curl_easy_setopt(curl, CURLOPT_URL,ftp_url);
    /* Define our callback to get called when there's data to be written */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    /* Set a pointer to our struct to pass to the callback */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

    /* Switch on full protocol/debug output */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    if(CURLE_OK != res) {
      /* we failed */
      fprintf(stderr, "curl told us %d\n", res);
		message_display_function(1,"","Firmware Update Error", "An error occured during firmaware update. Please ensure you have stable connectivity then try again.", (char *)NULL);
		kb_getkey();
		  if(ftpfile.stream)
		    fclose(ftpfile.stream); /* close the local file */

		  curl_global_cleanup();
		  return 0;
    }
    else
    {
    	  if(ftpfile.stream)
    	    fclose(ftpfile.stream); /* close the local file */

    	  curl_global_cleanup();
    	  return 1;
    }
  }



  return 0;
}



void* threader1(void *arg)
{
	char buffer[50];
	const int RUNTIME = 20;
	time_t start, current;
	start = time(NULL);

	if(pthread_self()==tidx1)
	{
		retval = wnet_power_on();
		printf("tidx1\n");
	}


	//retval = -2;
	else if(pthread_self()==tidx)
	do {

		current = time(NULL);

		if (difftime(current, start) == RUNTIME) {
			printf("tidx\n");

			pthread_cancel(tidx1);
			starting_time = -2;
			system("reboot");
			break;
		}
	} while (1);
	return NULL;
}

void start_ppp_session(char * requestType, char* request) {
	CURL *curl;
	CURLcode res;
	char filename[100];
	FILE *out_fd = (FILE *) 0;
	FILE *head_fd = (FILE *) 0;
	int retval = 0;
	char buffer[40];
	struct curl_slist *headers = NULL;
	char *output_request;
	char * url_data;
	char * url_receipts[200];

	if(strcmp(requestType , "TIMER") != 0 )
	{
		if(strcmp(requestType , "SYNCHRONIZE") == 0 || strcmp(requestType , "UPDATE_PRINT_FLAGS") == 0)
		{

		}
		else
		{
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER, "Connecting ....");
		lcd_printf(ALG_CENTER, "Please wait.");
		lcd_printf(ALG_CENTER, "");
		lcd_flip();
		}
		disable_timer = 1;
	}

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	retval = ppp_check("/var/mux1");
	if (0 != retval) {
		flag_online = 0 ;
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER, "");
		lcd_printf(ALG_CENTER, "Restarting Connection");
		lcd_printf(ALG_CENTER, "Established.");
		lcd_printf(ALG_CENTER, ".....");
		//kb_getkey();
		lcd_flip();
		int wnet_reset();
		wnet_power_down();
		power_on_modem_device();
		//start_ppp_sess();
		retval = ppp_check("/var/mux1");
		if (0 != retval) {
			lcd_clean();
			lcd_printf(ALG_CENTER, " ");
			lcd_printf(ALG_CENTER, "No Connection");
			lcd_printf(ALG_CENTER, "Established.");
			lcd_flip();
			//kb_getkey();

		}
	}
/*
	memset(buffer, 0,sizeof(buffer));
	sprintf(buffer , "ping -c 4 %s", myConfigurations->IpAddress);
	printf(buffer);
	int sy= system(buffer);
	if(sy != 0)
	{
		lcd_clean();
		lcd_printf(ALG_CENTER, " ");
		lcd_printf(ALG_CENTER, "Server not Reachable");
		lcd_printf(ALG_CENTER, "Please try again later");
		lcd_printf(ALG_CENTER, "Press any key to continue");
		lcd_flip();
		kb_getkey();
		return;
	}*/

	curl = curl_easy_init();
	if (curl) {
		 curl_off_t length_of_data;

		sprintf(filename, "%s%s.txt", "", requestType);
		printf("FILE NAME TX: %s\n",filename);
		out_fd = fopen(filename, "w");
		head_fd = fopen("header.txt", "w"); //open for read and write
		res = curl_easy_setopt(curl, CURLOPT_FILE, out_fd);
		res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

		printf("\nThis is my final : %s\n",request);
		 output_request  = curl_easy_escape(curl, request, strlen(request));
		   if(output_request) {
		     //curl_free(output_request);
//{"REPORTS":[{"billno":"009782016551-0031","transactiontype":"1","transactiondate":"2014-12-30 20:16:40","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"50.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"54"}},{"billno":"009782016551-0032","transactiontype":"1","transactiondate":"2014-12-30 20:39:27","serviceid":"2","paymentmethod":"Cash","paymentCode":"","total":"25.0","user":"6","voided":"0","posted":"1","receiptprinted":"1"},{"billno":"009782016551-0033","transactiontype":"1","transactiondate":"2014-12-30 20:39:45","serviceid":"2","paymentmethod":"Cash","paymentCode":"","total":"100.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"54645","Colour":"4645","Name of Seller":"4569","Location":"6546","Sub-Location":"6545"}},{"billno":"009782016551-0034","transactiontype":"1","transactiondate":"2014-12-30 20:47:41","serviceid":"2","paymentmethod":"Cash","paymentCode":"","total":"25.0","user":"6","voided":"0","posted":"1","receiptprinted":"1"},{"billno":"009782016551-0035","transactiontype":"1","transactiondate":"2014-12-30 20:47:55","serviceid":"2","paymentmethod":"Cash","paymentCode":"","total":"100.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"322","Customer Name":"322","Customer Phone":"544"}},{"billno":"009782016551-0036","transactiontype":"1","transactiondate":"2014-12-30 20:48:09","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"100.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"Vehicle Reg":"555"}},{"billno":"009782016551-0037","transactiontype":"1","transactiondate":"2014-12-30 20:48:24","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"100.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"555","Customer Name":"566","Customer Phone":"65","ID NO":"555","Customer Name":"566","Customer Phone":"65"}},{"billno":"009782016551-0038","transactiontype":"1","transactiondate":"2014-12-30 20:48:44","serviceid":"7","paymentmethod":"Cash","paymentCode":"","total":"20.0","user":"6","voided":"0","posted":"1","receiptprinted":"1"},{"billno":"009782016551-0039","transactiontype":"1","transactiondate":"2014-12-30 20:49:01","serviceid":"2","paymentmethod":"Cash","paymentCode":"","total":"300.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"5555"}},{"billno":"009782016551-0040","transactiontype":"1","transactiondate":"2014-12-30 20:49:12","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"100.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"Vehicle Reg":"55546"}},{"billno":"009782016551-0041","transactiontype":"1","transactiondate":"2014-12-30 20:49:25","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"50.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"5545"}},{"billno":"009782016551-0042","transactiontype":"1","transactiondate":"2014-12-30 20:49:49","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"50.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"4444"}},{"billno":"009782016551-0043","transactiontype":"1","transactiondate":"2014-12-30 20:50:10","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"100.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"Vehicle Reg":"565659656"}},{"billno":"009782016551-0044","transactiontype":"1","transactiondate":"2014-12-30 20:50:33","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"50.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"6"}},{"billno":"009782016551-0045","transactiontype":"1","transactiondate":"2014-12-30 20:51:26","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"50.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"44"}},{"billno":"009782016551-0046","transactiontype":"1","transactiondate":"2014-12-30 20:51:34","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"100.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"Vehicle Reg":"6544"}},{"billno":"009782016551-0047","transactiontype":"1","transactiondate":"2014-12-30 20:51:48","serviceid":"2","paymentmethod":"Cash","paymentCode":"","total":"100.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"4","Colour":"455","Name of Seller":"456698","Location":"644","Sub-Location":"65"}}]}
		   }
		  url_data =  malloc(strlen(output_request) + strlen(url)+20);
		  strcpy(url_data ,url);
/*		  if(strcmp(requestType , "UPDATE_PRINT_FLAGS") == 0)
			  strcat(url_data ,"updateprintflags/");*/
			 // url_receipts
		  strcat(url_data ,output_request);
		  length_of_data = strlen(url_data);
		res = curl_easy_setopt(curl, CURLOPT_URL, url_data);
		printf("%s \n", url_data);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, length_of_data);

		//res = curl_easy_setopt(curl, CURLOPT_POST, 1);

		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS,output_request);

		res = curl_easy_setopt(curl, CURLOPT_WRITEHEADER, head_fd);

		res = curl_easy_perform(curl);
		curl_free(output_request);
		free(url_data);
		if(res!=0)
		{
			//flag_stop_z = 1;
			if(strcmp(requestType,"REPORT")!=0)
				flag_online = 0;
			///lcd_clean();
			//screen_header();
			//lcd_printf(ALG_CENTER, "");
			//lcd_printf(ALG_CENTER, "An Error Occured in Setting \n up the transaction.\n Error CODE : %d.", res);
			if(strcmp(pmode, "Cash")==0 && strcmp(requestType,"TRANSACTION")==0)
					lcd_printf(ALG_CENTER, " Transaction shall be saved offline.");
			//lcd_printf(ALG_CENTER, ".....");
			//
			//lcd_flip();
			//kb_getkey();
			if(strcmp(pmode, "Cash")==0 && strcmp(requestType,"TRANSACTION")==0)
			{

				sprintf(transactionfile , "update TRANSACTIONS set receipt_printed = '1' where billno = '%s'", recptnum);
				sqlite_database_insert_into_table(transactionfile,"transaction");
				Printer_Demo("TX RECEIPT");
				//if(printflag == 0){
					sprintf(transactionfile , "update TRANSACTIONS set receipt_printed = '2' where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");
				//}
/*				else if (printflag == 1)
				{
					sprintf(transactionfile , "update TRANSACTIONS set receipt_printed = '1' and print_flag = '0' where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");
				}*/


				usleep(10000);
				please_print = 1;
	/*			sqlite_database_insert_into_table(transactionstring,"transaction");*/



				sprintf(transactionfile , "update Z_REPORTS set Z_done = '0' , Printed = 1 where billno = '%s'", recptnum);
				sqlite_database_insert_into_table(transactionfile,"transaction");
			}
			if( strcmp(requestType,"LOGIN")==0)
			{
				message_display_function(1,"","Error In Login", "Server connection failed. Please check connection and try again. Proceeding offline", (char *)NULL);
				kb_getkey();
			}
			flag_stop_z = 2;

		}
		else
		{
			fclose(out_fd);
			fclose(head_fd);
		}

	}

	curl_easy_cleanup(curl);
	if (CURLE_OK != res) {
		out_fd = fopen("error.txt", "w");
		fclose(out_fd);
		//exit(0);

	}
	if(strcmp(requestType , "TIMER") != 0)
	{
		disable_timer = 0;
	}
}
void start_ppp_session1(char* request) {
	CURL *curl;
	CURLcode res;
	FILE *out_fd = (FILE *) 0;
	FILE *head_fd = (FILE *) 0;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charsets: utf-8");
	curl = curl_easy_init();
	//chdir("/mnt/jffs2");
	if (curl) {
		printf("\nDownloading %s file...\n", "/root/response");
		out_fd = fopen("body.txt", "w"); //open for read and write
		//out_fd = fopen ("body.txt", "w");//open for read and write
		head_fd = fopen("header.txt", "w"); //open for read and write
		res = curl_easy_setopt(curl, CURLOPT_FILE, out_fd);
		printf("\nDownloading %s file...\n", "/root/response");
		res =curl_easy_setopt(curl, CURLOPT_URL,"http://197.220.114.46:8121/victoriaswitch/rest/service/$$12345678912345678@testuser@TRANSACTION@&\"name:amon\"&$");
		printf("url 2 %s", request);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		res = curl_easy_setopt(curl, CURLOPT_POST, 1);

		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
				"$$12345678912345678@testuser@TRANSACTION@&\{\"name\"amon}&$");
		res = curl_easy_setopt(curl, CURLOPT_WRITEHEADER, head_fd);
		res = curl_easy_perform(curl); //post away!
		fclose(out_fd);
		fclose(head_fd);
	}
	//there must be a corresponding curl_easy_cleanup() to curl_easy_init()

	curl_easy_cleanup(curl);
	if (CURLE_OK != res) {
		/* we failed */
		puts("error occured is\n");
		printf("%s\n", errmsg);
		out_fd = fopen("body2.txt", "w");
		fprintf(out_fd, "%s\n", errmsg);
		//Display_login_screen("** VIHIGA COUNTY **", errmsg, errmsg, errmsg,
				//errmsg);
		fclose(out_fd);

		//return -1;
	}

}

void start_ppp_sess() {
	int retval;
	retval = 0;
	retval = ppp_open("/var/mux1", gprs_chat_file,
			myConfigurations->apn_username, myConfigurations->apn_password,
			PPP_ALG_PAP, myConfigurations->apn_password);
	if (0 != retval) {
		//lcd_printf(ALG_LEFT, "PPP open failed %d", retval);
		//lcd_flip();
		// kb_getkey();
	} else {
		gettimeofday(&start, &tz);
		diff.tv_sec = 60;
		diff.tv_usec = 0;
		timeradd(&start, &diff, &end);

		//lcd_flip();
		while (1) {
			retval = ppp_check("/var/mux1");
			if (0 == retval) {
				//return 0;
				break;
			} else {
				gettimeofday(&cur, &tz);
				if (timercmp(&cur, &end, <))
					usleep(10000);
				else {
					lcd_printf(ALG_LEFT, "PPP login failed  ");
					lcd_printf(ALG_LEFT,"Error code: %S", retval);
					lcd_printf(ALG_LEFT, "Proceeding offline");
					lcd_flip();
					kb_getkey();
					break;
				}
			}
		}
		//ppp_close("/var/mux1");

	}
}
/*

void power_on_modem_device() {

	int retval = 0;
	int len;

	lcd_clean();
	lcd_printf(ALG_CENTER, "REVENUE SYSTEM");
	lcd_printf(ALG_CENTER, " ");
	lcd_printf(ALG_CENTER, "Connecting.....");
	lcd_flip();


	int err;


	retval = wnet_power_on();

	if (0 != retval) {
		lcd_printf(ALG_CENTER, "SIMCARD MODULE FAILED TO START");
		lcd_flip();
		kb_getkey();
	} else {

		lcd_flip();
		retval = wnet_init("/var/mux0");
		if (0 != retval) {
			lcd_printf(ALG_CENTER, "NETWORK INNITIALIZATION FAILED", retval);
			lcd_flip();
			kb_getkey();
		} else {
			gettimeofday(&start, &tz);
			diff.tv_sec = 60;
			diff.tv_usec = 0;
			timeradd(&start, &diff, &end);

			lcd_flip();

			while (1) {
				retval = wnet_set_attached(1);
				if (0 != retval) {
					gettimeofday(&cur, &tz);

					if (timercmp(&cur, &end, <))
						usleep(10000);
					else {
						lcd_printf(ALG_CENTER, "SIM CARD MISSING");
						lcd_flip();
						kb_getkey();
						break;
					}
					break;
				} else
					break;
			}

			if (0 == retval) {

			}
		}

		//wnet_power_down();
	}
}
*/

void power_on_modem_device() {

	int retval = 0;

	if(fag_start_ppp_session)
	{

		screen_header();
		lcd_printf(ALG_CENTER, "Starting Modem ...");
		lcd_printf(ALG_CENTER, "Please wait ...");
		lcd_flip();
	retval = wnet_power_on();
	}



	if (0 != retval) {
		printf("Hello 1\n");
		flag_online = 0;
		//message_display_function("Error" , "GSM module failed to start \n Please restart the POS");
		//kb_getkey();
	} else {
		printf("Hello 1\n");
		//lcd_flip();
		retval = wnet_init("/var/mux0");
		if (0 != retval) {
			printf("Hello 2\n");
			//message_display_function("Error" , "Network initialization failed \n Please check SIM and restart the POS");
			//kb_getkey();
		} else {
			printf("Hello 3\n");
			gettimeofday(&start, &tz);
			diff.tv_sec =20;
			diff.tv_usec = 0;
			timeradd(&start, &diff, &end);

			//lcd_flip();

			while (1) {
				printf("Hello 4\n");

				retval = wnet_set_attached(1);
				if (0 != retval) {
					gettimeofday(&cur, &tz);

					message_display_function(1, "","Error" , "Network initialization failed \n Please insert SIM and restart the POS", (char *)NULL);
					kb_getkey();
					if (timercmp(&cur, &end, <))
						usleep(1000);
					else {
						printf("Hello 5\n");
						break;
					}
					break;
				} else
				{
					printf("Hello 6\n");
					retval = ppp_open("/var/mux1", gprs_chat_file,myConfigurations->apn_username,  myConfigurations->apn_password, PPP_ALG_PAP, 30);
					if (0 != retval){
						flag_online = 1;
					}
					else
					{
						sleep(5);
						retval = ppp_open("/var/mux1", gprs_chat_file,myConfigurations->apn_username,  myConfigurations->apn_password, PPP_ALG_PAP, 30);

						if (0 != retval){
							flag_online = 1;
						}
						else
						{
						message_display_function(1,"","Modem Error ", "Failed to innitialize the PPP session . The POS shall operate in offline mode", (char *)NULL);
						kb_getkey();
						flag_online = 0;
						}
					}
				}
					break;
			}

			if (0 == retval) {
				//online_mode=1;
			}
		}

		//wnet_power_down();
	}
}

void gprs_demo() {

	int retval = 0;
	int len;

	CURL *curl;
	CURLcode res;
	FILE *out_fd = (FILE *) 0;
	FILE *head_fd = (FILE *) 0;
	char errmsg[256];
	struct timeval start, cur, diff, end;
	struct timezone tz;

//lcd_clean();
//lcd_printf(ALG_CENTER, "VIHIGA COUNTY SERVICES");
//lcd_printf(ALG_LEFT, "Connecting.....");
//lcd_flip();

	retval = wnet_power_on();
	if (0 != retval) {
		//lcd_printf(ALG_LEFT, "Power On Failed");
		//lcd_flip();
		kb_getkey();
	} else {

		//lcd_flip();
		retval = wnet_init("/var/mux0");
		if (0 != retval) {
			//lcd_printf(ALG_LEFT, "Init wnet failed %d", retval);
			//lcd_flip();
			kb_getkey();
		} else {
			gettimeofday(&start, &tz);
			diff.tv_sec = 60;
			diff.tv_usec = 0;
			timeradd(&start, &diff, &end);

			//lcd_flip();

			while (1) {
				retval = wnet_set_attached(1);
				if (0 != retval) {
					gettimeofday(&cur, &tz);

					if (timercmp(&cur, &end, <))
						usleep(10000);
					else {
						//lcd_printf(ALG_LEFT, "wnet_set_attached() failed %d", retval);
						//lcd_flip();
						kb_getkey();
						break;
					}
				} else
					break;
			}

			if (0 == retval) {

				//lcd_flip();

				retval = ppp_open("/var/mux1", gprs_chat_file,
						myConfigurations->apn_username,
						myConfigurations->apn_password, PPP_ALG_PAP,
						myConfigurations->ppp_timeout);
				if (0 != retval) {
					//lcd_printf(ALG_LEFT, "PPP open failed %d", retval);
					//lcd_flip();
					kb_getkey();
				} else {
					gettimeofday(&start, &tz);
					diff.tv_sec = 60;
					diff.tv_usec = 0;
					timeradd(&start, &diff, &end);

					//lcd_flip();
					while (1) {
						retval = ppp_check("/var/mux1");
						if (0 == retval) {
							curl = curl_easy_init();
							//chdir("/mnt/jffs2");
							if (curl) {
								out_fd = fopen("body.txt",
										"w");    //open for read and write
								head_fd = fopen(
										"header.txt", "w"); //open for read and write
								res = curl_easy_setopt(curl, CURLOPT_FILE,
										out_fd);

								res =
										curl_easy_setopt(curl, CURLOPT_URL,
												"http://197.220.114.46:8121/victoriaswitch/rest/service/$$12345678912345678@testuser@SUBSERVICE@NN@1,0$$");

								res = curl_easy_setopt(curl, CURLOPT_POST, 1);

								res =
										curl_easy_setopt(curl,
												CURLOPT_POSTFIELDS,
												"$$12345678912345678@testuser@SERVICEDATA@NN@1,1$$");

								res = curl_easy_setopt(curl,
										CURLOPT_WRITEHEADER, head_fd);

								res = curl_easy_perform(curl); //post away!
								fclose(out_fd);
							}
							//there must be a corresponding curl_easy_cleanup() to curl_easy_init()

							curl_easy_cleanup(curl);
							if (CURLE_OK != res) {
								/* we failed */
								puts("error occured is\n");
								printf("%s\n", errmsg);
								out_fd = fopen("body.txt",
										"a");
								fprintf(out_fd, "%s\n", errmsg);
								fclose(out_fd);
								//return -1;
							}
							// lcd_printf(ALG_LEFT, res);
							// lcd_menu("RES", res, sizeof(menu)/100, selected);

							//lcd_printf(ALG_LEFT, "Connected");
							//lcd_flip();
							kb_getkey();
							break;
						} else {
							gettimeofday(&cur, &tz);
							if (timercmp(&cur, &end, <))
								usleep(10000);
							else {
								//lcd_printf(ALG_LEFT, "PPP logon failed %d", retval);
								//lcd_flip();
								kb_getkey();
								break;
								}
						}
					}
					ppp_close("/var/mux1");
				}
			}
		}

		wnet_power_down();
	}
}
