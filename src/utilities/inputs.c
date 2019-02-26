/*
 * author linux
 *
 * To change this generated comment edit the template variable "comment":
 * Window > Preferences > C/C++ > Editor > Templates.
 */



#include "inputs.h"
#include "../common_includes.h"


int menu_terminal(char * title  ,const char  menus[][100] , int menu_size){
	int choice;
	int menu_count= 0;
	do{
		/*Displaying on screen*/
		printf("------------------\n");
		printf(" %s\n",title);
		printf("------------------\n");
		for(menu_count=0;menu_count<menu_size;menu_count++)
			printf("%d) %s\n",menu_count+1,menus[menu_count]);

		/*getting input*/
		printf("Your choice : ");
		scanf("%d" ,&choice);

		system("clear");
		//getchar();

		//printf ("Menusize %d \n Choice %d",menu_size,choice );
		if(choice>0 && choice<=menu_size)
		{
			return choice;
		}
		else
			printf("Invalid choice. Please try  again\n");
		getchar();

		{

		}

		//system("PAUSE");
	}
	while(choice!=4);

	return 0;
}


char ** getData_terminal(char * title  ,const char  data_requested[][100] , int menu_size){

	char ** return_string = 0;
	char str[100];
	int menu_count= 0;

	return_string = realloc(return_string, (  menu_size + 1 ) * sizeof( char * ));


	/*Displaying on screen*/
	printf("------------------\n");
	printf(" %s\n",title);
	printf("------------------\n");
	for(menu_count=0;menu_count<menu_size;menu_count++)
	{
		printf("%d) %s\n",menu_count+1,data_requested[menu_count]);
		/*getting input*/
		printf("%s: " , data_requested[menu_count]);
		scanf("%s" ,str);
		return_string[menu_count] = malloc( strlen(  str) + 1 );
		strcpy( return_string[menu_count],  str);
	}

	return return_string;
}

void display_info_terminal(int title_print , char * title ,  char * message  , int clear ){
	if(1)
		system("clear");

	if(title_print)
	{
		printf("------------------\n");
		printf(" %s\n",title);
		printf("------------------\n");
	}
	printf(" %s\n",message);

	return;
}
