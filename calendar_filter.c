
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//doubly linked list
struct Node
{ 
	char command;	
	char title[10];
	char date[10];
	char time[5];
	char location[10];
	struct Node* next; 
	struct Node* prev;
}; 

int event_counter = 0;

//adds to linked list
void append(struct Node** head_ref, char* title, char* date, char* time, char* location);
//checks earliest event on that date. if this is the earliest, returns 1
int check_earliest(struct Node** head_ref, char* date_to_check, char* time_to_check);
//looks through linked list for matching title, returns 1 if it found a match and swapped it.
int command_x(struct Node** head_ref, char* title_to_check, char* date, char* time, char* location);
//delete an event with a matching title, date, and time. returns 1 if it deletes
int command_d(struct Node** head_ref, char* title_to_check, char* date_to_check, char* time_to_check);
//after an event is deleted, find new earliest or find if we deleted all events on a date
void find_earliest_deleted(struct Node** head_ref, char* date_to_check);
//check if this was the only event on that day. does appropriate stuff.
int check_only_event(struct Node** head_ref,char* date_to_check);


int main(void)
{
	puts("working");
    
    char input_buffer[49];
    char command;
    char title[10];
    char date[10];
    char time[5];
    char location[10];
    
    int x;
    
    int was_earliest = 0;
    
    struct Node* head = NULL; //create an empty linked list
    
    
    while(fgets(input_buffer,sizeof(input_buffer)+1,stdin) != NULL) // get input
    {
    	was_earliest = 0; // used in delete
    
    	command = input_buffer[0];
    	memcpy(title,&input_buffer[2],10);
    	memcpy(date,&input_buffer[13],10);
    	memcpy(time,&input_buffer[24],5);
    	memcpy(location,&input_buffer[30],10);
    	
    	if(event_counter == 0)
    	{
    		//puts("Created!");
    		append(&head, title, date, time, location);
    		//first event, print it
		for(int i=0;i<10;i++) putchar(date[i]);
		putchar(',');
		for(int i=0;i<5;i++) putchar(time[i]); 
		putchar(',');
		for(int i=0;i<10;i++) putchar(location[i]);
		putchar('\n');
    	
    	}
    	else
    	{
	//look at the calendar, see if the date exists or not
	//if it exists, then see if its the earliest on that date
		if(command == 'C')
		{
			
			append(&head, title, date, time, location);
			//puts("Created!");
			if(check_earliest(&head,date,time) == 1) // if its earliest, print it
			{
				for(int i=0;i<10;i++) putchar(date[i]);
				putchar(',');
				for(int i=0;i<5;i++) putchar(time[i]); 
				putchar(',');
				for(int i=0;i<10;i++) putchar(location[i]);
				putchar('\n');			
			}
			
		}
		
		//change an event with a matching title
		else if(command == 'X')
		{
			if(command_x(&head,title,date,time,location) == 1) // returns 1 if successful replace
			{
				if(check_earliest(&head,date,time) == 1) // if its earliest, print it
				{
					//puts("Replaced!");
					for(int i=0;i<10;i++) putchar(date[i]);
					putchar(',');
					for(int i=0;i<5;i++) putchar(time[i]); 
					putchar(',');
					for(int i=0;i<10;i++) putchar(location[i]);
					putchar('\n');			
				}
			}
		
		}
		else if(command == 'D')
		{
			if(check_earliest(&head,date,time) == 1)
				was_earliest = 1;
				
			if(command_d(&head,title,date,time) == 1)
			{
				//if it deletes something..
				//puts("Deleted!");	
				//check if this was the only event on that day.
				if(check_only_event(&head,date) == 0)
					if(was_earliest == 1)
					{
						
						// find earliest other event on this date and print
						find_earliest_deleted(&head,date); 
					}
					
			}
		
		
		}
	}
    
    	
    	
    	

    }


    return 0;

}



/* Given a reference (pointer to pointer) to the head 
   of a list and an int, appends a new node at the end  */
void append(struct Node** head_ref, char* title, char* date, char* time, char* location)
{ 
	/* 1. allocate node */
	struct Node* new_node = (struct Node*) malloc(sizeof(struct Node)); 

	struct Node *last = *head_ref;  /* used in step 5*/

	/* 2. put in the data  */
	memcpy(new_node->title,title,10);
	memcpy(new_node->date,date,10);
	memcpy(new_node->time,time,5);
	memcpy(new_node->location,location,10);

	/* 3. This new node is going to be the last node, so make next  
	  of it as NULL*/
	new_node->next = NULL; 

	/* 4. If the Linked List is empty, then make the new node as head */
	if (*head_ref == NULL) 
	{ 
		*head_ref = new_node; 
		new_node->prev = NULL;
		event_counter++;
		return; 
	}   

	/* 5. Else traverse till the last node */
	while (last->next != NULL) 
		last = last->next; 

	/* 6. Change the next of last node */
	last->next = new_node; 
	new_node->prev = last; // the new nodes previous is the last node.
	
	event_counter++;
	return;     
} 


