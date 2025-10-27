#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
char ip_sym[50], op[100], tmp[100]; 
int ip_ptr = 0; 
void e(); 
void e_prime(); 
void t(); 
void t_prime(); 
void f(); 
void advance(); 

void e() { 
strcpy(op, "TE'"); 
printf("E=%-25s E->TE'\n", op); 
t(); 
e_prime(); 
} 



void t() { 
int i,n=0,l; 
for(i=0;i<=strlen(op);i++) 
if(op[i]!='#') tmp[n++]=op[i]; 
tmp[n]='\0'; 
strcpy(op,tmp); 
l = strlen(op); 
for(n=0;n<l && op[n]!='T';n++); 
i = n+1; 
do { op[i+2]=op[i]; i++; } while(i<l); 
op[n++]='F'; op[n++]='T'; op[n++]='\''; 
printf("E=%-25s T->FT'\n", op); 
f(); 
t_prime(); 
} 



void f() { 
int i,n=0,l; 
for(i=0;i<=strlen(op);i++) 
if(op[i]!='#') tmp[n++]=op[i]; 
tmp[n]='\0'; 
strcpy(op,tmp); 
l = strlen(op); 
for(n=0;n<l && op[n]!='F';n++); 
if(ip_sym[ip_ptr]=='i') { 
op[n]='i'; 
printf("E=%-25s F->i\n", op); 
advance(); 
} else if(ip_sym[ip_ptr]=='(') { 
advance(); 
op[n]='('; op[n+1]='E'; op[n+2]=')'; 
printf("E=%-25s F->(E)\n", op); 
e(); 
if(ip_sym[ip_ptr]==')') advance(); 
else { printf("Syntax error: expected )\n"); exit(1); } 
} else { 
printf("\nSyntax error: invalid symbol '%c'\n", ip_sym[ip_ptr]); 
exit(1); 
} 

void t_prime() { 
int i,n=0,l; 
for(i=0;i<=strlen(op);i++) 
if(op[i]!='#') tmp[n++]=op[i]; 
tmp[n]='\0'; 
strcpy(op,tmp); 
l = strlen(op); 
for(n=0;n<l && op[n]!='T';n++); 
if(ip_sym[ip_ptr]=='*') { 
i=n+2; 
do { op[i+2]=op[i]; i++; } while(i<l); 
op[n++]='*'; op[n++]='F'; op[n++]='T'; op[n++]='\''; 
printf("E=%-25s T'->*FT'\n", op); 
advance(); 
f(); 
t_prime(); 
} else { 
for(i=n;i<=strlen(op);i++) op[i]=op[i+1]; 
printf("E=%-25s T'->e\n", op); 
if(ip_sym[ip_ptr]=='*') t_prime(); 
} 
} 

void e_prime() { 
int i, n=0, l; 
for(i=0;i<=strlen(op);i++) 
if(op[i]!='#') tmp[n++]=op[i]; 
tmp[n]='\0'; 
strcpy(op, tmp); 
l = strlen(op); 
for(n=0; n<l && op[n]!='E'; n++); 
if(ip_sym[ip_ptr]=='+') { 
i = n+2; 
do { op[i+2]=op[i]; i++; } while(i<=l); 
op[n++] = '+'; op[n++] = 'T'; op[n++] = 'E'; op[n++] = '\'';  
printf("E=%-25s E'->+TE'\n", op); 
advance(); 
t(); 
e_prime(); 
} else { 
for(i=n;i<=strlen(op);i++) op[i]=op[i+1];  
printf("E=%-25s E'->e\n", op); 
if(ip_sym[ip_ptr]=='+') e_prime(); 
} 
} 

void advance(){ ip_ptr++; } 
int main() { 
printf("\nGrammar without left recursion\n"); 
printf("E -> TE'\nE' -> +TE'|e\nT -> FT'\nT' -> *FT'|e\nF -> (E)|i\n\n"); 
printf("Enter the input expression: "); scanf("%s", ip_sym); 
printf("\nExpressions\t Sequence of production rules\n"); 
e(); 
if(ip_sym[ip_ptr]=='\0') { 
printf("\nParsing successful!\n"); 
} else { 
printf("\nSyntax error at '%c'\n", ip_sym[ip_ptr]); 
} 
return 0;
}