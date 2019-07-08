#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <net/sock.h>
#include <net/netfilter/nf_conntrack.h>


static unsigned int pre_routing_hook(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	if (ip_hdr(skb)->protocol == IPPROTO_ICMP) {
		printk(KERN_INFO "pre_routing_hook: %p:%d\n", skb, icmp_hdr(skb)->type);
//		int err = sock_queue_rcv_skb(sk,skb);
	}
	return NF_ACCEPT;
}

static unsigned int post_routing_hook(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	if (ip_hdr(skb)->protocol == IPPROTO_ICMP) {

		enum ip_conntrack_info ctinfo;
		struct nf_conn *c;
		printk(KERN_INFO "post_routing_hook: %p:%p:%d\n", skb, skb->sk, icmp_hdr(skb)->type);
		c = nf_ct_get(skb, &ctinfo);
		if (c)
			printk(KERN_INFO "   c->status=%ld\n", c->status);
		}
	return NF_ACCEPT;
}

static unsigned int local_out_hook(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	if (ip_hdr(skb)->protocol == IPPROTO_ICMP)
		printk(KERN_INFO "local_out_hook: %p:%p:%d\n", skb, skb->sk, icmp_hdr(skb)->type);
	else
		printk(KERN_INFO "local_out_hook -not icmp-: %p:%d\n", skb, ip_hdr(skb)->protocol);
	return NF_ACCEPT;
}

static unsigned int local_in_hook(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	if (ip_hdr(skb)->protocol == IPPROTO_ICMP) {
		enum ip_conntrack_info ctinfo;
		struct nf_conn *c;
		printk(KERN_INFO "local_in_hook: %p:%d\n", skb, icmp_hdr(skb)->type);
		c = nf_ct_get(skb, &ctinfo);
		if (c)
			printk(KERN_INFO "   c->status=%ld\n", c->status);
	}
	return NF_ACCEPT;
}


static const struct nf_hook_ops nfacc_ops[] = {
	{
		.hook		= pre_routing_hook,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP_PRI_FIRST,
	},
	{
		.hook		= post_routing_hook,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_POST_ROUTING,
		.priority	= NF_IP_PRI_LAST,
	},
	{
		.hook		= local_out_hook,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_LOCAL_OUT,
		.priority	= NF_IP_PRI_FIRST,
	},
	{
		.hook		= local_in_hook,
		.pf		= NFPROTO_IPV4,
		.hooknum	= NF_INET_LOCAL_IN,
		.priority	= NF_IP_PRI_LAST,
	},
};


static int __init nfacc_init(void)
{
	int err;

	err = nf_register_net_hooks(&init_net, nfacc_ops,
				    ARRAY_SIZE(nfacc_ops));

	if (!err)
		printk("module insmoded");
	else
		printk("nf_register_net_hook: error");
	return err;
}

static void __exit nfacc_fini(void)
{
	nf_unregister_net_hooks(&init_net, nfacc_ops,
				ARRAY_SIZE(nfacc_ops));
	printk("module rmmoded");
}


MODULE_DESCRIPTION("Netfilter based packet accelerator");
MODULE_AUTHOR("Mikolaj Kowalik <kowalikmikolaj95@gmail.com>");
MODULE_LICENSE("GPL");

module_init(nfacc_init);
module_exit(nfacc_fini);
