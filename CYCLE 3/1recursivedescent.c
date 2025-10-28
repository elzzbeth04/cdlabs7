#include<stdio.h>

char input[10];
int pos=0;

int expression();

int match(char c){
	if(input[pos]==c){
        pos++;
		return 1;
	}
	return 0;
}

int factor(){
	if(match('i')) return 1;
	else if(match('(')){
		if(expression()){
			if(match(')')) return 1;
		}
	}
	return 0;
}

int term_prime(){
	if(match('*')){
		if(factor()){
			if (term_prime()) return 1;
		}
		return 0;
	}
	return 1;
}


int term(){
	if (factor()){
		if (term_prime()) return 1;
	}
	return 0;	
}

int expression_prime(){
	if (match('+')) {
		if (term()){
			if (expression_prime()) return 1;
		}
		return 0;
	}
	return 1;
}

int expression(){
	if (term()){
		if(expression_prime())return 1;
	}
	return 0;
}

void main(){
	printf("Enter input expression:");
	scanf("%s", input);
	
	if(expression() && input[pos]=='\0')
		printf("Accepted\n");
	else
		printf("Rejected\n");
}