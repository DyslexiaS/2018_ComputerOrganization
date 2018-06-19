#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

char input_file[50] = ".";
char output_file[50] = ".";
queue<int> hit_ins, miss_ins;
unsigned int ins_count;
int* dir_array;
vector<int>* V_four;
vector<int> V_full;
void direct_mapped(int replace_alg, const int tag, const int index);
void four_way(int replace_alg, const int tag, const int index);
void fully(int replace_alg, const int tag, const int block_num);
void print_result();
void write_result(FILE* fp);

int main(int argc, char* argv[]){
    string option(argv[1]);
    if(option == "-input"){
        strcat(input_file, argv[2]);
        strcat(output_file, argv[4]);
    }
    else if(option == "-output"){
        strcat(input_file, argv[4]);
        strcat(output_file, argv[2]);
    }
	printf("intput file = %s\n", input_file);
	printf("output file = %s\n", output_file);
	FILE* fp = fopen(input_file, "r");
	if( fp == NULL){
		printf(" Open failure.\n");
		exit(EXIT_FAILURE);
	}
	int cache_size, block_size, associativity, replace_alg;
	fscanf(fp, "%d %d %d %d", &cache_size, &block_size, &associativity, &replace_alg);
	printf("%d %d %d %d\n", cache_size,block_size,associativity, replace_alg);
	
	int block_num = (cache_size << 10)/block_size;
	switch (associativity){
		case(0):
            dir_array = new int[block_num];
            memset(dir_array, -1, sizeof(int)*block_num);
			break;
		case(1):
            block_num /= 4;
            V_four = new vector<int>[block_num];
			break;
		case(2):
			break;
	}

	unsigned int ins;
	while(fscanf(fp, "%x", &ins) != EOF){
        unsigned int tag;
        unsigned int index = 0;
        switch (associativity){
            case(0):
                tag = ins/(block_num*block_size);
                index = (ins%(block_num*block_size))/block_size;
                direct_mapped(replace_alg, tag, index);
                break;
            case(1):
                tag = ins/(block_num*block_size);
                index = (ins%(block_num*block_size))/block_size;
                four_way(replace_alg, tag, index);
                break;
            case(2):
                tag = ins/block_size;
                fully(replace_alg, tag, block_num);
        }
   //     printf("inst=%d, tag= %u, index= %u\n", ins_count, tag, index);
    }

    FILE* write = fopen(output_file, "w");
	if( fp == NULL){
		printf(" Output file failure.\n");
		exit(EXIT_FAILURE);
	}
    write_result(write);
    fclose(fp);
    fclose(write);
    return 0;
    
}

void direct_mapped(int replace_alg, const int tag, const int index){
    if(dir_array[index] == tag)
        hit_ins.push(++ins_count);
    else{
        miss_ins.push(++ins_count);
        dir_array[index] = tag;
    }
}

void four_way(int replace_alg, const int tag, const int index){
    if(replace_alg == 0){       //FIFO
        for(vector<int>::iterator i=V_four[index].begin(); i!=V_four[index].end(); ++i){
            if(*i == tag){
                hit_ins.push(++ins_count);
                return;
            }
        }
        miss_ins.push(++ins_count);
        if(V_four[index].size() == 4)
          V_four[index].begin() = V_four[index].erase(V_four[index].begin());
        V_four[index].push_back(tag);
    }
	else if(replace_alg = 1){                //LRU
        for(vector<int>::iterator i=V_four[index].begin(); i!=V_four[index].end(); ++i){
            if(*i == tag){
                hit_ins.push(++ins_count);
                i = V_four[index].erase(i);
                V_four[index].push_back(tag);
                return;
            }
        } 
        miss_ins.push(++ins_count);
        if(V_four[index].size() == 4)
          V_four[index].begin() = V_four[index].erase(V_four[index].begin());
        V_four[index].push_back(tag);
    }
}
void fully(int replace_alg, const int tag, const int block_num){
	if(replace_alg == 0){            //FIFO();
        for(vector<int>::iterator i=V_full.begin(); i!=V_full.end(); ++i){
            if(*i == tag){
                hit_ins.push(++ins_count);
                return;
            }
        }
        miss_ins.push(++ins_count);
        if(V_full.size() == block_num)
          V_full.begin() = V_full.erase(V_full.begin());
        V_full.push_back(tag);

    }
	else if(replace_alg == 1){      //LRU
        for(vector<int>::iterator i=V_full.begin(); i!=V_full.end(); ++i){
            if(*i == tag){
                hit_ins.push(++ins_count);
                i = V_full.erase(i);
                V_full.push_back(tag);
                return;
            }
        } 
        miss_ins.push(++ins_count);
        if(V_full.size() == block_num)
          V_full.begin() = V_full.erase(V_full.begin());
        V_full.push_back(tag); 
    }
}
void write_result(FILE* fp){
    float miss_rate = (float)miss_ins.size()/(miss_ins.size()+hit_ins.size()); 
    fprintf(fp,"Hits instructions: ");
    int first_ins = 0;
    while(!hit_ins.empty()){
        if(first_ins == 0){
            fprintf(fp,"%d", hit_ins.front());
            first_ins++;
        }
        else
            fprintf(fp,",%d", hit_ins.front());
        hit_ins.pop();
    }
    fprintf(fp,"\nMisses instructions: ");
    first_ins = 0;
    while(!miss_ins.empty()){
        if(first_ins == 0){
            fprintf(fp,"%d", miss_ins.front());
            first_ins++;
        }
        else
            fprintf(fp,",%d", miss_ins.front());
        miss_ins.pop();
    }
    fprintf(fp, "\nMiss rate: %f\n", miss_rate);
}
