#include "receipt.h"



char *get_receipt_number() {

	FILE *f, *g;
	int number;
	char str[10];
	char *str1;
	char *pch;
	int w;
	char formated_rcp_num[6];

	memset(str, 0, sizeof(str));
	f = fopen("receipt.txt", "r+");
	if (f == NULL) {
		/*
		 * No receipt.txt create one
		 */
		g = fopen("receipt.txt", "w");
		if (g == NULL) {
			return 1;

		}
		fclose(g);
		strcpy(str, "0");
	} else {
		while (fgets(str, 100, f) != NULL) {
			printf("%s", str);
		}
		fclose(f);
	}

	number = atoi(str) + 1;

	memset(str, 0, sizeof(str));
	sprintf(str, "%d", number);
	remove("receipt.txt");
	f = fopen("receipt.txt", "w");
	if (f == NULL) {
		return 1;
	}
	fprintf(f, str);
	fclose(f);
	w =strlen(str);

	strcpy(formated_rcp_num,"");
	while(w<4){
		strcat(formated_rcp_num,"0");
		w++;
	}

	strcat(formated_rcp_num,str);
	//sprintf(recptnum, "%s%s-%s", z_number,serial_num, formated_rcp_num);
	sprintf(recptnum, "%s%s-%s","Alex","Alex","####");

   pch=malloc(10);
   pch=strstr(recptnum,"\n");
	while(pch!=NULL)
	{
		strncpy(pch,"",1);
		pch=strstr(recptnum,"\n");

	}

	memset(str,0,sizeof(str));
	strncpy(str,recptnum,4);
	str1=malloc(20);
	str1=strstr(recptnum,"8");
	strcat(str,str1);
	memset(recptnum,0,sizeof(recptnum));
	strcpy(recptnum,str);

	return recptnum;


	//return atof(str);;fo
	return recptnum;
}

int get_voided_number() {

	FILE *f, *g;
	int number;
	char str[10];
	char *str1;
	char *pch;
	int w;

	memset(str, 0, sizeof(str));
	f = fopen("voided_rc_num.txt", "r+");
	if (f == NULL) {
		/*
		 * No receipt.txt create one
		 */
		g = fopen("voided_rc_num.txt", "w");
		if (g == NULL) {
			return -1;

		}
		fclose(g);
		strcpy(str, "0");
	} else {
		while (fgets(str, 100, f) != NULL) {
			printf("%s", str);
		}
		fclose(f);
	}

	number = atoi(str) + 1;

	memset(str, 0, sizeof(str));
	sprintf(str, "%d", number);
	remove("voided_rc_num.txt");
	f = fopen("voided_rc_num.txt", "w");
	if (f == NULL) {
		return -1;
	}
	fprintf(f, str);
	fclose(f);
	return number;


	//return atof(str);;f
}
/*
 * Function to revert receipt no
 */
void revert_receipt_number() {
	FILE *f;
	int number;
	char str[10];
	memset(str, 0, sizeof(str));
	f = fopen("receipt.txt", "r+");
	if (f == NULL) {
		/*
		 * No receipt.txt create one
		 */
		return;
	} else {
		while (fgets(str, 100, f) != NULL) {
			printf("%s", str);
		}
		fclose(f);
		if (atoi(str) == 0)
			return;
	}

	number = atoi(str) - 1;
	memset(str, 0, sizeof(str));
	sprintf(str, "%d", number);
	remove("receipt.txt");
	f = fopen("receipt.txt", "w");
	if (f == NULL) {
		printf(" Not able to open");
		return;
	}
	fprintf(f, str);
	fclose(f);
}


/*
 * Z Number
 *
 *
 *
 */

void set_get_z_number(int increament_operation) {

	FILE *f, *g;
	int number;
	char str[10], str1[10];
	int w , x;
	char formated_rcp_num[10];

	memset(str, 0, sizeof(str));
	f = fopen("z_num.txt", "r+");
	if (f == NULL) {
		/*
		 * No receipt.txt create one
		 */
		g = fopen("z_num.txt", "w");
		if (g == NULL) {
			//return 1;
		}
		fclose(g);
		strcpy(str, "0");
	} else {
		while (fgets(str, 100, f) != NULL) {
			printf("%s", str);
		}
		fclose(f);
	}


	if(increament_operation)
	{
	number = atoi(str) + 1;

	memset(str, 0, sizeof(str));
	sprintf(str, "%d", number);
	remove("z_num.txt");
	f = fopen("z_num.txt", "w");
	if (f == NULL) {
		//return 1;
	}
	fprintf(f, str);
	fclose(f);
	}


	w =strlen(str);
x =0;
strcpy(str1, str);
	if(str[w-1] == '\n')
	{
/*		strcpy(str, "");
		while(x<(w-2)){*/
			str[w-1]= '\0';
			//x++;
		//}

	}
	w =strlen(str);
	printf("z Num : %d   1%s1\n",w, trim(str));
	strcpy(formated_rcp_num,"");
	while(w<4){
		strcat(formated_rcp_num,"0");
		w++;
	}
	//strcpy(z_number,formated_rcp_num);
	sprintf(z_number,"%s%s",formated_rcp_num,str);
	printf("z Num :  %s\n", z_number);
	//return formated_rcp_num;
	//strcat(formated_rcp_num,z_num);
	//sprintf(recptnum, "%s-%s-%s", serial_num, formated_rcp_num,time_for_receipt);
	//return atof(str);;fo
	//return recptnum;
}
