#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int m = 487;
int R = 479;
char file_name[] = "smallDictionary.txt";
char table_name[] = "table.txt";
char mod[] = "";
char normal[] = "NORMAL";
char debug[] = "DEBUG"; 

struct Node{
	int key;
	char val[30];
};

struct Hash{
	int count;
	struct Node *node;
};

int h(int key,int i){
	return (key % m+ i*(R-(key % R))) % m;
}

struct Hash createHash(){
	struct Hash hash;
	hash.count = 0;
	hash.node = (struct Node*) malloc(m*sizeof(struct Node));
	int i;
	for(i=0;i<m;i++){
		hash.node[i].key = -1;
	}
	return hash;
}

int generator(char str[30]){
	int i;
	int key = 0;
	for(i = 0; i<strlen(str); i++){
		key += (str[i]-'a')*26*i;
	}
	return key;
}

void insertElement(struct Hash *hash, char str[30]){
	int i;
	int index;
	struct Node node;
	strcpy(node.val,str); 
	
	node.key = generator(str);
	i = -1;
	if(strcmp(mod,debug) == 0){
		printf("\nh1: %d Offset:%d\n",node.key%m,R-(node.key%R));
	}
	
	do{
		index = h(node.key,++i);
		if(strcmp(mod,debug) == 0){
			printf("i:%d---%d\n",i,index);	
		}
	}while((hash->node[index].key != -1) && (i < m));

	if(i != m){
		hash->node[index].key = node.key;
		strcpy(hash->node[index].val,str); 
		hash->count++;
		if(strcmp(mod,debug) == 0){
			printf("'%s' inserted to %d. line key:%d \n",node.val,index, node.key);	
		}
		return;
	}
	printf("Failed to add element!\n");
}

int search(struct Hash hash, char str[30]){
	int i;
	int index;
	struct Node node;
	strcpy(node.val,str); 
	node.key = generator(str);
	
	i = -1;
	if(strcmp(mod,debug) == 0){
		printf("\nh1: %d Offset:%d\n",node.key%m,R-(node.key%R));
	}
	do{
		index = h(node.key,++i);
		if(strcmp(mod,debug) == 0){
			printf("i:%d---index:%d\n",i,index);	
		}
	}while((hash.node[index].key != -1) && strcmp(hash.node[index].val, str) != 0);

	
	if(strcmp(hash.node[index].val, str) == 0){
		if(strcmp(mod,debug) == 0){
			printf("'%s' found in %d. line, key:%d \n",node.val,index, node.key);	
		}
		return i+1;
	}
	return -1;
}

void saveTable(struct Hash table){
	int i;
	FILE *hashFile = fopen("table.txt", "w");
	if(!hashFile){
		return;
	}
	
	for(i = 0; i<m; i++){
		if(table.node[i].key != -1){
			fprintf(hashFile,"%d %d %s\n",i,table.node[i].key,table.node[i].val);
		}else{
			fprintf(hashFile,"%d  -1  unf.\n",i);
		}
	}
	fclose(hashFile);
}
	
void suggest(struct Hash hash, char str[30]){
	if(search(hash,str) != -1){
		if(strcmp(mod,debug) == 0){
			printf("word found in %d step(s).\n",search(hash,str));	
		}
		printf("Contains the word!");
		return;
	}
	char buffer[30];
	int max = strlen(str);
	int letter;
	int i = 0;
	int flag = -1;
	int result;
	strcpy(buffer,str);
	
	while(flag == -1 && i < max){
		letter = 97;
		strcpy(str,buffer);
		while(flag == -1 && letter <= 122){
			str[i] = letter++;
			result = search(hash,str);
			if(result != -1){
				flag  = 0;
			}
		}
		i++;	
	}
	if(flag == 0){
		printf("Suggestion: %s\n",str);
		return;
	}	
	printf("There is no suggestion\n");
}

int addElementFromFile(struct Hash table, char fname[30]){
	FILE *fp = fopen(file_name,"r");
	if(!fp){
		printf("%s file not found!",fname);
		return -1;
	}
	char buffer[30];
	while(!feof(fp) && !ferror(fp)){
		fscanf(fp,"%s",buffer);
		insertElement(&table, buffer);
	}
	printf("Dictionary file read successfully!\n");
	fclose(fp);
	return 0;
}

int readTableFromFile(struct Hash table,char fname[30]){
	FILE *fp = fopen(fname,"r");
	if(!fp){
		printf("%s file not found!",fname);
		return -1;
	}
	int index;
	int key;
	char val[30];
	while(!feof(fp)){
		fscanf(fp,"%d %d %s",&index, &key, val);
		if(key != -1){
			table.node[index].key = key;
			strcpy(table.node[index].val,val);
			table.count++;
		}
	}
	printf("Table file read successfully!\n");
	fclose(fp);
	return 0;
}

void printTable(struct Hash table){
	int i=0;
	printf("  TABLE\n");
	while(i < m){
		for(i = 0; i<m; i++){
			if(table.node[i].key != -1){
				printf("%d %d %s\n",i,table.node[i].key,table.node[i].val);
			}else{
				printf("%d  -1  unf.\n",i);
			}
		}
	}
}

int main(){
	struct Hash table = createHash();
	
	while(strcmp(mod,normal)!=0 && strcmp(mod,debug)!=0){
		printf("Select mode(NORMAL/DEBUG):");
		scanf("%s",mod);
	}
	
	if(strcmp(mod,normal) == 0)
		strcpy(mod,normal);
	else{
		strcpy(mod,debug);
	}
	
	int selector;
	printf("Do you want to create hash table?(1/0)");
	do{
		scanf("%d",&selector);
	}while(selector != 0 && selector != 1);
	
	if(selector == 1){
		printf("\nDictionary file name:");
		scanf("%s",file_name);
		if(addElementFromFile(table,file_name) == -1)
			return 0;
		saveTable(table);
	}else{
		printf("\nHash table file name:");
		scanf("%s",table_name);
		if(readTableFromFile(table,table_name) == -1)
			return 0;
	}
	
	if(strcmp(mod,debug) == 0){
		printTable(table);	
	}
	
	char word[30];
	printf("Enter search word:");
	scanf("%s",word);
	suggest(table,word);
	
	free(table.node);
}
