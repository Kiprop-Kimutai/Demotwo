
#include "menu_hash.h"
struct menu_structure *menu_items_structure = NULL;
void add_menu(char *menu_level, char *name_provided ) {
    struct menu_structure *s;

    HASH_FIND_STR(menu_items_structure, menu_level, s);  /* id already in the hash? */
    if (s==NULL) {
      s = (struct menu_structure*)malloc(sizeof(struct menu_structure));
     // s->id = user_id;
      strcpy(s->serviceId,menu_level );
      HASH_ADD_STR( menu_items_structure, serviceId, s );  /* id: name of key field */
    }
    strcpy(s->serviceName, name_provided);
}



void add_code(char *menu_level, char *code) {
    struct menu_structure *s;

    HASH_FIND_STR(menu_items_structure, menu_level, s);  /* id already in the hash? */
    if (s==NULL) {
      s = (struct menu_structure*)malloc(sizeof(struct menu_structure));
     // s->id = user_id;
      strcpy(s->serviceId,menu_level );
      HASH_ADD_STR( menu_items_structure, serviceId, s );  /* id: name of key field */
    }
    strcpy(s->serviceCode, code);
}



void add_has_next(char *menu_level, int Next) {
    struct menu_structure *s;

    HASH_FIND_STR(menu_items_structure, menu_level, s);  /* id already in the hash? */
    if (s==NULL) {
      s = (struct menu_structure*)malloc(sizeof(struct menu_structure));
     // s->id = user_id;
      strcpy(s->serviceId,menu_level );
      HASH_ADD_STR( menu_items_structure, serviceId, s );  /* id: name of key field */
    }
    strcpy(s->has_next, Next);
}


struct menu_structure *find_menu(char *menu_id) {
    struct menu_structure *s;

    HASH_FIND_STR( menu_items_structure, menu_id, s );  /* s: output pointer */
    return s;
}

void delete_menu(struct menu_structure *user) {
    HASH_DEL( menu_items_structure, user);  /* user: pointer to deletee */
    free(user);
}

void delete_all() {
  struct menu_structure *current_user, *tmp;

  HASH_ITER(hh, menu_items_structure, current_user, tmp) {
    HASH_DEL(menu_items_structure,current_user);  /* delete it (users advances to next) */
    free(current_user);            /* free it */
  }
}

void print_menus() {
    struct menu_structure *s;

    for(s=menu_items_structure; s != NULL; s=(struct menu_structure*)(s->hh.next)) {
        printf("user id %s: \tname %s : \tname %s\n", s->serviceId, s->serviceName, s->serviceCode);

    }
}

int name_sort(struct menu_structure *a, struct menu_structure *b) {
    return strcmp(a->serviceName,b->serviceName);
}

int id_sort(struct menu_structure *a, struct menu_structure *b) {
    return (a->serviceId - b->serviceId);
}

void sort_by_name() {
    HASH_SORT(menu_items_structure, name_sort);
}

void sort_by_id() {
    HASH_SORT(menu_items_structure, id_sort);
}
void menu_array (char* current_menu,int  i)
{

	struct menu_structure *s;
	 char* t;
    int x;
    for(x = 0;x<i;x++){

    	HASH_FIND_STR( menu_items_structure, one_level_deep(current_menu, x), s );
    	strcpy(listed_menu[x], s->serviceName);

    	printf("\nMenu : %s\n",listed_menu[x]);


    }
}
/*
int main(int argc, char *argv[]) {
    char in[10];
    char menu_level[30];
    int id=1, running=1;
    struct menu_structure *s;
    unsigned num_users;

    while (running) {
        printf(" 1. add user\n");
        printf(" 2. add/rename user by id\n");
        printf(" 3. find user\n");
        printf(" 4. delete user\n");
        printf(" 5. delete all users\n");
        printf(" 6. sort items by name\n");
        printf(" 7. sort items by id\n");
        printf(" 8. print users\n");
        printf(" 9. count users\n");
        printf("10. quit\n");
        gets(in);
        switch(atoi(in)) {
            case 1:
                printf("name :");
                scanf("%s", menu_level);
                printf("name :");
                scanf("%s", in);
                add_user(menu_level, in);

                break;
            case 2:
                printf("id?\n");
                gets(in); id = atoi(in);
                printf("name?\n");
                add_user(id, gets(in));
                break;
            case 3:
                printf("id?\n");
                s = find_user(atoi(gets(in)));
                printf("user: %s\n", s ? s->name : "unknown");
                break;
            case 4:
                printf("id?\n");
                s = find_user(atoi(gets(in)));
                if (s) delete_user(s);
                else printf("id unknown\n");
                break;
            case 5:
                delete_all();
                break;
            case 6:
                sort_by_name();
                break;
            case 7:
                sort_by_id();
                break;
            case 8:
                print_menus();
                break;
            case 9:
                num_users=HASH_COUNT(users);
                printf("there are %u users\n", num_users);
                break;
            case 10:
                running=0;
                break;
        }
    }

    delete_all();   free any structures
    return 0;
}



*/

char* one_level_deep(char *s1,int  i)
{
	char* s2[3];
    size_t len1 = strlen(s1);
    size_t len2 ;

    snprintf(s2, 3,"%d",i);
    len2 = strlen(s2);
    char *result = malloc(len1+len2+1);
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);
    return result;
}

char* one_level_up(char *s1, int i)
{
	size_t len2 = strlen(s1);
	char *result = malloc(len2-i);
    memcpy(result, s1, len2 - i);
    result[strlen(s1) - i] = '\0';
    return result;
}