int check_earliest(struct Node** head_ref, char* date_to_check, char* time_to_check)
{

	struct Node *temp = *head_ref;
	
	//if its the only thing, its the earliest thing.
	if(temp->next == NULL) // if theres only 1 thing in the linked list
		return 1;
	//traverse linked list
	do  
	{	
		//check if dates matched
		if(memcmp(date_to_check,temp->date,10) == 0)
		{
			//check if it is earlier
			// if temp->time is EARLIER (less) so time_to_check is not earliest, return
			if(memcmp(time_to_check,temp->time,5) > 0) 
				return 0;
		}
		temp = temp->next; 
	} while (temp->next != NULL);
	// if it gets here, nothing else in the linked list has this date, so its earliest by default.
	//or nothing else had an earlier time.
	return 1; 

}

//return 1 if successful
//return 0 if no matches
int command_x(struct Node** head_ref, char* title_to_check, char* date, char* time, char* location)
{

	struct Node *temp = *head_ref;
	
	if(temp->next == NULL) // if theres only 1 thing in the linked list
	{
		//check if title matched
		if(memcmp(title_to_check,temp->title,10) == 0)
		{
			//if title matches, overwrite this node's details
			memcpy(temp->date,date,10);
		    	memcpy(temp->time,time,5);
		    	memcpy(temp->location,location,10);
		    	return 1;
			
		}	
	
	}
	
	while (temp->next != NULL) 
	{	
		//check if title matched
		if(memcmp(title_to_check,temp->title,10) == 0)
		{
			//if title matches, overwrite this node's details
			memcpy(temp->date,date,10);
		    	memcpy(temp->time,time,5);
		    	memcpy(temp->location,location,10);
		    	return 1;
			
		}
		temp = temp->next; 
	}
	return 0;
}



int command_d(struct Node** head_ref, char* title_to_check, char* date_to_check, char* time_to_check)
{
	struct Node *temp = *head_ref;
	while (temp->next != NULL) 
	{	
		//check if title matched
		if(memcmp(title_to_check,temp->title,10) == 0)
		{
			//if title matches, check date
			if(memcmp(date_to_check,temp->date,10) == 0)
			{
				//if time matches, delete
				if(memcmp(time_to_check,temp->time,5) == 0)
				{
				
					event_counter--; // i still havent used this but idk
					temp->next->prev = temp->prev;
					temp->prev->next = temp->next;
					return 1;
				}			
			}	
		}
		temp = temp->next; 
	}
	//if it got here, it didnt delete anything.
	return 0;
}


//after an event is deleted, find new earliest
void find_earliest_deleted(struct Node** head_ref, char* date_to_check)
{
	struct Node *temp = *head_ref;
	struct Node *earliest = temp;
	
	while (temp->next != NULL) 
	{	
		//check for matching date
		if(memcmp(date_to_check,temp->date,10) == 0)
		{
			earliest = temp;
			temp = temp->next;
			break;
		}
		
		temp = temp->next;
	}
	
	//check the rest of the list for any matches
	while(temp->next != NULL)
	{
		//found another match
		if(memcmp(date_to_check,temp->date,10)==0)
		{
			//check if temp is earlier. if its earlier, save node
			if(memcmp(earliest->time,temp->time,5) > 0) // str2 is less than str1 if >0
				earliest = temp;
		}
		
		temp = temp->next;
	
	}

	//print earliest's time date and stuff
	for(int i=0;i<10;i++) putchar(earliest->date[i]);
	putchar(',');
	for(int i=0;i<5;i++) putchar(earliest->time[i]); 
	putchar(',');
	for(int i=0;i<10;i++) putchar(earliest->location[i]);
	putchar('\n');		
	return; // if it gets here, nothing else in the linked list has this date, so its earliest by default.
	
	
}



int check_only_event(struct Node** head_ref,char* date_to_check)
{
	struct Node *temp = *head_ref;
	char temp_time[10];
	int date_match = 0;
	while (temp->next != NULL) 
	{	
		//check for matching date
		if(memcmp(date_to_check,temp->date,10) == 0)
		{
			//if date matches, it wasnt the only event.
			return 0;
		}
		temp = temp->next;
	}
	
	
	//if it got here, nothing had a matching date after we deleted it
	//so we deleted the only event on that day.
	for(int i=0;i<10;i++) putchar(date_to_check[i]);
	putchar(',');
	putchar('-');
	putchar('-');
	putchar(':');
	putchar('-');
	putchar('-');	
	putchar(',');
	puts("NA");
	return 1;

}
