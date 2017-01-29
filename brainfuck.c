#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>


/** Brain Fuck 
 * basic instructions
 * '>' increment pointer
 * '<' decrement pointer
 * '+' increment byte at pointer
 * '-' decrement byte at pointer
 * '.' output to console byte at pointer
 * ',' input from console at pointer byte
 * ']' jump to last '[' if byte at pointer is not zero
 * otherwise continue with next instruction
 * '[' jump to instruction after ']' if byte at pointer is zero
 * otherwise continue with next instruction
 * Copy Right: Free as in free AIR
 *
 */
 
 /** tokenize
  * token types:
  * INCR_BYTE
  * DECR_BYTE
  * INCR_PTR
  * DECR_PTR
  * PRINT_CON
  * READ_CON
  * ST_JMP_LAB
  * END_JMP_LAB
  */
 
 #define INCR_BYTE 1
 #define DECR_BYTE 2
 #define INCR_PTR  3
 #define DECR_PTR  4
 #define PRINT_CON 5
 #define READ_CON  6
 #define ST_JMP_LAB 7
 #define END_JMP_LAB 8
 #define BF_STAT_END 9
 #define BF_SPACE 10
 
 /** Max array size */
 #define ARRAY_MAX 300
 
 /** token object 
  * struct token:
  * char type
  * int type
  * 
  * 
  */
  
 typedef struct token {
	 int kind;
 } token;
 
 
 /** tokenize function
  * @ params src
  * @ returns list of tokens
  */
  
  token mkToken(int knd){
	  token nm;
	  nm.kind = knd;
	  return nm; 
  }
  
  token *tokenize(char *src){

	  char *wsrc;
	  token *tk_list = malloc(ARRAY_MAX * sizeof(token));
	  int idx = 0;
	  wsrc = src;
	  int c;
	  c = *wsrc++;
	  
	  while(c != '\0' && idx < ARRAY_MAX - 1){
		  switch(c){
			  case '+':
			   tk_list[idx++] = mkToken(INCR_BYTE);
			   c = *wsrc++;
			   break;
			  case '-':
				tk_list[idx++] = mkToken(DECR_BYTE);
				c = *wsrc++;
				break;
			  case '<':
				tk_list[idx++] = mkToken(DECR_PTR);
				c = *wsrc++;
				break;
			  case '>':
				tk_list[idx++] = mkToken(INCR_PTR);
				c = *wsrc++;
				break;
			  case '.':
				tk_list[idx++] = mkToken(PRINT_CON);
				c = *wsrc++;
				break;
			  case ',':
				tk_list[idx++] = mkToken(READ_CON);
				c = *wsrc++;
				break;
			  case '[':
				tk_list[idx++] = mkToken(ST_JMP_LAB);
				c = *wsrc++;
				break;
			  case ']':
				tk_list[idx++] = mkToken(END_JMP_LAB);
				c = *wsrc++;
				break;
			  default:
			    if(c == '\n'){
					tk_list[idx++] = mkToken(BF_STAT_END);
					c = *wsrc++;
					break;
				} else if(isspace(c)){
					tk_list[idx++] = mkToken(BF_SPACE);
					c = *wsrc++;
					break;
				}else{
					
					fprintf(stderr, "Error: Token type not allowed <token: %c>", c);
					exit(1);
				}
				
			}
			
		}
		tk_list[idx] = mkToken(-1);
		/**int i = 0;
		while(tk_list[i].kind != -1){
			printf("%d\n", tk_list[i].kind);
			i++;
		} */
		return tk_list;
}
				
char bfscan(){
	int c;
	scanf("%d", &c);
	return  '0' + c;
	} 			

void interpret(token *tks, char *bytes){
	int idx = 0;
	token tk;
	int bidx = 0;
	tk = tks[idx];
	/**int mstatus = 0; */
	int count = 0;
	//char ch;
	
	
	while(tk.kind != -1 && bidx < ARRAY_MAX){
		
		switch(tk.kind){
			
			case INCR_BYTE:
				++bytes[bidx];
				if(bytes[bidx] > 255){
					bytes[bidx] = 0;
				}
				break;
			case DECR_BYTE:
				--bytes[bidx];
				if(bytes[bidx] < 0){
					bytes[bidx] = 0;
				}
				break;
			case INCR_PTR:
				bidx++;
				break;
			case DECR_PTR:
				bidx--;
				break;
			case PRINT_CON:
				printf("%c\n", toascii(bytes[bidx]));
				printf("\nnot ascii%i\n", bytes[bidx]);
				count++;
				break;
			case READ_CON:
				printf("Enter charcter > ");
				bytes[bidx] = bfscan();
				break;
			case ST_JMP_LAB:
				if(bytes[bidx] == 0){
					idx++;
					tk = tks[idx];
					while(tk.kind != END_JMP_LAB) {
						idx++;
						tk = tks[idx];
					}

				}
				break;
			case END_JMP_LAB:
				if(bytes[bidx] != 0){
					idx--;
					tk = tks[idx];
					while(tk.kind != ST_JMP_LAB){
						idx--;
						tk = tks[idx];
					}

				}
				break;
			}
			idx++;
			tk = tks[idx];
			
			
		}
		return ;
					
}
	  
	
int size_file(FILE *fle){
	int size;
	fseek(fle, 0, SEEK_END);
	size = ftell(fle);
	fseek(fle, 0, SEEK_SET);
	return size;
}
	
char *read_file(FILE *fle, int sze){
	
	char *src = malloc((sze + 1) * sizeof(char));
	int rdn;
	if((rdn = fread(src, sizeof(char), sze, fle)) > 0){
		 src[sze] = '\0';
	} else if (src == NULL){
		 fprintf(stderr, "%s", "Failed to read file\n");
	}
	return src;
	
}

int main(int argc , char *argv[]){
	int size;
	FILE *src_file;
	char *src, bytes[ARRAY_MAX];
	token *list;
	if(argc != 2){
		fprintf(stderr, "Include only brain fuck file");
		exit(1);
	}
	
	src_file = fopen(argv[1], "rb");
	if(src_file == NULL){
		fprintf(stderr, "%s\n", "file could not open");
		exit(1);
	}
	size = size_file(src_file);
	src = read_file(src_file, size);
	fclose(src_file);
	memset(bytes, 0, ARRAY_MAX);
	list = tokenize(src);
	interpret(list, bytes);
	free(src);
	printf("Crafted by Dreams\n");
	
	return 0;
}
	
	
