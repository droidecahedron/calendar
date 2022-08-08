
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int main(void)
{
    bool total_length_check,title_length_check,location_length_check,valid_command_check;

    //read the file and store its contents to a buffer
    char input_buffer[49]; //the input chars, process 100 emails, max is 49 [40 after you remove subject: ]
    char data_to_copy[49];
    
    while(fgets(input_buffer,sizeof(input_buffer)+1,stdin) != NULL)
    {

        total_length_check = (strlen(input_buffer) == 49);
        title_length_check = (*(input_buffer + 10) == ',');
        location_length_check = (*(input_buffer + 21) == ',');
        valid_command_check = (*(input_buffer + 9) == 'C' || *(input_buffer + 9) == 'D' || *(input_buffer + 9) == 'X') ;

        if(total_length_check && title_length_check && location_length_check && valid_command_check)
        {
            strcpy(data_to_copy,input_buffer+9); // copies it to the output buffer, ignoring subject:
            puts(data_to_copy);//fputs, unlike puts, does not append a new line.
        }

    }


    return 0;
}
