#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "riscv.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int U_TYPE = 3;
const int UNKNOWN_TYPE = 4;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
static int get_op_type(char *op)
{
    const char *r_type_op[] = {"add", "sub", "and", "or", "xor", "nor", "slt", "sll", "sra"};
    const char *i_type_op[] = {"addi", "andi", "ori", "xori", "slti"};
    const char *mem_type_op[] = {"lw", "lb", "sw", "sb"};
    const char *u_type_op[] = {"lui"};
    for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(r_type_op[i], op) == 0)
        {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(i_type_op[i], op) == 0)
        {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(mem_type_op[i], op) == 0)
        {
            return MEM_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(u_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(u_type_op[i], op) == 0)
        {
            return U_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

hashtable_t* hashtable;
registers_t *registers;

// TODO: create any additional variables to store the state of the interprete

int getRegister(int pos ) {
	return registers -> r[pos];
}

//---------------R-TYPE---------------ADD, SUB, AND, OR, XOR, NOR SLT, SLL, SRA

void add(int rd, int rs1, int rs2) {
	registers -> r[rd] = registers -> r[rs1] + registers -> r[rs2];
}

void sub(int rd, int rs1, int rs2) {
	registers -> r[rd] = registers -> r[rs1] - registers -> r[rs2];
}

void and_func(int rd, int rs1, int rs2) {
	registers -> r[rd] = registers -> r[rs1] & registers -> r[rs2];
}

void or_func(int rd, int rs1, int rs2) {
	registers -> r[rd] = registers -> r[rs1] | registers -> r[rs2];
}

void xor_func(int rd, int rs1, int rs2) {
	registers -> r[rd] = registers -> r[rs1] ^ registers -> r[rs2];
}

void nor_func(int rd, int rs1, int rs2) {
	registers -> r[rd] = !(registers -> r[rs1] | registers -> r[rs2]);
}

void slt (int rd, int rs1, int rs2) {
	registers -> r[rd] = registers -> r[rs1] < registers -> r[rs2] ? 0x1 : 0x0;
}

void sll(int rd, int rs1, int rs2) {
	registers -> r[rd] = registers -> r[rs1] << registers -> r[rs2];
}

void sra(int rd, int rs1, int rs2) {
	registers -> r[rd] = registers -> r[rs1] >> registers -> r[rs2];
}

//---------------I-TYPE---------------ADDI, ANDI, ORI, XORI, SLTI

void addi (int rd, int rs1, int imm) { 
	
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	}

	//To have a sign-extended 12 bits imm value
	
	registers -> r[rd] = registers -> r[rs1] +  actual_imm;	
}

void andi (int rd, int rs1, int imm) {
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	}//To have a sign-extended 12 bits imm value
	registers -> r[rd] = registers -> r[rs1] & actual_imm;
}

void ori (int rd, int rs1, int imm) {
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	} //To have a sign-extended 12 bits imm value
	registers -> r[rd] = registers -> r[rs1] | actual_imm;
}

void xori (int rd, int rs1, int imm) {
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	} //To have a sign-extended 12 bits imm value
	registers -> r[rd] = registers -> r[rs1] ^ actual_imm;
}


void slti (int rd, int rs1, int imm) {
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	}//To have a sign-extended 12 bits imm value
	registers -> r[rd] = registers -> r[rs1] < actual_imm ? 0x1 : 0x0;
}

//---------------MEM-TYPE---------------LW, LB, SW, SB

int getByte (int value, int pos) {
	
	int result = 0;
	
	if (pos == 0) {
		
		result = (int) value & 0xFF;
		return result;
		
	} else if (pos == 1) {
		
		result = (int) (value >> 8) & 0xFF;
		return result; 
		
	} else if (pos == 2) {
		
		result = (int) (value >> 16) & 0xFF;
		return result;
		
	} else {
		result = (int) (value >> 24) & 0xFF;
		return result;
	}
}

//In order to use this, we need to initialize a memory simulation first



