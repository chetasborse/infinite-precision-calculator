#include"decimal.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
void initInteger(Integer *a) {
	a->head = NULL;
	a->tail = NULL;
	a->parity = 0;
	a->dec = 0;
	a->len = 0;
}
void addPrev(Integer *a, char c) {
	int k;
	if(c >= '0' && c <= '9') {
	node *tmp = (node*)malloc(sizeof(node));	
		k = c - '0';
		tmp->data = k;
		tmp->prev = NULL;
		tmp->next = a->head;
		a->head->prev = tmp;
		a->head = tmp;
	}
	length(a);
}
void addDigit(Integer *a, char c) {
	int k;
	if(c == '.') 
		a->dec = a->len;	
	else if(c == '-') 
		a->parity = 1;
	else if(c >= '0' && c <= '9') {
	node *tmp = (node*)malloc(sizeof(node));	
		k = c - '0';
		tmp->data = k;
		if(a->tail == NULL && a->head == NULL) {
			tmp->next = NULL;
			tmp->prev = NULL;
			a->tail = tmp;
			a->head = tmp;
		}
		else {
			tmp->next = NULL;
			tmp->prev = a->tail;
			a->tail->next = tmp;
			a->tail = tmp;
		}
	}
	length(a);
}
void length(Integer *a) {
	node *ptr;
	int length = 0;
	ptr = a->head;
	while(ptr) {
		ptr = ptr->next;
		length++;
	}
	a->len = length;
}	
Integer Createintegerfromstring(char *str) {
	Integer a;
	int n, i;
	initInteger(&a);
	n = strlen(str);
	for(i = 0; i < n; i++)
		addDigit(&a, str[i]);
	length(&a);
	return a;
}
void printInteger(Integer a) {
	int i, m = 0;
	node *tmp = (node*)malloc(sizeof(node));
	tmp = a.head;
	if(a.parity == 1) 
		printf("-");
	while(tmp) {
		i = tmp->data;
		tmp = tmp->next;
		printf("%d", i);
		m++;
		if(m == a.dec)
			printf(".");
	}
	printf("\n");
}
void destroyInteger(Integer *a) {
	node *ptr1, *ptr2;
	ptr1 = a->head;
	if(a->head != NULL && a->tail != NULL) {
		while(ptr1) {
			ptr2 = ptr1;
			free(ptr2);
			ptr1 = ptr1->next;
		}
		initInteger(a);
	}
}
int compare(Integer a, Integer b) {
	node *ptr1, *ptr2;
	int i1 = 0, i2 = 0;
	ptr1 = a.head;
	ptr2 = b.head;
	while(ptr1) {
		if(ptr1->data == 0) {
			a.head = ptr1->next;
			ptr1 = ptr1->next;
		}
		else
			break;
	}
	while(ptr2) {
		if(ptr2->data == 0) {
			b.head = ptr2->next;
			ptr2 = ptr2->next;
		}
		else
			break;
	}
	ptr1 = a.head;
	ptr2 = b.head;
	while(ptr1) {
		i1++;
		ptr1 = ptr1->next;
	}
	while(ptr2) {
		i2++;
		ptr2 = ptr2->next;
	}
	if(i2 > i1) 
		return 2;
	else if(i1 > i2) 
		return 1;
	else if(i1 == i2) {
		ptr1 = a.head;
		ptr2 = b.head;
		while(ptr1) {
			if(ptr1->data < ptr2->data) {
				return 2;
			}
			else if(ptr1->data > ptr2->data) {
				return 1;
			}
			else if(ptr1->data == ptr2->data) {
				ptr1 = ptr1->next;
				ptr2 = ptr2->next;
			}
		}
		if(ptr1 == NULL) 
			return 0;
	}
}
Integer INT_MIN() {
	Integer a;
	initInteger(&a);
	addDigit(&a, '9');
	addDigit(&a, '3');
	addDigit(&a, '7');
	addDigit(&a, '0');
	addDigit(&a, '8');
	addDigit(&a, '3');
	addDigit(&a, '4');
	addDigit(&a, '0');
	addDigit(&a, '8');
	addDigit(&a, '2');
	return a;
}	
Integer addIntegers(Integer a, Integer b) {
	int i1 = 0, i2 = 0, k = 0, d, r;
	Integer e, c, g;
	char f;
	initInteger(&c);
	initInteger(&e);
	initInteger(&g);
	node *ptr1, *ptr2;
	match(&a, &b);
	ptr1 = a.head;
	ptr2 = b.head;
	if(a.parity == 1 && b.parity == 0) {
		a.parity = 0;
		c = subtractIntegers(b, a);
		return c;
	}
	if(a.parity == 0 && b.parity == 1) {
		b.parity = 0;
		c = subtractIntegers(a, b);
		return c;
	}
	while(ptr1){
		i1++;
		ptr1 = ptr1->next;
	}
	while(ptr2){
		i2++;
		ptr2 = ptr2->next;
	}
	if(i2 > i1) {
		c = a;
		a = b;
		b = c;
	}
	ptr1 = a.tail;
	ptr2 = b.tail;
	while(ptr1){	
		while(ptr2) {
			d = ptr1->data + ptr2->data + k;
			r = d % 10;
			k = d / 10;
			f = r + 48;
			addDigit(&e,f); 
			ptr2 = ptr2->prev;
			ptr1 = ptr1->prev;
		}
		
		if(ptr1 != NULL) {
			d = ptr1->data + k;
			r = d % 10;
			k = d / 10;
			f = r + 48;
			addDigit(&e,f);
			ptr1 = ptr1->prev;
		}
		if(ptr1 == NULL) {
			while(k != 0) {
				d = k;
				r = d % 10;
				k = d / 10;
				f = r + 48;
				addDigit(&e,f);
			}
		}
	}
	ptr1 = e.tail;
	while(ptr1) {
		k = ptr1->data;
		f = k + 48;
		addDigit(&g, f);
		ptr1 = ptr1->prev;
	}
	destroyInteger(&c);
	if(a.parity == 1 && b.parity == 1)
		g.parity = 1;
	length(&g);
	g.dec = g.len - (a.len - a.dec);
	if(a.dec == 0 && b.dec == 0)
		g.dec = 0;
	return g;
}
Integer subtractIntegers(Integer a, Integer b) {
	int i1 = 0, i2 = 0, k = 0, d, par = 0, par1 = 0;
	char f;
	Integer c, g, e, z;
	initInteger(&z);
	initInteger(&c);
	initInteger(&g);
	initInteger(&e);
	if(a.parity == 1 && b.parity == 0) {
		b.parity = 1;
		z = addIntegers(a, b);
		length(&z);
		return z;
	}
	if(a.parity == 0 && b.parity == 1) {
		b.parity = 0;
		z = addIntegers(a, b);
		length(&z);
		return z;
	}
	match(&a, &b);	
	node *ptr1, *ptr2;
	ptr1 = a.head;
	ptr2 = b.head;
	while(ptr1){
		i1++;
		ptr1 = ptr1->next;
	}
	while(ptr2){
		i2++;
		ptr2 = ptr2->next;
	}
	ptr1 = a.head;
	ptr2 = b.head;
	if(i1 == i2) {
		while(ptr1) {
			if(ptr1->data < ptr2->data) {
				par++;
				c = a;
				a = b;
				b = c;
				break;
			}
			else if(ptr1->data > ptr2->data) {
				break;
			}
			else if(ptr1->data == ptr2->data) {
				ptr1 = ptr1->next;
				ptr2 = ptr2->next;
			}
		}
	}
	if(i1 < i2) {
		par++;
		c = a;
		a = b;
		b = c;
	}
	ptr1 = a.tail;
	ptr2 = b.tail;
	while(ptr1) {
		while(ptr2) {
			if((ptr1->data + k) < ptr2->data) {
				d = 10 + ptr1->data - ptr2->data + k;
				k = -1;
			}
			else {
				d = ptr1->data - ptr2->data + k;
				k = 0;
			}
			f = d + 48;
			addDigit(&e, f);
			ptr1 = ptr1->prev;
			ptr2 = ptr2->prev;
		}
		if(ptr1 != NULL) {
			d = ptr1->data + k;
			if(d < 0) {
				d = 10 + ptr1->data + k;
				k = -1;
			}
			else 
				k = 0;
			f = d + 48;
			addDigit(&e, f);
			ptr1 = ptr1->prev;
		}
	}
	ptr1 = e.tail;
	while(ptr1) {
		k = ptr1->data;
		f = k + 48;
		addDigit(&g, f);
		ptr1 = ptr1->prev;
	}
	destroyInteger(&c);
	if(par == 1)
		g.parity = 1;
	if(a.parity == 1 && b.parity == 1) {
		if(g.parity == 1)
			g.parity = 0;
		else
			g.parity = 1;
	}
	if(a.dec != 0 || b.dec != 0) {
		length(&g);
		g.dec = g.len - (a.len - a.dec);
		return g;
	}
	ptr1 = g.head;
	while(ptr1) {
		if(ptr1->data == 0 && g.len > 1) {
			g.head = ptr1->next;
			ptr1 = ptr1->next;
			length(&g);
		}
		else
			break;
	}
	g.dec = g.len - (a.len - a.dec);
	if(g.dec < 0)
		g.dec = 0;
	return g;
}
Integer multiplyIntegers(Integer a, Integer b) {
	Integer c, sum, d, c3;
	initInteger(&c);
	initInteger(&c3);
	initInteger(&sum);
	initInteger(&d);
	node *ptr;
	char c1 = '0';
	int k, i, par = 0, z1, z2, k1, k2;
	k1 = a.dec;
	k2 = b.dec;
	z1 = a.len - a.dec;
	z2 = b.len - b.dec;
	a.dec = 0;
	b.dec = 0;
	if(b.parity == 1) 
		par++;	
	if(a.parity == 1) 
		par++;
	c = a;
	d = a;
	ptr = b.head;
	while(ptr) {
		k = ptr->data;
		for(i = 0; i < k - 1; i++) {
			a = addIntegers(a, c);
		}
		ptr = ptr->next;
		addDigit(&sum, c1);
		if(k != 0) 
			sum = addIntegers(sum, a);
		a = d;
	}
	if(par == 1)
		sum.parity = 1;
	else
		sum.parity = 0;
	length(&sum);
	if(k1 == 0)
		z1 = 0;
	if(k2 == 0)
		z2 = 0;
	sum.dec = sum.len - (z1 + z2);
	if(k1 == 0 && k2 == 0)
		sum.dec = 0;
	if(k1 != 0 && k2 != 0 && sum.dec <= 0) {
		sum.dec = 0 - sum.dec;
		for(i = 0; i <= sum.dec; i++) 
			addPrev(&sum, '0');
		sum.dec = sum.len - (z1 + z2);
	}
	return sum;
}
Integer divideIntegers(Integer a, Integer b) {
	Integer c, d, e, f, z;
	initInteger(&f);
	addDigit(&f, '0');
	node *ptr;
	char ch;
	int par = 0, a1, b1, x1 = 0, y1 = 0, i = 0, t1, t2, q = 0;
	t1 = Itoi(b);
	if(t1 == 0) 
		return f;
	a1 = a.dec;
	b1 = b.dec;
	t2 = b.len;
	if(a1 > 0 || b1 > 0) {
		if(a1 > 0) {
			x1 = a.len - a.dec;
			t1 = x1;
			ptr = a.tail;
			while(t1 != 0 && ptr->data == 0) {
				if(b1 == 0) {
					b.dec = t2;
					b1 = t2;
				}
				addDigit(&b, '0');
				ptr = ptr->prev;
				t1--;
			}
		
		}
		if(b1 > 0)
			y1 = b.len - b.dec;
		if(x1 < y1) {
			for(i = 0; i < y1 - x1; i++)
				addDigit(&a, '0');
		}
	}
	a.dec = 0;
	b.dec = 0;
	if(a.parity == 1) {
		par++;
		a.parity = 0;
	}
	if(b.parity == 1) {
		b.parity = 0;
		par++;
	}
	ptr = b.head;
	while(ptr->data == 0) {
		ptr = ptr->next;
		b.head = ptr;
		b.head->prev = NULL;
	}
	initInteger(&c);
	initInteger(&e);
	initInteger(&d);
	initInteger(&z);
	addDigit(&c, '0');
	addDigit(&d, '1');
	e = b;
	if(compare(b, c) == 0) {
		if(par == 1) 
			b.parity = 1;	
		length(&b);
		return b;
	}
	destroyInteger(&c);
	if(compare(a, b) == 2) 
		return c;
	destroyInteger(&c);	
	if(compare(a, b) == 1) {
		ptr = a.head;
		while(ptr) {
			ch = ptr->data + 48;
			addDigit(&z, ch);
			while(compare(b, z) == 1 && ptr->next != NULL) {
				ptr = ptr->next;
				ch = ptr->data + 48;
				addDigit(&z, ch);
				if(compare(b, z) == 2)
					break;
				addDigit(&c , '0');
			}
			while(compare(b, z) == 2 || compare(b, z) == 0) {
				z = subtractIntegers(z, b);
				q++;
			}
			ptr = ptr->next;
			ch = q + 48;
			addDigit(&c, ch);
			q = 0;
		}
	}

	if(compare(a, b) == 0) {
		if(par == 1)
			d.parity = 1;
		length(&d);
		return d;
	}
	if(par == 1)
		c.parity = 1;
	length(&c);
	if(x1 > y1)
		c.dec = c.len - (x1 - y1);
	else
		c.dec = 0;
	return c;
} 		
Integer modIntegers(Integer a, Integer b) {
	if(compare(a, b) == 2) 
		return a;
	Integer c, d, e, f, z;
	initInteger(&f);
	addDigit(&f, '0');
	node *ptr;
	char ch;
	int par = 0, a1, b1, x1 = 0, y1 = 0, i = 0, t1, t2, q = 0;
	t1 = Itoi(b);
	if(t1 == 0) 
		return f;
	a1 = a.dec;
	b1 = b.dec;
	t2 = b.len;
	if(a1 > 0 || b1 > 0) {
		if(a1 > 0) {
			x1 = a.len - a.dec;
			t1 = x1;
			ptr = a.tail;
			while(t1 != 0 && ptr->data == 0) {
				if(b1 == 0) {
					b.dec = t2;
					b1 = t2;
				}
				addDigit(&b, '0');
				ptr = ptr->prev;
				t1--;
			}
		
		}
		if(b1 > 0)
			y1 = b.len - b.dec;
		if(x1 < y1) {
			for(i = 0; i < y1 - x1; i++)
				addDigit(&a, '0');
		}
	}
	a.dec = 0;
	b.dec = 0;
	if(a.parity == 1) {
		par++;
		a.parity = 0;
	}
	if(b.parity == 1) {
		b.parity = 0;
		par++;
	}
	ptr = b.head;
	while(ptr->data == 0) {
		ptr = ptr->next;
		b.head = ptr;
		b.head->prev = NULL;
	}
	initInteger(&c);
	initInteger(&e);
	initInteger(&d);
	initInteger(&z);
	addDigit(&c, '0');
	addDigit(&d, '1');
	e = b;
	if(compare(b, c) == 0) {
		if(par == 1) 
			b.parity = 1;	
		length(&b);
		return b;
	}
	destroyInteger(&c);	
	if(compare(a, b) == 1) {
		ptr = a.head;
		while(ptr) {
			ch = ptr->data + 48;
			addDigit(&z, ch);
			while(compare(b, z) == 1 && ptr->next != NULL) {
				ptr = ptr->next;
				ch = ptr->data + 48;
				addDigit(&z, ch);
				if(compare(b, z) == 2)
					break;
				addDigit(&c , '0');
			}
			while(compare(b, z) == 2 || compare(b, z) == 0) {
				z = subtractIntegers(z, b);
				q++;
			}
			ptr = ptr->next;
			ch = q + 48;
			addDigit(&c, ch);
			q = 0;
		}
	}

	if(compare(a, b) == 0) {
		if(par == 1)
			d.parity = 1;
		length(&d);
		return d;
	}
	if(par == 1)
		c.parity = 1;
	length(&c);
	if(x1 > y1)
		c.dec = c.len - (x1 - y1);
	else
		c.dec = 0;
	return z;
}				
int Itoi(Integer a) {
	node *ptr;
	int d, sum = 0;
	ptr = a.head;
	while(ptr) {
		d = ptr->data;
		sum = sum * 10 + d;
		ptr = ptr->next;
	}
	if(a.parity == 1) {
		sum = sum * (-1);
		return sum;
	}
	return sum;
}
Integer Leftshift(Integer a, Integer b) {
	int b1, i, k2;
	node *ptr;
	ptr = b.head;
	k2 = b.dec;
	if(k2 >= 1) {
		for(i = 0; i < k2 - 1; i++) {
			ptr = ptr->next;
		}
		b.tail = ptr;
		b.tail->next = NULL;
	}
	b1 = Itoi(b);
	Integer c;
	char ch = '2';
	initInteger(&c);
	addDigit(&c, ch);
	for(i = 0; i < b1; i++) 
		a = multiplyIntegers(a, c);
	length(&a);
	return a;
}
Integer Rightshift(Integer a, Integer b) {
	int b1, i, k1, k2 = 0;
	node *ptr;
	ptr = b.head;
	k2 = b.dec;
	if(k2 >= 1) {
		for(i = 0; i < k2 - 1; i++) {
			ptr = ptr->next;
		}
		b.tail = ptr;
		b.tail->next = NULL;
	}
	b1 = Itoi(b);
	Integer c;
	char ch = '2';	
	initInteger(&c);
	addDigit(&c, ch);
	for(i = 0; i < b1; i++) 
		a = divideIntegers(a, c);
	length(&a);
	return a;
}
Integer Power(Integer a, Integer b) {
	int b1, b2, b3, k3, i, h;
	h = a.parity;
	a.parity = 0;
	node *ptr;
	ptr = b.head;
	k3 = b.dec;
	if(k3 >= 1) {
		for(i = 0; i < k3 - 1; i++) {
			ptr = ptr->next;
		}
		b.tail = ptr;
		b.tail->next = NULL;
	}
	b1 = Itoi(b);	
	Integer temp, c, d, f, a1, a2, k1 ,k2;
	char ch = '1', ch1 = '2', ch2 = '0';
	initInteger(&d);
	initInteger(&k2);
	initInteger(&k1);
	initInteger(&a1);
	initInteger(&a2);
	addDigit(&d, ch1);
	initInteger(&f);
	addDigit(&f, ch2);
	initInteger(&c);
	addDigit(&c, ch);
	initInteger(&temp);
	a1 = multiplyIntegers(a,b); 
	b2 = Itoi(a1);	
	if(b2 == 0) {
		length(&c);
		return c;
	}
	temp = Power(a, divideIntegers(b, d));
	a2 = modIntegers(b, d); 	
	b3 = Itoi(a2);	
	if(b3 == 0){ 
		k1 = multiplyIntegers(temp, temp);
		length(&k1);
		if(b.parity == 1) 
			return divideIntegers(c, k1);
		if(h == 1 && b2 % 2 != 0)
			k1.parity = 1;
		return k1;
	}
	else {
		k2 = multiplyIntegers(a, multiplyIntegers(temp, temp));
		length(&k2);
		if(b.parity == 1)
			return divideIntegers(c, k2);
		if(h == 1 && b2 % 2 != 0)
			k2.parity = 1;
		return k2;
	} 
}

