#include <stdio.h>
#include <string.h>
struct Prod {
	char lhs;
	char rhs[10];
}productions[10];

char nonterminals[10], terminals[10], firsts[10][10], firstcounts[10], follows[10][10], followcounts[10];
int ntcount,count, numrules, visited[10];

int isterminal(char c){
	for(int a = 0; a < count;a++){
		if(terminals[a] == c) return 1;
	}
	return 0;
}

int indexof(char nt){
	for(int i=0;i<ntcount;i++)
		if(nonterminals[i]==nt) return i;
	return -1;
}

int alreadyin(char list[], char val, int length){
	for(int i=0;i<length;i++)
		if(list[i]==val) return 1;
	return 0;
}

void find_first(char nt, int idx){
	visited[idx] = 1;
	for(int i=0;i<numrules;i++){
		if(productions[i].lhs == nt){
			if(isterminal(productions[i].rhs[0]) || productions[i].rhs[0]=='e'){
				if(!alreadyin(firsts[idx], productions[i].rhs[0], firstcounts[idx]))
					firsts[idx][firstcounts[idx]++] = productions[i].rhs[0];
			}
			else{
				int j=0;
				int canbeepsilon = 1;
				while(productions[i].rhs[j]!='\0'){
					char sym = productions[i].rhs[j];
					int index = indexof(sym);
					if(!visited[index]) find_first(sym, index);

					for(int k=0;k<firstcounts[index];k++){
						if(!alreadyin(firsts[idx], firsts[index][k], firstcounts[idx]) && firsts[index][k]!='e') 
							firsts[idx][firstcounts[idx]++] = firsts[index][k];
					}

					if(!alreadyin(firsts[index], 'e', firstcounts[index])){
						canbeepsilon = 0;
						break;
					} 

					j++;
				}
				if(canbeepsilon && !alreadyin(firsts[idx], 'e', firstcounts[idx])) firsts[idx][firstcounts[idx]++] = 'e';
			}
		}	
	}
}

void find_follow(char nt,int idx){
	visited[idx] = 1;
	for(int i  = 0;i < numrules; i++){
		char rhs[10];
		strcpy(rhs,productions[i].rhs);
		int k = 0;
		while(rhs[k] != '\0' && rhs[k] != nt) k++;
		if (rhs[k] == nt){
			if (rhs[k+1] != '\0'){
				if (isterminal(rhs[k+1]) && !alreadyin(follows[idx],rhs[k+1],followcounts[idx])) follows[idx][followcounts[idx]++] = rhs[k+1];
				else{
					int hasepsilon = 0;
					do{
						k++;
						int index = indexof(rhs[k]);
						for(int f = 0; f < firstcounts[index]; f++){
							if(firsts[index][f] != 'e' && !alreadyin(follows[idx],firsts[index][f], followcounts[idx]) ) follows[idx][followcounts[idx]++] = firsts[index][f];
							else hasepsilon = 1;
						}
					}while(hasepsilon && rhs[k+1] != '\0');
					
					if (hasepsilon) {
						int index = indexof(productions[i].lhs);
						if (!visited[index]){
							find_follow(productions[i].lhs,index);
						}
						for(int j = 0; j < followcounts[index];j++){
							if(!alreadyin(follows[idx], follows[index][j], followcounts[idx])) follows[idx][followcounts[idx]++] = follows[index][j];
						}
					}
				}
			}	
			int index = indexof(productions[i].lhs);
			if (!visited[index]) find_follow(productions[i].lhs,index);
			for(int j = 0; j < followcounts[index];j++){
				if(!alreadyin(follows[idx], follows[index][j], followcounts[idx])) follows[idx][followcounts[idx]++] = follows[index][j];
			}
		}
	}
}
int main(){
	printf("Enter the number of terminal symbols: ");
	scanf("%d", &count);
	printf("Enter the numebr of non terminal symbols: ");
	scanf("%d", &ntcount);

	printf("Enter the terminal symbols:  ");
	scanf("%s", terminals);

	printf("Enter the nonterminal symbols:  ");
	scanf("%s", nonterminals);

	printf("Enter the number of productions:  ");
	scanf("%d", &numrules);

	FILE *fp = fopen("grammar.txt","r");
	char leftside, rightside[10];
	for(int i=0;i<numrules;i++){
		fscanf(fp," %c --> %s", &productions[i].lhs, productions[i].rhs);
	}	

	for(int i=0;i<ntcount;i++){
		firstcounts[i]=0;
		visited[i]=0;
	}


	for(int j = 0; j < ntcount; j++){
		if(!visited[j]) find_first(nonterminals[j], j);
	}
	for(int i=0;i<ntcount;i++){
		followcounts[i]=0;
		visited[i]=0;
	}
	printf("Enter start state: ");
	char start;
	scanf(" %c", &start);
	int startindex = indexof(start);
	follows[startindex][followcounts[startindex]++]='$';

	for(int j = 0; j < ntcount; j++){
		if(!visited[j]) find_follow(nonterminals[j], j);
	}

	printf("Firsts:\n");
	for(int j=0;j<ntcount;j++){
		printf("%c {", nonterminals[j]);
		for(int i=0;i<firstcounts[j];i++)
			printf("%c", firsts[j][i]);
		printf("}\n");
	}

	printf("Follows:\n");
	for(int j=0;j<ntcount;j++){
		printf("%c {", nonterminals[j]);
		for(int i=0;i<followcounts[j];i++)
			printf("%c", follows[j][i]);
		printf("}\n");
	}
}