#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

extern int randomNumberGen(int seed);
extern int create_list_of_numbers();
extern int add_number_to_list(int number);
extern char *get_poetry_line(int num);
extern int add_strings_to_list();
extern char *get_first_word(char *line);


struct num_and_string
{
    int number;
    char *poetry;
    struct num_and_string *next;
};

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
    FILE *fp;
    // Initialize list indicating that the list is empty
    ns_list = NULL;
    
    
    for (i=0; i<10; i++)
    {
        random_number = randomNumberGen(i);
        printf("seed = %d, rand = %d\n", i, random_number);
        
        strcpy(filename, "file_");
        len = strlen(filename);
        filename[len] = '0' + i;
        filename[len+1] = '\0';             // add null char at the end
        strcat(filename, ".txt");
        //printf ("filename = %s\n", filename);
        
        fp = fopen(filename, "w");          // creates file in WRITE mode with filename
        if ( fp == NULL )                   // if file pointer is not valid-
        {
            printf("Error opening file: %s\n", filename);
            return 1;
        }
        
        fclose(fp);                         //if the file stays open, it cant be accessed
    }
    
    create_list_of_numbers();    //create link list and copy rand numbers into it
    add_strings_to_list();
    
    getc(stdin);
    
    return 0;
    
}

int randomNumberGen(int seed)
{
    int number;
    number = seed * 1103515245 + 12345;
    return (unsigned int)(number/65536) % 32768;
}

int create_list_of_numbers()
{
    FILE *fp;
    uint32_t i, random_number;
    unsigned long len;
    char filename[20];
    struct num_and_string *temp_node;
    
    for (i=0; i<10; i++)
    {
        random_number = randomNumberGen(i);
        strcpy(filename, "file_");
        len = strlen(filename);
        filename[len] = '0' + i;
        filename[len+1] = '\0';             // add null char at the end
        strcat(filename, ".txt");
        
        fp = fopen(filename, "r");          // creates file in WRITE mode with filename
        if ( fp == NULL )                   // if file pointer is not valid-
        {
            printf("Error opening file: %s\n", filename);
            return 1;
        }
        fscanf(fp, "%d", &random_number);
        printf ("filename = %s %d\n", filename, random_number);
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
    temp_node = ns_list;        //the following logic traverses the list
    
    while (temp_node != NULL)   //if next == NULL, were at the end of the list
    {
        temp_node->poetry = get_poetry_line(i);
        i++;
        if(temp_node->poetry !=NULL)
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
    if(num > 9) {
        printf ("wrong arg #: %d\n", num);    }
    
    fp = fopen("ten_green_bottles.txt", "r");
    if(fp == NULL) {
        printf("error: no open\n");
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
                return NULL;
            }
            strcpy(poetry_line, line);
            fclose(fp);
            return poetry_line;    //to calling function
        }
        }
    }
    fclose(fp);
    return NULL;
}

char *get_first_word(char *line)
{
    static char word[10];   //calling function can access this STATIC even tho is is declared in the called function. STSTIC vars stay alive as long as the program is alive
    int i, j = 0;
    
    while ( (line[i] != 0) && isspace(line[i])){     //runs while not 0 and while spaces.
        i++;  //
        if(line[i] == 0)  //we hit the end of the line
        {
            return NULL;
        }                 //we hit a letter!
        while ( (line[i] !=0) && isalpha(line[i]))
        {
            word[j] = line[i];
            i++, j++;
        }
        word[j] = 0;
        
        return word;
    }
    
    return 0;
}









