#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>



struct num_and_string
{
    int number;
    char *poetry;
    struct num_and_string *next;
};


extern int randomNumberGen(int seed);
extern int create_list_of_numbers();
extern int add_number_to_list(int number);
extern char *get_poetry_line(int num);
extern int add_strings_to_list();
extern char *get_first_word(char *line);
extern void sort_list_ascending_order();
extern void insert_node_ascending(struct num_and_string *new_node);
extern void insert_node_descending(struct num_and_string *new_node);
extern void sort_list_descending_string();
extern int get_number(char *word);


struct num_and_string *ns_list;     //num and string list
//all we need is the address of the first element

char *number_map[10] = {"one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten" };          //declared as a global varible


int main(int argc, char* argv[])
{
    int random_number;
    int i;
    unsigned long len;
    char filename[20];
    struct num_and_string *temp_node;
    FILE *fp;
    // Initialize list indicating that the list is empty
   // ns_list = NULL;
    
    
    
    
    for (i=0; i<10; i++)
    {
        random_number = randomNumberGen(i);
        //       printf("seed = %d, rand = %d\n", i, random_number);
        //assert(argv[1] != NULL);
        strcpy(filename, argv[1]);
        len = strlen(filename);
        filename[len] = '0' + i;
        filename[len+1] = '\0';             // add null char at the end
        strcat(filename, ".txt");
        //printf ("filename = %s\n", filename);
        fp = fopen(filename, "w");          // creates file in WRITE mode with filename
        if ( fp == NULL )                   // if file pointer is not valid-
        {
            printf("66Error opening file: %s\n", filename);
            return 1;
        }
        fprintf(fp, "%d\n", random_number);
        fclose(fp);                         //if the file stays open, it cant be accessed
    }
    
    create_list_of_numbers();    //create link list and copy rand numbers into it
    add_strings_to_list();
    sort_list_ascending_order();
    sort_list_descending_string();
    
    temp_node = ns_list;
    
    while (temp_node != NULL)   //if next == NULL, were at the end of the list
    {
        printf("list_member: number: %s\n", temp_node->poetry);
        temp_node = temp_node->next;  //moves to the next element in the list
    }
    
    getc(stdin);
    
    return 0;
    
}

int randomNumberGen(int seed)
{
    int number;
    number = seed * 1103515245 + 12345;
    return (unsigned int)(number/65536) % 32768;
}


int create_list_of_numbers(char *file_prefix)
{
    FILE *fp;
    uint32_t i, random_number;
    unsigned long len;
    char filename[20];
    struct num_and_string *temp_node;
    
    for (i=0; i<10; i++)
    {
        strcpy(filename, "file_");
        len = strlen(filename);
        filename[len] = '0' + i;
        filename[len+1] = '\0';             // add null char at the end
        strcat(filename, ".txt");
        
        
        fp = fopen(filename, "r");
        if ( fp == NULL )                   // if file pointer is not valid-
        {
            printf("120Error opening file: %s\n", filename);
            return 1;
        }
        fscanf(fp, "%d", &random_number);
        printf ("filename: %s, number: %d\n", filename, random_number);
        add_number_to_list(random_number);
        
        fclose(fp);
    }
    
    temp_node = ns_list;
    
    while (temp_node != NULL)   //if next == NULL, were at the end of the list
    {
        printf("list_member: number: %d\n", temp_node->number);
        temp_node = temp_node->next;  //moves to the next element in the list
    }
    
    return 0;
    
}

int add_number_to_list(int number)
{
    struct num_and_string *new_node, *temp_node;
    
    new_node = (struct num_and_string *)malloc(sizeof(struct num_and_string));
    if(new_node == NULL)          //if malloc returns a NULL, it didnt work
    {
        printf("Error allocating memory: %d\n", number);
        return 1;
    }
    
    new_node->number = number;
    new_node->poetry = NULL;
    new_node->next = NULL;
    
    if (ns_list == NULL)     // the list is empty
    {
        ns_list = new_node;
        return 0;
    }
    else  // list is not empty
    {
        temp_node = ns_list;
        
        while (temp_node->next != NULL)   //if next == NULL, were at the end of the list
        {
            temp_node = temp_node->next;  //moves to the next element in the list
        }
        //Here, temp_node is the last element in the list
        
        
        temp_node->next = new_node;
    }
    return 0;
}

