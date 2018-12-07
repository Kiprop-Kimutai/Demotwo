#include <stdio.h>   /* gets */
#include <stdlib.h>  /* atoi, malloc */
#include <string.h>  /* strcpy */

#include "../src/utilities/uthash.h"
#include "jsonread.h"




struct menu_path *menu_items_in_path = NULL;
void add_path(int menu_in_path_id, char *name) {
    struct menu_path *s;

    HASH_FIND_INT(menu_items_in_path, &menu_in_path_id, s);  /* id already in the hash? */
    if (s==NULL) {
      s = (struct menu_path*)malloc(sizeof(struct menu_path));
      s->id = menu_in_path_id;
      HASH_ADD_INT( menu_items_in_path, id, s );  /* id: name of key field */
    }
    strcpy(s->name, name);
    strcpy(s->current_head_name, current_menu);
}

struct menu_path *find_path_item(int menu_in_path_id) {
    struct menu_path *s;

    HASH_FIND_INT( menu_items_in_path, &menu_in_path_id, s );  /* s: output pointer */
    return s;
}

void delete_path(struct menu_path *user) {
    HASH_DEL( menu_items_in_path, user);  /* user: pointer to deletee */
    free(user);
}

void delete_all_in_path() {
  struct menu_path *current_user, *tmp;

  HASH_ITER(hh, menu_items_in_path, current_user, tmp) {
    HASH_DEL(menu_items_in_path,current_user);  /* delete it (menu_items_in_path advances to next) */
    free(current_user);            /* free it */
  }
}

void print_menu_items_in_path() {
    struct menu_path *s;
    printf("n\n\n\n\\n\n\nThus is the isy");
    for(s=menu_items_in_path; s != NULL; s=(struct menu_path*)(s->hh.next)) {
        printf("user id %d: name %s\n", s->id, s->name);
    }
}

int travese_path(int task , char * service_id) {
	 printf("n\n\n\n\\n\n\nThus is the isy");
	 printf("%d , %s",last_in_path, service_id);
	 printf("n\n\n\n\\n\n\nThus is the isy");
   // char in[10];
   // int id=1, running=1;
	 struct menu_path *s = NULL;

    //unsigned num_menu_items_in_path;
        switch(task)
        {
            case 1:
                add_path(last_in_path++,service_id);
                break;
            case 2:
            	delete_all_in_path();
            	//s = find_path_item(last_in_path);
                //printf("path_id: %s\n", s ? s->name : "service_id");
                break;
            case 3:
            	last_in_path--;
            	s = find_path_item(last_in_path);
            	//printf("user: %s\n", s ? s->name : "unknown");
            	//printf("I GET %s",s->name );

                if (s)
                {
                	printf("\nI GET %s\n",s->name );

                	delete_path(s);
                	last_in_path--;
                	s = find_path_item(last_in_path);

                	if(s){
                		strcpy(current_menu,s->current_head_name);
                		if(last_in_path>1)
                			if(online)
                				design_url("SUBSERVICE",s->name);
                			else
                				get_offline_menu("SUBSERVICE",s->name);
                		else if( last_in_path == 0 || last_in_path == 0)
                		{
                			strcpy(main_service_level, "0");

                			if(online)
                				design_url("SUBSERVICE",s->name);
                			else
                				get_offline_menu("SUBSERVICE",s->name);

                			strcpy(main_service_level, "1");
                		}

                		else if ( last_in_path < 0)
                		{
                			delete_all_in_path();
                			if(online)
                			design_url("SERVICEDATA","j");
                			else
                				get_offline_menu("SERVICEDATA","j");
                			last_in_path=-1;
                		}
                		last_in_path++;

                	}
                	number_of_trans = 0;
                	strcpy(charge,"");
                	strcpy(pmode,"");
                	strcpy(payment_ref_code,"");
                }

               // else printf("id unknown\n");
                break;
        }

       print_menu_items_in_path();

   // delete_all();  /* free any structures */
    return 0;
}
