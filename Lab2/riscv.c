#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char** tokens);
void print_regs();
bool str_equals(char str1[], char str2[]);
char** parse(char instruction[]);

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}


/*
This method takes in an array of tokens, each token is a segment of the current instruction,
If the token at index 0 is either ADD, ADDI, LW, or SW it will interpret the 
instruction and return true.
If it not any of the listed instructions it will only return false.*/

bool interpret(char** tokens){
	
	int rd; //destination register
	int rs1; //source register 1
	int rs2; //source register 2
	int imm; //immediate value
	int address; //address for calling read and write to memory
	char* mem_file = "mem.txt"; //memory text file we are altering 
		
	//ADD RD RS1 RS2 --> ADD X6 X0 X29
	if(str_equals(tokens[0], "ADD")){ //if tokens at index 0 is the ADD instruction
		rd = atoi(tokens[1]); //set destination register to tokens at index 1
		rs1 = atoi(tokens[2]); //set source register 1 to tokens at index 2
		rs2 = atoi(tokens[3]); //set source register 2 to tokens at index 3
			
		reg[rd] = reg[rs1] + reg[rs2]; //the destination register will now hold source 1 plus source 2
		
	}
	//ADDI RD RS1 IMM --> ADDI X6 X6 329
	else if(str_equals(tokens[0], "ADDI")){
		rd = atoi(tokens[1]); //set destination register to tokens at index 1
		rs1 = atoi(tokens[2]); //set source register 1 to tokens at index 2
		imm = atoi(tokens[3]); //set the imediate value to tokens at index 3
			
		reg[rd] = reg[rs1] + imm; //the destination register will now hold source 1 plus the immediate
		
	}
	//LW RD RS1 IMM --> LW X7 1000(X5)
	else if(str_equals(tokens[0], "LW")){//read from memory
		rd = atoi(tokens[1]); //set destination register to tokens at index 1
		rs1 = atoi(tokens[2]); //set source register 1 to tokens at index 2
		imm = atoi(tokens[3]); //set the imediate value to tokens at index 3
			
		address = reg[rs1] + imm; //set the address to the source 1 plus the immediate to get the offset
		
		int32_t read = read_address(address, mem_file); //call read address to get the value located at the address
		
		reg[rd] = read; //the destination register is now holding the new value from memory
		
	}
	//SW RD RS1 IMM --> SW X5 2000(X0)
	else if(str_equals(tokens[0], "SW")){//store to memory
		rd = atoi(tokens[1]); //set destination register to tokens at index 1
		rs1 = atoi(tokens[2]); //set source register 1 to tokens at index 2
		imm = atoi(tokens[3]); //set the imediate value to tokens at index 3
			
			
		address = reg[rs1] + imm; //set the address to the source 1 plus the immediate to get the offset
		
		write_address(reg[rd], address, mem_file); //call write address with the destination register being the data to write, and the calculated address 
		//write address will store the value of the destination register into the address of memory
		
	}
	//if not a valid instruction, return false
	else if(!(str_equals(tokens[0], "ADD") || str_equals(tokens[0], "ADDI") || str_equals(tokens[0], "LW") || str_equals(tokens[0], "SW"))){
		return false;
	}
	//otherwise return true for a successful interpretation
	return true;
}


/*This method prints the array of registers given*/
void print_regs(){
    int col_size = 10;
    for(int i = 0; i < 8; i++){
        printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
        printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
        printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
        printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
    }
}


/*This method checks if two strings are equal, returns true if they are and false otherwise*/
bool str_equals(char str1[], char str2[]){
	
	int i = 0;
	
	//while str1[i] is equal to str2[i] AND both strings have not reached their null symbol \0
	while(str1[i] != '\0' && str2[i] != '\0'){
		
		if(str1[i] != str2[i]){ //if the 2 strings are not equal at i, return false
			return false;
		}
		++i;
	}
	
	return true; //return true, the strings are equal
}




/*This method receives a char array of instructions, and returns a tokenized array*/
 
char** parse(char instruction[]){
	char *start; //set a pointer to the start of the array
	start = instruction;
  
	char* endword; //set a pointer to the end of the first token
	endword = end_word(start);

  
	int numtokens = count_tokens(start); //count num of tokens to determine the size we need to allocate
  
	char** tokens = (char**) malloc(numtokens * sizeof(char)); //declare an array of pointers, where we will store our tokens

  
	tokens = tokenize(tokens, start, endword, 0, numtokens); //call the tokenizer function
	print_all_tokens(tokens, numtokens); //print the tokens 
	
	return tokens; //return the token array of tokens
 }
 
 

int main(){
	
	init_regs(); 
	
	//get first instruction from the user and store it in the instruction array
	char instruction[100];
	printf("Enter instruction: ");
	fgets(instruction, sizeof(instruction), stdin);
	
	//call the parse method to tokenize the instructions
	char** tokens = parse(instruction);
	
	//while the interpret function returns true, keep prompting the user for instructions
	while(interpret(tokens)){
		
		print_regs(); //print the array of registers
		
		interpret(tokens); //interpret the instruction
		
		printf("Enter instruction: "); //prompt for instruction from user
		fgets(instruction, sizeof(instruction), stdin);
		
		tokens = parse(instruction); //tokenize the new instruction
	
	}
		
	

	return 0;
}

