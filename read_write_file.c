#include<stdio.h>

#include<fcntl.h>

#include <unistd.h>

#include<string.h>

int main(void){

    mode_t f_attrib;                         // 声明mode_t型变量，保存文件属性
    f_attrib = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int fd=open("a.txt",O_RDWR+O_CREAT, f_attrib);

    if(fd==-1){

        printf("can not open the file\n");

        return 1;

    }

    char buf[1024]={"I love www.dotcpp.com very much!"},buf2[1024]={"\0"};

    int len=write(fd,buf,strlen(buf));

    lseek(fd,0,SEEK_SET);

    int len2=read(fd,buf2,1024);

    printf("%s\nlen=%d\n",buf2,len);

    close(fd);

    return 0;

}