//Each bucket will store an int 
//in order to access to one byte of an int for loading or storing, we can use bit mask

void lw(int rd, int rs1, int imm) {
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	}
	int key = (registers -> r[rs1] + actual_imm) >> 2; 
	registers -> r[rd] = ht_get(hashtable, key);
}

void lb(int rd, int rs1, int imm) {
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	}
	int byte_pos = (registers -> r[rs1] + actual_imm) & 0x3;
	int key = (registers -> r[rs1] + actual_imm) >> 2;
	int value = ht_get(hashtable, key);
	
	value = getByte(value, byte_pos);
	
	value = value << 24 >> 24;
	registers -> r[rd] = value;
}

void sw(int rd, int rs1, int imm) {
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	}
	int key = (registers -> r[rs1] + actual_imm) >> 2; 
	ht_add(hashtable, key, registers->r[rd]);
}

void sb(int rd, int rs1, int imm) {
	int actual_imm = imm & 0xFFF;

	if ((actual_imm & 0x800) == 0x800) {
		actual_imm = -((actual_imm^0xFFF) + 1);
	}
	
	int byte_pos = (registers -> r[rs1] + actual_imm) & 0x3;
	int key = (registers -> r[rs1] + actual_imm) >> 2;
	int value = 0;
	
	if (ht_get(hashtable, key) != 0) {
		
		int bitmask = 0;
		if (byte_pos == 0) {
			bitmask = 0xFFFFFF00;
		} else if (byte_pos == 1) {
			bitmask = 0xFFFF00FF;
		} else if (byte_pos == 2) {
			bitmask = 0xFF00FFFF;
		} else {
			bitmask = 0x00FFFFFF;
		}
		
		
		value = ht_get(hashtable, key) & bitmask;
		value = value + (getByte(registers->r[rd], 0)<< byte_pos*8);
		ht_add(hashtable, key, value);
		
	} else {
		value = getByte(registers->r[rd], 0) << byte_pos*8;
		ht_add(hashtable, key, value);
	}
	
}

//---------------U-TYPE---------------LUI

void lui (int rd, int imm) {
	int value = imm & 0x000FFFFF; 
	value = value << 12;
	registers -> r[rd] = value;
}

//Function of left trim a string s
char* l_trim (char* s) {
	
    while (isspace(*s)) {
    	s++;
	}
       
    return s;
}	

//char *strsep(char **stringp, const char *delim) {
//    char *rv = *stringp;
//    if (rv) {
//        *stringp += strcspn(*stringp, delim);
//        if (**stringp)
//            *(*stringp)++ = '\0';
//        else
//            *stringp = 0; }
//    return rv;
//}


void init(registers_t *starting_registers)
{
    registers = starting_registers;
    // TODO: initialize any additional variables needed for state
    hashtable = ht_init(32);
}

// TODO: create any necessary helper functions

