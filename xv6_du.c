#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

char * fmtname(char * path) {
  static char buf[DIRSIZ + 1];
  char * p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && * p != '/'; p--);
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

void du(char * path, int k, int threshold) {

  char buf[512], * p;
  int fd;
  struct dirent de;
  struct stat st;

  if(k==0)
    k=1;
  else
    k=512;

  if ((fd = open(path, 0)) < 0) {
    printf(2, "du: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, & st) < 0) {
    printf(2, "du: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_FILE:
    printf(1, "%d %s\n", (st.size/k), fmtname(path));
    printf(1, "%d %s\n", (st.size/k), fmtname(path));
    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    * p++ = '/';
    int summarySize=0;
    while (read(fd, & de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, & st) < 0) {
            printf(1, "ls: cannot stat %s\n", buf);
            continue;
        }
        if(st.type==T_FILE && st.size>threshold){
            summarySize+=(st.size/k);
            printf(1, "%d %s\n", (st.size/k), fmtname(buf));
        }
    }
    printf(1, "%d %s\n", summarySize, path);
    break;
  }
  close(fd);
}

int strncmp(const char * p,
  const char * q, uint n) {
  while (n > 0 && * p && * p == * q)
    n--, p++, q++;
  if (n == 0)
    return 0;
  return (uchar) * p - (uchar) * q;
}

int isNumber(char * s) {
  int len = strlen(s);
  while (len > 0) {
    if (strncmp(s, "0123456789", 1) >= 0 && strncmp(s, "0123456789", 1) <= 10) {
      len--;
      s++;
    } else {
      return 0;
    }
  }
  return 1;
}

int main(int argc, char * argv[]) {
  int i, t, k;
  int threshold=0;
  char* fileOrDir = ".";
  for (i = 1; i < argc; i++) {
    if (strncmp(argv[i], "-",1) != 0) {
        if(i-1>=0 && strcmp(argv[i-1], "-t") != 0)
            fileOrDir = argv[i];
    } 
    else {
        if (strcmp(argv[i], "-k") == 0 && k==0) {
            k = 1;
            // printf(1, "we got k\n");
        } 
        else if (strcmp(argv[i], "-t") == 0 && t==0) {
            // printf(1, "we got t\n");
            if ((i + 1 < argc) && isNumber(argv[i + 1])) {
                t = 1;
                threshold=atoi(argv[i + 1]);
                // printf(1, "\"%s\" valid t param\n", argv[i + 1]);
            } 
            else {
                // printf(1, "\"%s\" invalid t param\n", argv[i + 1]);
                printf(1,"check usage.\n");
                exit();
            }
        }
        else {
            printf(1,"check usage.\n");
            exit();
        }
    }
  }
//   printf(1, "%d %d %d %s\n", t,threshold, k, fileOrDir);
  du(fileOrDir,k,threshold);
  exit();
}