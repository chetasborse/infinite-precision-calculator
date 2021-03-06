typedef struct node{
	struct node *prev;
	int data;
	struct node *next;
}node;
typedef struct Integer{
	node *head;
	int len;
	int dec;
	int parity;
	node *tail;
}Integer;
typedef struct node2{
	Integer val;
	struct node2 *next;
}node2;
typedef node2 *stack;
void initInteger(Integer *a);
void addPrev(Integer *a, char c);
void addDigit(Integer *a, char c);
Integer Createintegerfromstring(char *str);
Integer addIntegers(Integer a, Integer b);
void printInteger(Integer a);
void destroyInteger(Integer *a);
int compare(Integer a, Integer b);
Integer INT_MIN();
Integer subtractIntegers(Integer a, Integer b);
Integer multiplyIntegers(Integer a, Integer b);
Integer divideIntegers(Integer a, Integer b);
Integer modIntegers(Integer a, Integer b);
Integer Rightshift(Integer a, Integer b);
Integer Leftshift(Integer a, Integer b);
Integer Power(Integer a, Integer b);
int Itoi(Integer a);
Integer itoI(int a);
void length(Integer *a);
void match(Integer *a, Integer *b);
Integer ftoI(float);
Integer s(float);
Integer c(float);
Integer t(float);
void init(stack *s);
void push(stack *s, Integer no);
Integer pop(stack *s);
int isempty(stack *s);
int isfull(stack *s);
void History(Integer, int);
