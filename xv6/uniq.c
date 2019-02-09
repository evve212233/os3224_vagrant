#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void uniq(int fd, char *name) {
	int i, n;
	int l, c;
	l = c = 0;
	char string[512];
	char prev[512];

	while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (i = 0; i < n; i++) {
        	string[c] = buf[i];
            c++;
            if (buf[i] == '\n') {
                string[c] = '\0';
                prev[l] = '\0';
                l = strlen(string);
                if (strcmp(prev, string) !=0){
                	write(1, string, strlen(string));
                }
                strcpy(prev, string);
                memset(string, 0, 512);
                c = 0;
            }
        }
    }
    if(n < 0){
	    printf(1, "uniq: read error\n");
	    exit();
  }
    if (strcmp(prev, string) !=0){
        write(1, string, strlen(string));
    }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    uniq(0, "");
    exit();
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "wc: cannot open %s\n", argv[i]);
      exit();
    }
    uniq(fd, argv[i]);
    close(fd);
  }
  exit();
}
