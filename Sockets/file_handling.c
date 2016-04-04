#include <stdio.h>
#include <fcntl.h>

int main(){

	char buf[1024];
	int sent_block;
	int fd = open("/home/sanket/Downloads/Timeline.pdf", O_RDONLY | O_BINARY, 0644);
	if(!fd){
		printf("Error opening file\n");
		return 0;
	}
	while(sent_block = read(fd, buf, 256) > 0){
		printf("%d\n", sent_block);

	}
	return 0;
}