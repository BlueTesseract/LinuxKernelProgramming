#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define NETLINK_TESSERACT 31
#define MAX_PAYLOAD 1024


int main()
{
	int sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TESSERACT);
	if (sock_fd < 0) {
		printf("Sender: couldn't create netlink socket\n");
		return -1;
	}

	struct sockaddr_nl src_addr, dst_addr;
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();
	bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));

	memset(&dst_addr, 0, sizeof(dst_addr));
	dst_addr.nl_family = AF_NETLINK;
	dst_addr.nl_pid = 0;
	dst_addr.nl_groups = 0;

	struct nlmsghdr *nlh = malloc(NLMSG_SPACE(MAX_PAYLOAD));
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_flags = 0;

	strcpy(NLMSG_DATA(nlh), "Hello Kernel");

	struct iovec iov = { nlh,  nlh->nlmsg_len};
	struct msghdr msg = { &dst_addr, sizeof(dst_addr), &iov, 1, NULL, 0, 0 };

	printf("Sending message to kernel\n");
	sendmsg(sock_fd, &msg, 0);
	printf("Done!\n");

	free(nlh);
	close(sock_fd);

	return 0;
}