void step(char *instruction)
{
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    
    char *op = strsep(&instruction, " ");
    
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE)
    {
        return;
    }
    
    if (op_type == R_TYPE) {
    	
    	
    	int tokens[3];
    	int index = 0;
    	char *token;
    	token = strsep(&instruction, " ,x");
    	
    	while ( token != NULL) {
    		if ((token != NULL) && (token[0] != '\0')) {
    			tokens[index] = (int) strtol(token, NULL, 0);
    			index++;
			}
			
			token = strsep(&instruction, " ,x");
		}
		
    	int rd_value = tokens[0];
    	int rs1_value = tokens[1];
    	int rs2_value = tokens[2];
    	
    	if (strcmp("add", op) == 0) {
    		add( rd_value, rs1_value, rs2_value);
		} else if (strcmp("sub", op) == 0) {
			sub( rd_value, rs1_value, rs2_value);
		} else if (strcmp("and", op) == 0) {
			and_func( rd_value, rs1_value, rs2_value);
		} else if (strcmp("or", op) == 0) {
			or_func( rd_value, rs1_value, rs2_value);
		} else if (strcmp("xor", op) == 0) {
			xor_func( rd_value, rs1_value, rs2_value);
		} else if (strcmp("nor", op) == 0) {
			nor_func( rd_value, rs1_value, rs2_value);
		} else if (strcmp("sll", op) == 0) {
			sll( rd_value, rs1_value, rs2_value);
		} else if (strcmp("slt",op) == 0) {
			slt( rd_value, rs1_value, rs2_value);
		} else if (strcmp("sra", op) == 0) {
			sra( rd_value, rs1_value, rs2_value);
		}
		
    	if (registers->r[0] != 0) {
    		registers -> r[0] = 0;
		}		
    	
	} 

	else if (op_type == I_TYPE){
		
    	int tokens[3];	
    	int index = 0;
    	char *token;
    	token = strsep(&instruction, " ,x");
    	
    	while ( token != NULL) {
    		if ((token != NULL) && (token[0] != '\0')) {
    			tokens[index] = (int) strtol(token, NULL, 0);
    			index++;
			}
			if (index == 2) {
				token = strsep(&instruction, " ,");
			} else {
				token = strsep(&instruction, " ,x");
			}	
			
		}

		
    	int rd_value = tokens[0];
    	int rs1_value = tokens[1];
    	int imm_value = tokens[2];
		    	
    	if (strcmp("addi", op) == 0) {
    		addi(rd_value, rs1_value, imm_value);
		} else if (strcmp("andi", op) == 0) {
    		andi(rd_value, rs1_value, imm_value);
		} else if (strcmp("ori", op) == 0) {
    		ori(rd_value, rs1_value, imm_value);
		} else if (strcmp("xori", op) == 0) {
    		xori(rd_value, rs1_value, imm_value);
		} else if (strcmp("slti", op) == 0) {
    		slti(rd_value, rs1_value, imm_value);
		}
    	
    	if (registers->r[0] != 0) {
    	registers -> r[0] = 0;
		}
	}
	
	else if (op_type == MEM_TYPE) {
		
    	int tokens[3];
    	int index = 0;
    	char *token;
    	token = strsep(&instruction, " ,x()");
    	
    	while ( token != NULL) {
    		if ((token != NULL) && (token[0] != '\0')) {
    			tokens[index] = (int) strtol(token, NULL, 0);
    			index++;
			}
			
			if (index == 1) {
				token = strsep(&instruction, " ,()");
					
			} else {
				token = strsep(&instruction, " ,x()");
			}
			
			
			
		}
		
    	int rd_value = tokens[0];
    	int imm_value = tokens[1];
    	int rs1_value = tokens[2];
		    	
    	if (strcmp("lw", op) == 0) {
    		lw(rd_value, rs1_value, imm_value);
		} else if (strcmp("lb", op) == 0) {
			lb(rd_value, rs1_value, imm_value);
		} else if (strcmp("sw", op) == 0) {
			sw(rd_value, rs1_value, imm_value);
		} else if (strcmp("sb", op) == 0) {
			sb(rd_value, rs1_value, imm_value);
		} 
		
    	if (registers->r[0] != 0) {
    	registers -> r[0] = 0;
		}
	}
    
    else if (op_type == U_TYPE) {
    	
    	int tokens[2];
    	int index = 0;
    	char *token;
    	token = strsep(&instruction, " ,x");
    	
    	while ( token != NULL) {
    		if ((token != NULL) && (token[0] != '\0')) {
    			tokens[index] = (int) strtol(token, NULL, 0);
    			index++;
			}
			if (index == 1) {
				token = strsep(&instruction, " ,");
			} else {
				token = strsep(&instruction, " ,x");
			}
			
		}
		
    	int rd_value = tokens[0];
    	int imm_value = tokens[1];
    
    	
    	lui(rd_value, imm_value);
    	
    	if (registers->r[0] != 0) {
    	registers -> r[0] = 0;
		}
    	
	}

    // TODO: write logic for evaluating instruction on current interpreter state
}
