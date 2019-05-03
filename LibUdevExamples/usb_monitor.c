#include <stdio.h>
#include <unistd.h>
#include <libudev.h>

int main()
{
	struct udev *udev;
	struct udev_device *dev;
	struct udev_enumerate *enm;
	struct udev_list_entry *devs, *dev_list_entry;


	udev = udev_new();
	if (!udev) {
		printf("Can't create udev\n");
		return -1;
	}

	enm = udev_enumerate_new(udev);
	if (!enm) {
		printf("Cannot create enumerate\n");
		return -1;
	}
	udev_enumerate_add_match_subsystem(enm, "usb");
	udev_enumerate_scan_devices(enm);

	devs = udev_enumerate_get_list_entry(enm);
	udev_list_entry_foreach(dev_list_entry, devs) {
		const char *path;
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);
		printf("devnode = %s\n", udev_device_get_devnode(dev));
		dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
		if (!dev) {
			printf("Cannot get device parent\n");
			continue;
		}
		printf("VID/PID: %s %s\n",
			udev_device_get_sysattr_value(dev,"idVendor"),
			udev_device_get_sysattr_value(dev, "idProduct"));

		udev_device_unref(dev);
	}

	return 0;
}