void match(Integer *a, Integer *b) {
	int post, i;
	if(a->dec == 0 && b->dec != 0) {
		addDigit(a, '.');
		addDigit(a, '0');
	}
	else if(b->dec == 0 && a->dec != 0) {
		addDigit(b, '.');
		addDigit(b, '0');
	}
	if(a->dec != 0 && b->len != 0) {
		post = (a->len - a->dec) - (b->len - b->dec);
		if(post > 0) {
			for(i = 0; i < post; i++) 
				addDigit(b, '0');
		}
		else if(post < 0) {
			post = 0 - post;
			for(i = 0; i < post; i++)
				addDigit(a, '0');
		}
	}
}
Integer ftoI(float a) {
	char str[25];
	sprintf(str, "%f", a);
	Integer x;
	initInteger(&x);
	x = Createintegerfromstring(str);
	return x;
}
Integer s(float a) {
	float b = 0;
	b = sin(a);
	Integer x;
	initInteger(&x);
	x = ftoI(b);
	return x;
}
Integer c(float a) {
	float b = 0;
	Integer x;
	initInteger(&x);
	b = cos(a);
	x = ftoI(b);
	return x;
}
Integer t(float a) {
	float b = 0;
	Integer x;
	initInteger(&x);
	b = tan(a);
	x = ftoI(b);
	return x;
}	
void init(stack *s) {
	*s = NULL;
}
void push(stack *s, Integer no) {
	node2 *temp = (node2 *)malloc(sizeof(node2));
	temp->val = no;
	temp->next = *s;
	*s = temp;
}
Integer pop(stack *s) {
	Integer t = (*s)->val;
	node2 *temp = *s;
	*s = temp->next;
	free(temp);
	return t;
}
int isempty(stack *s) {
	return *s == NULL;
}
int isfull(stack *s) {
	return 0;
}
void History(Integer a, int fd) {
	node *ptr;
	int n = 0;
	char c[3];
	write(fd, "Answer : ", 9);
	if(a.parity == 1)
		write(fd, "-", 1);
	ptr = a.head;;
	while(ptr != NULL) {
		if(n == a.dec && a.dec != 0)
			write(fd, ".", 1);
		sprintf(c, "%d", ptr->data);
		write(fd, c, 1);
		ptr = ptr->next;
		n++;
	}
}
	
	
