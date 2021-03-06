#include <stdio.h>
#include"decimal.h"
#include<stdlib.h>
#include<math.h>
#define OPERAND	100
#define OPERATOR 200
#define END	300
#define ERR 400
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
static int i = 0;
static int flag = 0;
typedef struct token {
	int type; // OPERAND, OPERATOR or END
	union data{
           Integer num; // will be used if type==OPERAND
	   char op; // will e used if type == OPERATOR
       }data;
}token;
enum state { NUMBER, OP, FINISH, ERROR, SPC, TRIGO};
char *states[6] = { "NUMBER", "OP", "FINISH", "ERROR", "SPC", "TRIGO" };
	char currchar;
	static enum state currstate = SPC;
token gettoken(char *expr) {
	Integer no;
	initInteger(&no);
	enum state nextstate;
	char trig;
	float f = 0;
	int j = 0, j1 = 0, no1;
	token t1;
	initInteger(&t1.data.num);
	while(1) {
		currchar = expr[i];
		//printf("currstate: %s currchar: %c\n", states[currstate], currchar);
		switch(currstate) {
			case NUMBER:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':case '.':
						nextstate = NUMBER;
						//no = no * 10 + (currchar - '0');
						addDigit(&no, currchar); 
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(': case ')': case '>': case '<': case '^':
						/*if(currchar != ')' || currchar != '(');
							flag++;
						if(flag > 1) {
							t1.type = ERR;
							return t1;
							break;
						}*/
						nextstate = OP;
						t1.type = OPERAND;
						t1.data.num = no;
						currstate = nextstate;
						i++;
						return t1;
						break;
					case '\0':
						nextstate = FINISH;
						t1.type = OPERAND;
						t1.data.num = no;
						currstate = nextstate;
						return t1;
						break;
					case ' ':
						nextstate = SPC;
						t1.type = OPERAND;
						t1.data.num = no;
						currstate = nextstate;
						i++;
						return t1;
						break;
					default: // anything else
						nextstate = ERROR;
						t1.type = OPERAND;
						t1.data.num = no;
						currstate = nextstate;
						return t1;
						break;                                                                                     
				}
				break;
			case OP:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':case '.':
						//no = currchar - '0';
						nextstate = NUMBER;
						//addDigit(&no, currchar);
						t1.type = OPERATOR;
						t1.data.op = expr[i - 1];
						currstate = nextstate;
						//i++;
						return t1;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(': case ')': case '>': case '<': case '^':
						if(currchar != '(' || currchar != ')') {
							nextstate = ERROR;
							t1.type = ERR;
							return t1;
						}
							nextstate = OP;
						t1.type = OPERATOR;
						t1.data.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t1;
						break;
					case '\0':
						nextstate = FINISH;
						t1.type = OPERATOR;
						t1.data.op = expr[i - 1];
						currstate = nextstate;
						return t1;
						break;
					case ' ':
						nextstate = SPC;
						t1.type = OPERATOR;
						t1.data.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t1;
						break;
					case 's': case 'c' : case 't':
						nextstate = TRIGO;
						t1.type = OPERATOR;
						t1.data.op = expr[i - 1];
						currstate = nextstate;
						i++;
						trig = currchar;
						return t1;
						break;
					default: // anything else
						nextstate = ERROR;
						t1.type = OPERATOR;
						t1.data.op = expr[i - 1];
						currstate = nextstate;
						return t1;
					 	break;
				}
				break;
			case FINISH:
				t1.type = END;
				return t1;
				break;
			case ERROR:
				t1.type = ERR;
				return t1;
				break;
			case SPC:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':case '.':
						//no = currchar - '0';
						addDigit(&no, currchar);
						nextstate = NUMBER;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(': case ')': case '>': case '<': case '^':
						/*if(currchar != ')' || currchar != '(');
							flag++;
						if(flag > 1) {
							t1.type = ERR;
							return t1;
							break;
						}*/	
						nextstate = OP;
						i++;
						break;
					case '\0':
						nextstate = FINISH;
						break;
					case ' ':
						nextstate = SPC;
						i++;
						break;
					case 's': case 'c': case 't':
						nextstate = TRIGO;
						i++;
						trig = currchar;
						break;
					default: // anything else
						nextstate = ERROR;
						break;
				}
				currstate = nextstate;
				break;
			case TRIGO:
				switch(currchar) {
					case '.' :
						j = 1;
						nextstate = TRIGO;
						i++;
						break;	
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						no1 = currchar - '0';
						if(j == 1)
							j1++;
						f = f * 10 + no1;
						nextstate = TRIGO;
						i++;
						break;
					case '(':
						nextstate = TRIGO;
						i++;
						break;
					case ')':
						nextstate = SPC;
						f = f / pow(10, j1);
						j1 = 0;
						j = 0;
						if(trig == 's') 
							no = s(f);
						if(trig == 'c')
							no = c(f);
						if(trig == 't')
							no = t(f);
						t1.type = OPERAND;
						t1.data.num = no;
						i++;
						currstate = nextstate;
						return t1;
						break;
				}		
				currstate = nextstate;
				break;
		}
	}
}
Integer infixeval(char *line) {
	token t;
	Integer new, sec, fir, ans;
	initInteger(&new);
	initInteger(&sec);
	initInteger(&fir);
	initInteger(&ans);
	int x, y, z, b = 0, f = 0, i1, lon, pp = 0, pp1, tt, ch, et, as, count = 0, count1 = 0, c1, c2, kk, i2, i3;
	char k[100], k1[20], k2[20];
	stack s, s1, s2, s3;
	init(&s);
	init(&s1);
	init(&s2);
	init(&s3);	
	while(1) {
		t = gettoken(line);
		if(t.type == OPERAND){
                        if(!isfull(&s)) { 
			     push(&s, t.data.num);
			     b++;
			}
                        else
                             return INT_MIN();
		}else if(t.type == OPERATOR)  {
				k[f] = t.data.op;
				f++;
				
		} else if(t.type == END){
			for(i3 = 0; i3 < f; i3++) {
				if(k[i3] == '(')
					count++;
				if(k[i3] == ')')
					count1++;
			}
			if(count1 != count) 
				return INT_MIN();
			count1 = 0;
			count = 0;
			if(!isempty(&s)) {
				for(x = 0; x < f; x++) {
					if((k[x] == '^') || (k[x] == '<') || (k[x] == '>')){
						for(y = 0; y < b - x - 2; y++) {
							new = pop(&s);
							push(&s1, new);
						}	
						sec = pop(&s);
						fir = pop(&s);
						if(k[x] == '^')	
							ans = Power(fir, sec);
						else if(k[x] == '>')
							ans = Rightshift(fir, sec);
						else
							ans = Leftshift(fir, sec);
						push(&s,ans);
						for(y = 0; y < b - x - 2; y++) {
							new = pop(&s1);
							push(&s, new);
						}
						b--;
						i1 = x;						
						while(i1 < f -1) {
							k[i1] = k[i1 + 1];
							i1++;
						}
						f--;
						x--;
					}
				} 
				for(x = 0; x < f; x++) {
					if(k[x] == '(') 
						count++;
				}
				for(y = 0; y < count; y++) {
					for(x = 0; x < f; x++) {
						if(k[x] == ')') {
							lon = x - 1;
							while(k[lon] != '(') {
								k1[pp] = k[lon];
								lon--;
								pp++;
							}
							for(tt = 0; tt < pp; tt++) {
								k2[tt] = k1[pp - tt - 1];						
							}
							ch = x - pp;
							for(et = ch; et < f; et++) {
								if(k[x] == '(') {
									count1++;
								}
							}				
							for(as = 0; as <= b - x + count - count1 + pp - 1; as++) {
								new = pop(&s);
								push(&s1,new);
							}							
							for(as = 0; as < pp + 1; as++) {
								new = pop(&s1);
								push(&s2,new);
							}
							kk = pp + 1;
							pp1 = pp;
							for(c1 = 0; c1 < pp1; c1++) {
								if((k2[c1] == '*') || (k2[c1] == '/') || (k2[c1] == '%')){
									for(c2 = 0; c2 < kk - c1 - 2; c2++) {
										new = pop(&s2);
										push(&s3, new);
									}	
									sec = pop(&s2);
									fir = pop(&s2);
									if(k2[c1] == '*')	
										ans = multiplyIntegers(fir, sec);
									else if(k2[c1] == '%')
										ans = modIntegers(fir, sec);
									else
										ans = divideIntegers(fir, sec);
									push(&s2,ans);
									for(c2 = 0; c2 < kk - c1 - 2; c2++) {
										new = pop(&s3);
										push(&s2, new);
									}
									
									kk--;
									i1 = c1;						
									while(i1 < pp1 - 1) {
										k2[i1] = k2[i1 + 1];
										i1++;
									}
									pp1--;
									c1--;
								}
							}
							for(c1 = 0; c1 < pp1; c1++) {
								if((k2[c1] == '+') || (k2[c1] == '-')){
									for(c2 = 0; c2 < kk - c1 - 2; c2++) {
										new = pop(&s2);
										push(&s3, new);
									}	
									sec = pop(&s2);
									fir = pop(&s2);
									if(k2[c1] == '+')	
										ans = addIntegers(fir, sec);
									else
										ans = subtractIntegers(fir, sec);
									push(&s2,ans);
									for(c2 = 0; c2 < kk - c1 - 2; c2++) {
										new = pop(&s3);
										push(&s2, new);
									}
									kk--;
									i1 = c1;						
									while(i1 < pp1 - 1) {
										k2[i1] = k2[i1 + 1];
										i1++;
									}
									pp1--;
									c1--;
								}
							}				
							new = pop(&s2);
							push(&s1, new);
							while(!isempty(&s1)){
								new = pop(&s1);
								push(&s,new);			
							}
							i2 = x - 1 - pp;
							while(i2 < f - 2 - pp) {
								k[i2] = k[i2 + pp + 2];
								i2++;
							}
							b = b - pp;
							x = x - pp - 2;
							f = f - pp - 2;							
							count--;
							y--;
						}
						count1 = 0;
						pp = 0;
					}
					
				}
				for(x = 0; x < f; x++) {
					if((k[x] == '*') || (k[x] == '/') || (k[x] == '%')){
						for(y = 0; y < b - x - 2; y++) {
							new = pop(&s);
							push(&s1, new);
						}	
						sec = pop(&s);
						fir = pop(&s);
						if(k[x] == '*')	
							ans = multiplyIntegers(fir, sec);
						else if(k[x] == '%')
							ans = modIntegers(fir, sec);
						else
							ans = divideIntegers(fir, sec);
						push(&s,ans);
						for(y = 0; y < b - x - 2; y++) {
							new = pop(&s1);
							push(&s, new);
						}
						b--;
						i1 = x;						
						while(i1 < f -1) {
							k[i1] = k[i1 + 1];
							i1++;
						}
						f--;
						x--;
					}
				}
				for(x = 0; x < f; x++) {	
					if((k[x] == '+') || (k[x] == '-')) {
						for(y = 0; y < b - x - 2; y++) {
							new = pop(&s);
							push(&s1, new);
						}	
						sec = pop(&s);
						fir = pop(&s);
						if(k[x] == '+')	
							ans = addIntegers(fir, sec);
						else
							ans = subtractIntegers(fir, sec);
						push(&s,ans);
						for(y = 0; y < b - x - 2; y++) {
							new = pop(&s1);
							push(&s, new);
						}
						b--;
						i1 = x;
						while(i1 < f -1) {
							k[i1] = k[i1 + 1];
							i1++;
						}
						f--;
						x--;
					}
							
				}			 
				i = 0;
				currstate = SPC;
     
				return pop(&s);
			}			
			else{ 
				i = 0;
				flag = 0;
				currstate = SPC;                              
				return INT_MIN();
			}
		}
		else{ 
			i = 0;
			flag = 0;
			currstate = SPC;                              
			return INT_MIN();
		}			
	}
	
}
	

