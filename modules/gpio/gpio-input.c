/***************************************************************************
 *      Organisation    : Kernel Masters, KPHB, Hyderabad, India.          *
 *      facebook page   : www.facebook.com/kernelmasters                   *
 *      Website         : www.kernelmasters.org                            *
 *  Conducting Workshops on - Embedded Linux & Device Drivers Training.    *
 *  -------------------------------------------------------------------    *
 *  Tel : 91-9949062828, Email : kernelmasters@gmail.com                   *    
 *                                                                         *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/uaccess.h>

static int gpio_in = 11; // Par defaut : broche 10
module_param(gpio_in, int, 0644);

static dev_t          example_dev;
static struct cdev    example_cdev;
static struct class * example_class = NULL;

static int example_read(struct file * filp, char * buffer, size_t length, loff_t * offset);

static struct file_operations fops_example = {
    .owner   =  THIS_MODULE,
    .read    =  example_read,
  };

static int __init example_init (void)
{
  int err;

  if ((err = gpio_request(gpio_in, THIS_MODULE->name)) != 0) {
    return err;
  }

  if ((err = gpio_direction_input(gpio_in)) != 0) {
    gpio_free(gpio_in);
    return err;
  }

  if ((err = alloc_chrdev_region(& example_dev, 0, 1, THIS_MODULE->name)) < 0) {
    gpio_free(gpio_in);
    return err;
  }

  example_class = class_create(THIS_MODULE, "class_example");
  if (IS_ERR(example_class)) {
    unregister_chrdev_region(example_dev, 1);
    gpio_free(gpio_in);
    return -EINVAL;
  }

  device_create(example_class, NULL, example_dev, NULL, THIS_MODULE->name);

  cdev_init(& example_cdev, & fops_example);

  if ((err = cdev_add(& example_cdev, example_dev, 1)) != 0) {
    device_destroy(example_class, example_dev);
    class_destroy(example_class);
    unregister_chrdev_region(example_dev, 1);
    gpio_free(gpio_in);
    return err;
  }
  return 0;
}

static void __exit example_exit (void)
{
  cdev_del(& example_cdev);
  device_destroy(example_class, example_dev);
  class_destroy(example_class);
  unregister_chrdev_region(example_dev, 1);
  gpio_free(gpio_in);
}

static int example_read(struct file * filp, char * buffer, size_t length, loff_t * offset)
{
  char chaine[32];
  int lg;
  snprintf(chaine, 32, "%d", gpio_get_value(gpio_in));
  lg = strlen(chaine) + 1;
  if (lg > length)
    return -ENOMEM;
  if (copy_to_user(buffer, chaine, lg) != 0)
    return -EFAULT;
  return lg;
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
