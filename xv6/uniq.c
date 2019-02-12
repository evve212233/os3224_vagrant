#include "types.h"
#include "stat.h"
#include "user.h"

char prev[1024], cur[1024];
int cFlag, dFlag, iFlag;

//for i flag, convert each the upper case to lower case to compare
int i_sensitive(const char* prev, const char* curr){
	const char* str1 = prev;
	const char* str2 = curr;
	char temp_str1;
	char temp_str2;
	while(*str1 != '\0'){
		if(*str1 <= 'Z' && *str1 >= 'A'){
			temp_str1 = *str1 + 32;
		}else{
			temp_str1 = *str1;
		}
		if(*str2 <= 'Z' && *str2 >= 'A'){
			temp_str2 = *str1 + 32;
		}else{
			temp_str2 = *str2;
		}
		int diff = temp_str1 - temp_str2;
		str1++;
		str2++;
		if(diff) return diff; 
	}
	if(*str2 == '\0') return 0;
  	return 1;
}

int readLine(int fd, char *input, int count){
	int curr_ind;
	char elem;
	curr_ind = 0;
	// if the file input is invalid, return -1
	while(curr_ind < count){
		if(read(fd, &elem, 1) < 1) return -1;
		input[curr_ind] = elem;
		curr_ind += 1;
		if(elem == '\n') break;
	}
	input[curr_ind] = '\0';
	return curr_ind;
}

void uniq(int fd){
	char *prev_str, *curr_str, *temp;
	int count;
	count = 1;
	prev_str = prev;
	int i_cond, d_cond;
	if(readLine(fd,prev_str,1024) < 0) exit();
	curr_str = cur;
	while( readLine(fd,curr_str,1024) > 0 ){
		i_cond = (iFlag) ? i_sensitive(prev_str, curr_str) : strcmp(prev_str,curr_str);
		if(i_cond){
			d_cond = count > 1 && dFlag;
			if(!dFlag || d_cond){
				if(cFlag){
					printf(1, "%d %s", count, prev_str);
				}else{
					printf(1, "%s", prev_str);
				}
			}
			temp = curr_str;
			curr_str = prev_str;
			prev_str = temp;
			count = 1;
		}else{
			count++;
		}
	}
	d_cond = count > 1 && dFlag;
	if(d_cond || !dFlag){
		if(cFlag){
			printf(1, "%d %s", count, prev_str);
		}else{
			printf(1, "%s", prev_str);
		}
	}
}


int main(int argc, char *argv[]) {
	int fd, i;
	int condC, condD, condI;
	cFlag = 0, dFlag = 0, iFlag = 0;
	if(argc <= 1){
		uniq(0);
		exit();
	}

	for(i = 1; i < argc; i++){
		switch(argv[i][1]){
			case 'c':
				cFlag = 1;
				break;
			case 'd':
				dFlag = 1;
				break;
			case 'i':
				iFlag = 1;
				break;
		}
	}

	if(cFlag == 1 && dFlag == 1){
		printf(1, "uniq: cannot open -c -d at same time\n");
		exit();
	}

	condC = strcmp(argv[(argc-1)], "-c");
	condD = strcmp(argv[(argc-1)], "-d");
	condI = strcmp(argv[(argc-1)], "-i");

	if(condC == 0 || condD == 0 || condI == 0){
		uniq(0);
	} else if ((fd = open(argv[(argc-1)], 0)) < 0){
		printf(1, "uniq: cannot open %s\n", argv[(argc-1)]);
		exit();
	}
	uniq(fd);
	close(fd);
	exit();
}