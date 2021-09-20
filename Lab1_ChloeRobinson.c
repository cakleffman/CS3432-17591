#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true if c is a whitespace characer
   ('\t' or ' ').*/
bool delim_character(char c)
{
	return (c == '\t' || c == ' ');
}

/* Return true if c is a non-whitespace
   character */
bool non_delim_character(char c)
{
	return (c != '\t' && c != ' ');
}

/* Returns a pointer to the first character of the next
   space-separated word*/
char *word_start(char* str)
{
	while(delim_character(*str)){
    str++;
	}
	return str;
}

/* Returns a pointer to the first space character of the zero
terminated string*/
char *end_word(char* str){
	while(non_delim_character(*str)){
		if(*str == '\0'){
		break;
		}
		str++;
	}
  
	return str;
}

// Returns the number of words, or suubsets separated by white-space
int count_tokens(char* str)
{
	int counter = 0;
	while(*str != '\0'){
		if(delim_character(*str)){
			counter++;
		}
		str++;
	}
	return ++counter;
}

//Returns a pointer array, which points to the new subset/word
char *copy_str(char *inStr, short len)
{
	char *new_string = (char*) malloc(len * sizeof(char));
	for(int i = 0; i < len; i++){
		new_string[i] = *inStr;
		inStr++;
	}
  
	return new_string;
}

 //Recursive method to generate all tokens
char** tokenize(char** tokenizer, char* start, char* end, int idx, int tokens)
{
	if (idx >= tokens){
		return tokenizer;
	}
  //get the length of the current word
	short length = end - start;
  
  //set a pointer to point at this word
	char *new_string = (char*) malloc(length * sizeof(char));
	new_string = copy_str(start, length);

  //set tokenizer at index 0 to point to this pointer
	tokenizer[idx] = new_string;
  
  //get start of next word
	char* newstart = word_start(end);
  
  //get end of next word
	char* newend = end_word(newstart);
  
  //recursively call in this new word
	return tokenize(tokenizer, newstart, newend, idx + 1, tokens);
}

//prints all tokens
void print_all_tokens(char** tokens, int limit)
{
	int i = 0;

	while(i < limit){
		printf("Tokens[%d]: %s\n", i, tokens[i]);
		i++;
	}
  
}


 int main()
 {
   //Input from user
	char words[50];
	printf("Please enter the input string: ");
	fgets(words, sizeof(words), stdin);

  //declare pointer to start of first word
	char *start;
	start = words;
  //declare pointer to end of first word
	char* endword;
	endword = end_word(start);

  //get the number of words in the string
	int tokens = count_tokens(start);
  //initialize a pointer to a list of pointers based on number of words
	char** tokenizer = (char**) malloc(tokens * sizeof(char));

  //fill the list of pointers with the pointers from each word
	tokenizer = tokenize(tokenizer, start, endword, 0, tokens);

  //print all of the tokens, or words
	print_all_tokens(tokenizer, tokens);

	return 0;

}