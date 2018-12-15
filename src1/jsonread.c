#include "menu_hash.h"
#include "jsonread.h"

#include "../../src/utilities/JSON_checker.h"
#include "../src/utilities/cJSON.h"
#include "../src/utilities/send_online_request.h"
#include "../src/utilities/lcd.h"
#include "receipt.h"
#include "main_old.h"

extern int usleep (__useconds_t __useconds);

char* main_menu_string = "0";
char* holder;
int lock_at_menu_items = 0, location = 0;
char menu_name[40] = "";
char *replace1(char *s);
extern void Printer_Demo(char * mychar);
//int jcheck(char* mystr);

/*printing the value corresponding to boolean, double, integer and strings*/

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

char * hash_fnc(char * Str){
	char buf[256];
	char cmd[40];
	FILE *output;
	char hashed_password[40];
	sprintf(cmd , "echo -n %s | md5sum",Str);
	output= popen(cmd, "r");
	while (fgets(buf, sizeof(buf), output) != 0) {
	    /*...*/
		//printf("%s\n",buf);
		snprintf(hashed_password,33, "%s", buf);
	}
	pclose(output);
	return hashed_password;
}


char* Start_online_display(char *filename) {

	char filelast[100];
	char str_cmp[100];
	cJSON *name = NULL;
	cJSON *request_json = NULL;

	char * outputstring;

	char * sql;


	FILE* body_file ;
	body_file = fopen(filename, "r"); //open for read and write
	printf("FILE NAME ALEX : %s\n",filename);
	if (body_file == NULL) {

		if(strcmp("REPORT.txt",filename ) !=0)
		flag_online = 0;
		printf("Error Opening the file\n");
		if(strcmp("SYNCHRONIZE.txt",filename ) ==0)
		{
			system("cp services.db_bak services.db");
			update_stopped = 1;
		}
		return "err";
	} else
	{
		service_details_not_found = 0;
		memset(string,0,sizeof(string));
		if (fgets(string, 10000, body_file) != NULL) {
			printf("RETURNED ALEX:  \n%s\n\n", string);
			fclose(body_file);
			sprintf(filelast,"rm %s",filename);
			system(filelast);


			if(jcheck(string))
			{
				int  i =0, len_ar;

				if (strcmp(filename, "data.txt") == 0) {

					request_json = cJSON_Parse(string);
					printf("Line 1\n");

					if(request_json!=NULL){
						printf("Line rm operator.db 1\n");
					 system("rm operator.db");
					 create_all_table();

						  printf("Line 2\n");
						cJSON * message =cJSON_GetObjectItem(request_json,"message" );
						  printf("Line 3\n");

						 char *  pin    = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "pin")));
						 char *  username    = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "username")));
						 char *  agentid    = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "agentid")));
						 char *  idnumber    = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "idnumber")));

					     printf("Line 4\n");
					     sql = malloc(750);
					     sprintf(sql , "insert into operator (username,pin,idnumber,agentid ) values ( LOWER('%s'),'%s','%s', '%s')" , username, pin, idnumber, agentid /*hash_fnc(pin1)*/);
					     printf("%s\n",sql);

					     read_database(sql,"operator");
					     free(sql);
					}
					  return string;
				}
				else
				{



								if (subservice_id != 0) {
									free(subservice_id);
									subservice_id = 0;
								}
								if (service_level != 0) {
									free(service_level);
									service_level = 0;
								}
								if (parentType != 0) {
									free(parentType);
									parentType = 0;
								}
								if (params != 0) {
									free(params);
									params = 0;
								}

								if (charges != 0) {
									free(charges);
									charges = 0;
								}
								if (parentId != 0) {
									free(parentId);
									parentId = 0;
								}
								if (subservice_name != 0) {
									free(subservice_name);
									subservice_name = 0;
								}

								strcpy(response, "");

/*
								json_object * jobj = json_tokener_parse(string);
								json_object *jarray = jobj;

								if (jobj != NULL) {
									arraylen = json_object_array_length(jarray);
									int loop = 0;

									read_online_jason_file(jobj, NULL);
								} else {
									return "err";
								}*/
				}
								//flag_online = 1;

			}
			else
			{
				update_stopped = 1;
				if(strcmp(filename,"REPORT.txt")!= 0)
				flag_online = 0;
				if(strcmp(filename,"UPDATE_PRINT_FLAGS.txt")== 0)
				{

				}
				else
				{
					lcd_clean();
					lcd_printf(ALG_CENTER, " ");
					screen_header();
					lcd_printf(ALG_CENTER, "Invalid JSON Format.\n Contact Admin");
					lcd_printf(ALG_CENTER, "Press enter to continue");
					lcd_flip();
					kb_getkey();
				}
				if(online)
				design_url("SERVICEDATA", "j");
				return "err";
			}

			if (strcmp(response, "Invalid user") == 0)
			{
				lcd_clean();
				printf(" \n\n\n\n");
				lcd_printf(ALG_CENTER, " ");
				printf("Invalid User");
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid User");
				lcd_printf(ALG_CENTER, "Press enter to continue");
				lcd_flip();
				login_successful = 0;
				update_stopped = 1;
				kb_getkey();
				goto EXITLOOP;
			}

			if (strcmp(response, "User is not allowed to use POS") == 0)
			{
				lcd_clean();
				printf(" \n\n\n\n");
				lcd_printf(ALG_CENTER, " ");
				printf("Invalid User");
				screen_header();
				lcd_printf(ALG_CENTER, "User is not allowed to use POS");
				lcd_printf(ALG_CENTER, "Press enter to continue");
				lcd_flip();
				login_successful = 0;
				update_stopped = 1;
				kb_getkey();
				goto EXITLOOP;
			}
			if (strcmp(response, "Device is locked. Contact Admin") == 0) {
				lcd_clean();
				printf(" \n\n\n\n");
				lcd_printf(ALG_CENTER, " ");
				printf("Invalid User");
				screen_header();
				lcd_printf(ALG_CENTER, "Your device has been locked");
				lcd_printf(ALG_CENTER, "Please contact admin");
				lcd_printf(ALG_CENTER, "Press enter to exit");
				lcd_flip();
				kb_getkey();
				exit(100);
				//login();
				login_successful = 0;
				update_stopped = 1;

				goto EXITLOOP;
			}
			if (strcmp(filename, "VOID.txt") == 0) {
				sprintf(str_cmp ,"Transaction bill - %s has voided successfully", recptnum);
				if (strcmp(response, "200")  == 0) {
					 getdate_mine();
					sprintf(str_cmp ,"SELECT * from TRANSACTIONS where billno = '%s'",recptnum);
					read_database(str_cmp ,"transaction");

					if (transactin_val_name != 0) {
						free(transactin_val_name);
						transactin_val_name = 0;
					}
					if (paramtype != 0) {
						free(paramtype);
						paramtype = 0;
					}
					if (transactin_name != 0) {
						free(transactin_name);
						transactin_name = 0;
					}
					if (transactin_val != 0) {
						free(transactin_val);
						transactin_val = 0;
					}
					number_of_trans=0;
					sprintf(str_cmp ,"SELECT name  as param_dtls_name , Value as param_dtls_val from PARAMS where billno = '%s';",recptnum);
					read_database(str_cmp,"transaction");

					void_receipt();
					Printer_Demo("VOIDED RECEIPT");
					sprintf(str_cmp ,"UPDATE voids_count SET voids = voids + 1 WHERE  date(voidtransactiondate) =  (SELECT date('now','localtime'));");
								read_database(str_cmp,"users_config");
					please_print = 1;
					goto EXITLOOP;
				} else{
					//Printer_Demo();

					//revert_receipt_number();
					screen_header();
					lcd_printf(ALG_CENTER, response);
					lcd_flip();
					kb_getkey();
					if(online)
					design_url("SERVICEDATA", "j");
					goto EXITLOOP;
				}
				//flag_online = 1;
			}
			else if (strcmp(filename, "LOGIN.txt") == 0) {
				printf("home/user0/Desktop/LOGIN.txt\n");
				//flag_online = 1;
				if (strcmp(response, "9006") == 0)
				{
					printf("Invalid User");
					screen_header();
					lcd_printf(ALG_CENTER, "Invalid login");
					lcd_printf(ALG_CENTER, "Credential.");
					lcd_printf(ALG_CENTER, "Press enter to try again");
					lcd_flip();
					kb_getkey();
					login_successful = 0;
					login_successful = 0;
					goto EXITLOOP;
				}
				else

				{
					//design_url("GET_POS_USERS","j");
				}


			}

			else if (strcmp(filename, "SYNCHRONIZE.txt")== 0) {
				int x;
				char * sql;
				sql = malloc(700);
				strcpy(sql, "");
				current_synch_batch = current_synch_batch+10;
				//flag_online = 1;
				if (strcmp(response, "done") == 0) {
					current_synch_batch =0;
					lcd_clean();
					current_synch_item++;
					screen_header();
					lcd_printf(ALG_CENTER, "");
					lcd_printf(ALG_CENTER, "Please wait ..");

					if(current_synch_item == 1)
					lcd_printf(ALG_CENTER, "Synchronizing subservices ...");
					if(current_synch_item == 2)
					lcd_printf(ALG_CENTER, "Synchronizing params ");
					if(current_synch_item == 3)
					lcd_printf(ALG_CENTER, "Synchronizing service \n details ");

					lcd_flip();

					if(current_synch_item>4){
						flag_update_menu= 0;
					}
						goto EXITLOOP;
				}
				else{
					for (x = 0; x < arraylen; x++)
					{
						if(current_synch_item == 0){
							sprintf(sql , "insert into services (id, serviceCode,serviceName) VALUES ('%s','%s','%s');",subservice_id[x] , service_code[x], service_name[x]);
						}
						if(current_synch_item == 1){


							sprintf(sql , "insert into subservices (id, parentType,parentId,level,name) VALUES ('%s','%s','%s','%s','%s');",subservice_id[x] , parentType[x], parentId[x], service_level[x], subservice_name[x]);
						}
						if(current_synch_item == 2){

							sprintf(sql , "insert into params (id, name,paramtype) VALUES ('%s','%s','%s');",subservice_id[x] , subservice_name[x], paramtype[x]);
						}
						if(current_synch_item == 3){

							sprintf(sql , "insert into SERVICEDTLS (id, charge,params) VALUES ('%s','%s','%s');",subservice_id[x] , charges[x], params[x]);
						}
						sqlite_database_insert_into_table(sql,"services");

					}
					free(sql);
				}
			}

			else if (strcmp(filename, "CHANGEPASSWORD.txt")== 0) {
				printf("home/user0/Desktop/CHANGEPASSWORD.txt\n");
				if (strcmp(response, "200") == 0) {
					printf("home/user0/Desktop/CHANGEPASSWORD.txt failed\n");
					lcd_clean();
					lcd_printf(ALG_CENTER, "");
					lcd_printf(ALG_CENTER, "password change successful");
					lcd_printf(ALG_CENTER, "Press enter to continue");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;
				}

				else if (strcmp(response, "400") == 0) {
					printf("home/user0/Desktop/CHANGEPASSWORD.txt failed\n");
					lcd_clean();
					lcd_printf(ALG_CENTER, "");
					lcd_printf(ALG_CENTER, "Change Not successful");
					lcd_printf(ALG_CENTER, "Press enter to continue");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;
				}
			}

			else if (strcmp(filename, "LICENCEFEE.txt")== 0) {
				//flag_online = 1;
				if (strlen(response) > 0) {

					if (strcmp(response, "400") == 0) {

										lcd_clean();
										screen_header();
										lcd_printf(ALG_CENTER, "");
										lcd_printf(ALG_CENTER, "Invalid Transaction");
										lcd_printf(ALG_CENTER, "Try Again");
										lcd_flip();/*
										revert_receipt_number();
										sprintf(transactionfile , "delete from TRANSACTIONS where billno = '%s'", recptnum);
										sqlite_database_insert_into_table(transactionstring,"transaction");
										sprintf(transactionfile , "delete from Z_REPORTS where billno = '%s'", recptnum);
										sqlite_database_insert_into_table(transactionstring,"transaction");

*/
										//design_url("SERVICEDATA", "j");
										kb_getkey();
										goto EXITLOOP;
									}
					else{
					lcd_clean();
					lcd_printf(ALG_CENTER, "");
					lcd_printf(ALG_CENTER, response);
					lcd_printf(ALG_CENTER, "Please try Again");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;
					}
				}
			}
			else if (strcmp(filename, "LICENCE.txt")== 0) {
				//flag_online = 1;
				if(strcmp(response, "200")== 0){
					lcd_clean();

					sprintf(transactionfile , "update SBP_TX set posted = '1' where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");
					SBP_printer("ORIGINAL RECEIPT");
					printf(transactionfile , "update SBP_TX set receipt_printed = '1' where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");
					goto EXITLOOP;
				}





				else if (strlen(response) > 0) {;
					lcd_clean();
					screen_header();
					lcd_printf(ALG_CENTER, "");

					if(strcmp(response, "400")== 0)
					{
						lcd_printf(ALG_CENTER, "Error in Processing");
						lcd_printf(ALG_CENTER, "transaction");
					}
					else
						lcd_printf(ALG_CENTER, response);
					lcd_printf(ALG_CENTER, "Please try Again");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;
				}
			}
			else if (strcmp(filename, "TRANSACTION.txt")== 0) {
				//flag_online = 1;
				if (strcmp(response, "200") == 0) {

					sprintf(transactionfile , "update TRANSACTIONS set posted = '1' where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");
					sprintf(transactionfile , "update TRANSACTIONS set receipt_printed = '1' where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");
					Printer_Demo("TX RECEIPT");
					//if(printflag == 0){
						sprintf(transactionfile , "update TRANSACTIONS set  receipt_printed= '2' where billno = '%s'", recptnum);
						sqlite_database_insert_into_table(transactionfile,"transaction");





					usleep(10000);
					please_print = 1;
					sqlite_database_insert_into_table(transactionstring,"transaction");



					sprintf(transactionfile , "update Z_REPORTS set Z_done = '0' , Printed = 1 where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");


					memset(transactionfile,0,sizeof(transactionfile));
					memset(transactionstring,0,sizeof(transactionstring));


					//Send update flags



					goto EXITLOOP;
				} else if (strcmp(response, "400") == 0) {


					revert_receipt_number();

                    //transactionstring  to  transactionfile  by micah
					sprintf(transactionfile,"update transactions set receipt_printed = '0' where billno = '%s';",recptnum);
					//sprintf(transactionfile , "delete from TRANSACTIONS where billno = '%s';", recptnum);
					printf("SQL : %s\n" , transactionfile);
					read_database(transactionfile,"transaction");
					sprintf(transactionfile , "delete from Z_REPORTS where billno = '%s';", recptnum);
					printf("SQL : %s\n" , transactionfile);
					read_database(transactionfile,"transaction");
					sprintf(transactionstring , "delete from PARAMS where billno = '%s';", recptnum);
					printf("SQL : %s\n" , transactionfile);
					read_database(transactionfile,"transaction");
					//added by micah
					memset(transactionfile,0,sizeof(transactionfile));
					memset(transactionstring,0,sizeof(transactionstring));

					//design_url("SERVICEDATA", "j");
					lcd_clean();
					lcd_printf(ALG_CENTER, "");
					lcd_printf(ALG_CENTER, "Invalid Transaction");
					lcd_printf(ALG_CENTER, "Try Again");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;
				}



				else if(strcmp(response,"400")!=0 && strcmp(response,"200")!=0) {
					revert_receipt_number();

                //transactionstring  to  transactionfile  by micah
				sprintf(transactionfile , "delete from TRANSACTIONS where billno = '%s';", recptnum);
				printf("SQL : %s\n" , transactionfile);
				read_database(transactionfile,"transaction");
				sprintf(transactionfile , "delete from Z_REPORTS where billno = '%s';", recptnum);
				printf("SQL : %s\n" , transactionfile);
				read_database(transactionfile,"transaction");

				sprintf(transactionstring , "delete from PARAMS where billno = '%s';", recptnum);
				printf("SQL : %s\n" , transactionfile);
				read_database(transactionfile,"transaction");
				//added by micah
				memset(transactionfile,0,sizeof(transactionfile));
				memset(transactionstring,0,sizeof(transactionstring));

				//design_url("SERVICEDATA", "j");
				lcd_clean();
				lcd_printf(ALG_CENTER, "");
				lcd_printf(ALG_CENTER, "Invalid Transaction");
				lcd_printf(ALG_CENTER, "Try Again");
				lcd_flip();
				kb_getkey();
				goto EXITLOOP;
				}

				else
				{
					sprintf(transactionfile , "update TRANSACTIONS set errored = '0' , posted = '0' where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");
					sprintf(transactionfile , "update Z_REPORTS set Z_done = '0'  where billno = '%s'", recptnum);
					sqlite_database_insert_into_table(transactionfile,"transaction");
					if(online)
					design_url("SERVICEDATA", "j");
					strcpy(payment_ref_code,"");
					strcpy(pmode,"");
					strcpy(charge,"");
					goto EXITLOOP;


					memset(transactionfile,0,sizeof(transactionfile));
					memset(transactionstring,0,sizeof(transactionstring));

				}


			}

			else if (strcmp(filename, "REPORT.txt")	== 0) {

				//z_number_recieved;
				/*if(strlen(string)>9)
					snprintf(z_number_recieved ,10,"%s" ,string);*/

				//flag_online = 1;
				printf ("Z response :  %s \n", response);
				if (strcmp(response, "400") == 0)
				{

					flag_stop_z = 1;
					lcd_clean();
					screen_header();
					lcd_printf(ALG_CENTER, "");
					lcd_printf(ALG_CENTER, "Invalid Z transactions");
					lcd_printf(ALG_CENTER, "Please contact Technician");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;
				}
				if (z_number_recieved != 0)
				{
					//print_z_report();

					printf("Z _Number : %d", z_number_recieved);
					//sprintf(transactionfile , "update Z_REPORTS set Z_done = '1'", recptnum);
					//sqlite_database_insert_into_table(transactionfile,"transaction");

					//print_x_z_report("Z");

					z_rejected = 0;
					flag_stop_z = 0;

					goto EXITLOOP;
				}
				else
				{
					flag_stop_z = 1;
					lcd_clean();
					screen_header();
					lcd_printf(ALG_CENTER, response);
					lcd_printf(ALG_CENTER, "Please contact Technician");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;

				}

			}

			else if (strcmp(filename, "SERVICEDTL.txt")== 0) {

				if (strlen(response) != 0) {

					service_details_not_found = 1;
					body_file = fopen(filename, "r");
					lcd_clean();
					lcd_printf(ALG_CENTER, " ");
					lcd_printf(ALG_CENTER, " ");
					lcd_printf(ALG_CENTER, response);
					lcd_printf(ALG_CENTER, "Press enter to continue");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;

				}
				service_details_not_found = 0;
			} else if (strcmp(filename, "SUBSERVICE.txt")== 0) {
				if (strcmp(string, "service details not found") == 0) {
					service_details_not_found = 1;
					printf("Child services not found");
					lcd_clean();
					lcd_printf(ALG_CENTER, " ");
					lcd_printf(ALG_CENTER, "Child services not found");
					lcd_printf(ALG_CENTER, "Press enter to continue");
					lcd_flip();
					kb_getkey();
					goto EXITLOOP;
				}

				service_details_not_found = 0;
			}
			else if (strcmp(filename, "TIMER.txt")== 0) {
				if (strcmp(response, "200") == 0 ) {
				//flag_online = 1;
				}
				else
				{
					flag_stop_z = 1;
					flag_online = 0;
				}
			}


			please_print = 0;


			EXITLOOP: return "exit";

		} else {
			printf("The file is empty");
			update_stopped = 1;
			 if (strcmp(filename, "REPORT.txt")!= 0)
			 {
				 flag_online = 0;
				 flag_stop_z  = 2;
			 }
			//memset(string,0,sizeof(string));

			return "empty_file";
		}
	}
	return string;

}

/*Parsing the json object*/

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
	    	//printf("Good json");
	    	return 1;
	    }
	}
