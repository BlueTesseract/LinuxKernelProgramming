#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ip.h>

/* tnd.c - template of net dev
 */

static int template_init(struct net_device *dev);
static void template_uninit(struct net_device *dev);
static netdev_tx_t template_start_xmit(struct sk_buff *skb, struct net_device *dev);
static void template_setup(struct net_device *dev);

struct net_device * dev_template;

static const struct net_device_ops template_netdev_ops = {
	.ndo_init		= template_init,
	.ndo_uninit		= template_uninit,
	.ndo_start_xmit		= template_start_xmit,
};

static int template_init(struct net_device *dev)
{
	printk(__FUNCTION__);
	return 0;
}

static void template_uninit(struct net_device *dev)
{
	printk(__FUNCTION__);
}

static netdev_tx_t template_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct ethhdr *ethh;
	/* struct iphdr *iph; */

	printk(__FUNCTION__);

	ethh = eth_hdr(skb);
	printk("%s: %pM -> %pM, [%04x]\n",
	       __FUNCTION__,
	       &ethh->h_source,
	       &ethh->h_dest,
	       ntohs(ethh->h_proto));

	/* ... */
	/* iph = ip_hdr(skb);
	printk("%s: %pI4 -> %pI4 , l=%d\n",
	       __FUNCTION__,
	       &iph->saddr,
	       &iph->daddr,
	       skb->len);
	*/
	return NETDEV_TX_OK;
}

static void template_setup(struct net_device *dev)
{
	printk(__FUNCTION__);
	ether_setup(dev);
	eth_hw_addr_random(dev);
}

static int __init template_module_init(void)
{
	int err;

	printk(__FUNCTION__);
	dev_template = alloc_netdev(0, "tnd%d", NET_NAME_ENUM, template_setup);
	if (!dev_template)
		return -ENOMEM;

	dev_template->netdev_ops = &template_netdev_ops;
	err = register_netdev(dev_template);
	if (err < 0)
		goto err;

	return 0;
err:
	free_netdev(dev_template);
	return err;
}

static void __exit template_module_exit(void)
{
	printk(__FUNCTION__);
	unregister_netdev(dev_template);
	free_netdev(dev_template);
}

module_init(template_module_init);
module_exit(template_module_exit);
MODULE_LICENSE("GPL");