int add_strings_to_list()
{
    struct num_and_string *temp_node;
    int i = 0;
    //
    temp_node = ns_list;        //the following logic traverses the list
    if (temp_node == NULL) printf ("temp node NULL");
    
    
    while (temp_node != NULL)   //if next == NULL, we're at the end of the list
    {
        temp_node->poetry = get_poetry_line(i);
        i++;
        //if(temp_node->poetry !=NULL)
        printf("i = %d, %s\n",i,temp_node->poetry);
        temp_node = temp_node->next;  //moves to the next element in the list
    }
    return 0;
}
char *get_poetry_line(int num)    //char* returns pointer to string
{
    FILE *fp;
    char line[80];
    char *word, *poetry_line;
    
    if(num > 9)
    {
        printf ("wrong arg num: %d\n", num);
        return NULL;
    }
    
    fp = fopen("ten_green_bottles.txt", "r");
    if(fp == NULL)
    {
        printf("error: cannot open file: ten_green_bottles.txt");
        return NULL;
    }
    
    while( fgets(line, 70, fp) )
    {
        word = get_first_word(line);
        
        if(word != NULL)
        {
            if(strcmp(word, number_map[num]) == 0)
            {
                poetry_line = (char *)malloc(strlen(line)+1);
                if (poetry_line == NULL);
                {
                    printf("error in get_poetry_line: cannot allocate mem: %d\n", num);
                    fclose(fp);
                    return NULL;
                }
                strcpy(poetry_line, line);
                fclose(fp);
                return poetry_line;    //to calling function
                printf("poetry line:%s\n",poetry_line);
                free(poetry_line);
            }
        }
    }
    fclose(fp);
    return NULL;
}

char *get_first_word(char *line)
{
    static char word[10];
    int i, j;
    
    i = 0; j = 0;
    
    while ( (line[i] != 0) && isspace(line[i]))        //corrupt pointer??? why???
        i++;  //
    if(line[i] == 0)  //we hit the end of the line
    {
        return NULL;
    }                 //we hit a letter!
    while ( (line[i] !=0) && isalpha(line[i]))
    {
        word[j] = line[i];
        i++ ;
        j++ ;
    }
    
    word[j] = 0;
    return word;
}



void sort_list_ascending_order()
{
    struct num_and_string *curr, *prev, *temp_node;
    
    if (ns_list == NULL)
        return;
    
    curr = prev = ns_list;
    
    while (curr != NULL)
    {
        if (curr->number >= prev->number) //already in ascending order
        {
            prev = curr;
            curr = curr->next;
        }
        else // not ascending    curr->number < prev->number
        {
            temp_node = curr;
            prev->next = curr->next;
            curr = curr->next;
            insert_node_ascending(temp_node);
        }
    }
}



void insert_node_ascending(struct num_and_string *new_node)
{
    struct num_and_string *iprev, *icurr;
    if (ns_list == NULL)
    {
        return;
    }
    icurr = iprev = ns_list;
    
    while (icurr != NULL)
    {
        if (new_node->number > icurr->number)
        {
            iprev = icurr;
            icurr = icurr->next;
        }
        else // new-node has to be inserted before curr node
            //because this new_node->number <= icurr->number
        {
            if (icurr == iprev)
            {
                new_node->next = icurr;
                ns_list = new_node;     //new node is at beginning of list now
            }
            else
            {
                new_node->next = icurr;
                iprev->next = new_node;
            }
            return;
        }
    }
    //new node has to be inserted at the end of the list
    new_node->next = NULL;
    iprev->next = new_node;
    return;
}









void sort_list_descending_string()
{
    struct num_and_string *curr, *prev, *temp_node;
    char *word;
    int curr_number, prev_number;
    
    if (ns_list == NULL)
        return;
    
    curr = prev = ns_list;
    
    while (curr != NULL)
    {
        word = get_first_word(curr->poetry);
        curr_number = get_number(word);
        
        word = get_first_word(prev->poetry);
        prev_number = get_number(word);

        
        if (curr->number >= prev->number) //already in descending order
        {
            prev = curr;
            curr = curr->next;
        }
        else // not descending    curr_number > prev_number
        {
            temp_node = curr;
            prev->next = curr->next;
            curr = curr->next;
            insert_node_descending(temp_node);
        }
    }
}



void insert_node_descending(struct num_and_string *new_node)
{
    struct num_and_string *iprev, *icurr;
    char *word;
    int icurr_number, iprev_number, new_number;
    if (ns_list == NULL)
    {
        return;
    }
    icurr = iprev = ns_list;
    
    while (icurr != NULL)
    {
        word = get_first_word(icurr->poetry);
        icurr_number = get_number(word);
        
        word = get_first_word(iprev->poetry);
        iprev_number = get_number(word);
        
        word = get_first_word(new_node->poetry);
        new_number = get_number(word);

        
        if (new_number < icurr->number)
        {
            iprev = icurr;
            icurr = icurr->next;
        }
        else // new-node has to be inserted before curr node
            //because this new_node->number >= icurr_number
        {
            if (icurr == iprev)
            {
                new_node->next = icurr;
                ns_list = new_node;     //new node is at beginning of list now
            }
            else
            {
                new_node->next = icurr;
                iprev->next = new_node;
            }
            return;
        }
    }
    //new node has to be inserted at the end of the list
    new_node->next = NULL;
    iprev->next = new_node;
    return;
}


int get_number(char *word)
{
    int i;
    
    for (i = 0; i < 10; i++)
    {
        if ( strcmp(word, number_map[i]) == 0)
        {
            return i++;     //because number map 0 corresponds to string 'one'
        }
    }
    return 0;
}

    








