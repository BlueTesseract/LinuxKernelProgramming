#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

#define NETLINK_TESSERACT 31

struct sock *nl_sk = NULL;

static void listener_nl_recv_msg(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;

	printk(KERN_INFO "Listener received message:\n");
	nlh = nlmsg_hdr(skb);
	printk(KERN_INFO "\t> %s\n", (char*)nlmsg_data(nlh));
	printk(KERN_INFO "!End of Message!\n");
}

static int __init listener_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.input = listener_nl_recv_msg,
	};

	printk(KERN_INFO "Loading listener module\n");
	nl_sk = netlink_kernel_create(&init_net, NETLINK_TESSERACT, &cfg);
	if (!nl_sk) {
		printk(KERN_ALERT "Error: couldn't create listener's netlink socket\n");
	}
	return 0;
}

static void __exit listener_exit(void)
{
	printk(KERN_INFO "Exitting listener module\n");
	netlink_kernel_release(nl_sk);
}

module_init(listener_init);
module_exit(listener_exit);