int readline(char *line, int len, int fd) {
	int j;
	char ch;
	j = 0;
	if(line[1] != '\n')
		write(fd, "Expression : ", 13);
	while(j < len - 1) {
		ch = getchar();
		write(fd, &ch, 1);
		if(ch == '\n') {
			line[j++] = '\0';
			return j - 1;
		}
		else
			line[j++] = ch;
	}
	line[len - 1] = '\0';
	return len - 1;
}
int main(int argv, char *argc[]) {
	int j, fd;
	if(argv < 2) {
                errno = EINVAL;
                perror("Enter filename for storing history\n");
                return errno;
        }
        fd = open(argc[1], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if(fd == -1) {
        	perror("Unable to open the file");
        	return errno;
        }
        
        printf("This is a basic calculator with infinte precision.\n\nYou can perform the following operations : +, -, *, /, %%, ^, >>, <<.\nPrecautions :\n\n1) For Right shift, Enter '>'.\n2) For Left Shift, Enter '<'.\n3) The power to which the number is raised should be strictly an integer.\n4) For trigonometric functions, Enter s() for sine, c() for cosine, t() for tangent.\n\n"); 
	Integer r;
	initInteger(&r);
	char line[1000], ch;
	write(fd, "\n", 1);
	write(fd, "\n", 1);
	printf("Expression : ");
	j = readline(line, 1000, fd);
	lseek(fd, SEEK_END, 1);
	while(j) {
		printf("Answer : ");
		r = infixeval(line);
		History(r, fd);
		write(fd, "\n", 1);
		write(fd, "\n", 1);
		if(compare(r, INT_MIN()) != 0){
			printInteger(r);
			printf("\n");
		}
		else {
			printf("error in expr\n");
		}
		printf("Expression : ");
		j = readline(line, 1000, fd);

	}
	printf("Do you wish to display History?(Y/N) : ");
	scanf("%c", &ch);
	if(ch == 'y' || ch == 'Y') {
		lseek(fd, SEEK_SET, 0);
		while(read(fd, &ch, 1))
			printf("%c", ch);
	}
			
	return 0;
}
