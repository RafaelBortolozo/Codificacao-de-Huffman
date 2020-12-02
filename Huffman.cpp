#include <stdio.h> 
#include <stdlib.h>
#include <vector>
#include <conio.h>
#define QTD_BITS 15

/* Alunos: Lucas Mozzer & Rafael Bortolozo 
   Codificação de Huffman
*/

typedef struct sNodo{ 
    struct sNodo* left;
	struct sNodo* right;
	char c; 
    unsigned int freq;  
}Nodo;

typedef struct sBinary{ 
	char c; 
    char binary[QTD_BITS];  
}Binary;

FILE* readArquive();
FILE* readArquiveBinary();
FILE* writeArquive();
FILE* writeArquiveDecoded();
void frequencyCalc(std::vector<Nodo*> &vet, FILE* text);
Nodo* createNodo(Nodo* left, Nodo* right, char c, unsigned int freq);
void insertionSort(std::vector<Nodo*> &vet);
void print(std::vector<Nodo*> &vet);
Nodo* createHuffmanTree(std::vector<Nodo*> &vet);
void binarySequenceCalc(Nodo* root, std::vector<Binary*> &vetBinary, char* binary, int top);
Binary* createBinary(char c, char* binary, int top);
void convertTextToBinary(std::vector<Binary*> &vetBinary);
void convertBinaryToText(Nodo* root);
void pos_ordem (Nodo* root);
void printVetBinary(std::vector<Binary*> &vetBinary);

main(){
	Nodo* root= NULL;
	std::vector<Nodo*> vet;
	FILE* text= readArquive();
	
	//passo 1: calcular frequencias
		frequencyCalc(vet, text);
		insertionSort(vet);
	
	//passo 2: contruir a arvore binaria
		root= createHuffmanTree(vet);
	
	//passo 3: calcular a sequencia binaria de cada simbolo
		std::vector<Binary*> vetBinary;
		char binary[QTD_BITS];
		int top=0;
		binarySequenceCalc(root, vetBinary, binary, top);
	
	//passo 4: converter o texto em binario
		convertTextToBinary(vetBinary);

	//DECODIFICACAO:
		convertBinaryToText(root);
	
}

FILE* readArquive(){
	FILE *file= fopen("text.txt", "r");
	if(file==NULL){
		printf("Erro ao abrir o arquivo");
		exit(1);
	}
	return file;
}

FILE* readArquiveBinary(){
	FILE *file= fopen("binaryText.txt", "r");
	if(file==NULL){
		printf("Erro ao abrir o arquivo");
		exit(1);
	}
	return file;
}

FILE* writeArquive(){
	FILE *file= fopen("binaryText.txt", "a");
	if(file==NULL){
		printf("Erro ao gerar o arquivo");
		exit(1);
	}
	return file;
}

FILE* writeArquiveDecoded(){
	FILE *file= fopen("decodedText.txt", "a");
	if(file==NULL){
		printf("Erro ao gerar o arquivo");
		exit(1);
	}
	return file;
}
void frequencyCalc(std::vector<Nodo*> &vet, FILE* text){
	Nodo* aux;
	char c= fgetc(text);
	while(c != EOF){
		for(int i=0; i<vet.size() || vet.empty() ;i++){
			if(vet.empty()){
				aux= createNodo(NULL, NULL, c, 1);
				vet.push_back(aux);
				break;
			}else{
				
				if(c == vet[i]->c){
					vet[i]->freq += 1;
					break;
				}
				
				else if(i==vet.size()-1){
					aux= createNodo(NULL, NULL, c, 1);
					vet.push_back(aux);
					break;
				}
			}
		}
		c= fgetc(text);
	}
}

void insertionSort(std::vector<Nodo*> &vet){
	for (int i = 1; i < vet.size(); i++){
		Nodo* tmp = vet[i];
		int j;
		for (j = i; j > 0 && tmp->freq < vet[j-1]->freq; j--){
			vet[j] = vet[j-1];
		}
		vet[j] = tmp;
	}
}

void print(std::vector<Nodo*> &vet){
	for(int i=0; i < vet.size(); i++){
		printf("%c - %d\n", vet[i]->c, vet[i]->freq);
	}
}

void pos_ordem (Nodo* root){
 	if(root != NULL){
	    pos_ordem(root->left);
	    pos_ordem(root->right);
	    if(root->c != NULL){
	    	printf("%c ",root->c);
		}
 	}
}

void printVetBinary(std::vector<Binary*> &vetBinary){
	for(int i=0; i< vetBinary.size(); i++){
		printf("%c - %s\n", vetBinary[i]->c, vetBinary[i]->binary);
	}
}

Nodo* createNodo(Nodo* left, Nodo* right, char c ,unsigned int freq){
	Nodo* nodo= (Nodo*)malloc(sizeof(Nodo));
	nodo->left= left;
	nodo->right= right;
	nodo->c= c;
	nodo->freq= freq;
	return nodo;
}	

Nodo* createHuffmanTree(std::vector<Nodo*> &vet){
	Nodo* root;
	Nodo* aux1;
	Nodo* aux2;
	while(vet.size()>0){
		//retira 2 elementos menores
		//como o vector ja esta ordenado, então retira os 2 primeiros
		aux1= vet.front();
		vet.erase(vet.begin());
		aux2= vet.front();
		vet.erase(vet.begin());
		
		//une os nodos a um nodo pai
		root= createNodo(aux1, aux2, NULL, (aux1->freq+aux2->freq));
		
		//se o vector ficou vazio, entao retorne o root
		if(vet.empty()){
			return root;
		}
		
		//se nao, adiciona o root no vector, respeitando a ordem crescente
		vet.push_back(root);
		insertionSort(vet);	
	}
}

void binarySequenceCalc(Nodo* root, std::vector<Binary*> &vetBinary, char* binary, int top){
	
	if(root->left != NULL){
		binary[top]='0';
		binarySequenceCalc(root->left, vetBinary, binary, top+1);
	}
	
	if(root->right != NULL){
		binary[top]='1';
		binarySequenceCalc(root->right, vetBinary, binary, top+1);
	}
	
	if(root->left==NULL && root->right==NULL){
		vetBinary.push_back(createBinary(root->c, binary, top));
	}
}

Binary* createBinary(char c, char* binary, int top){
	Binary* aux= (Binary*)malloc(sizeof(Binary));
	aux->c= c;
	int i=0;
	
	for(i; i<top; i++){
		aux->binary[i]= binary[i];
	}
	aux->binary[i]= NULL;
	
	return aux;
}

void convertTextToBinary(std::vector<Binary*> &vetBinary){
	FILE* text= readArquive();
	FILE* binaryText= writeArquive();
	
	char c= fgetc(text);
	while(c != EOF){
		for(int i=0; i<vetBinary.size(); i++){
			if(vetBinary[i]->c == c){
				fprintf(binaryText,"%s", vetBinary[i]->binary);
				break;
			}
		}
		c= fgetc(text);
	}
	fclose(text);
	fclose(binaryText);	
}

void convertBinaryToText(Nodo* root){
	FILE* binaryText= readArquiveBinary();
	FILE* decodedText= writeArquiveDecoded();
	Nodo* aux= root;
	
	char c= fgetc(binaryText);
	while(c != EOF){
		if(c == '0'){
			aux= aux->left;
		}else if(c == '1'){
			aux= aux->right;
		}
		
		if(aux->left == NULL && aux->right == NULL){
			fprintf(decodedText,"%c", aux->c);
			aux= root;
		}
		c= fgetc(binaryText);
	}
	fclose(binaryText);
	fclose(decodedText);
}


