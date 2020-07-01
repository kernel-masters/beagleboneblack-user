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
#include <linux/timer.h>
#include <linux/moduleparam.h>
#include <asm/uaccess.h>

static void timer_gpio(void);
//static struct timer_list timer;

static int gpio_out=10;

module_param(gpio_out,int,0);

static int init_gpio (void)
{
  int err;

  if ((err = gpio_request(gpio_out, THIS_MODULE->name)) != 0) {
    return err;
  }
  if ((err = gpio_direction_output(gpio_out, 1)) != 0) {
    gpio_free(gpio_out);
    return err;
  }

  timer_gpio();
 // init_timer (& timer);
 // timer.function = timer_gpio;
  //timer.data = 0;
 // timer.expires = jiffies + 1*HZ;
 // add_timer(&timer);

  return 0;
}

static void exit_gpio (void)
{
 // del_timer(& timer);
  gpio_free(gpio_out);
}

static void timer_gpio()
{
  static int value = 0;
  int i=0;
  for(i=0;i<10;i++)
  {	  
  printk (KERN_INFO "my_sender_thread sent a message at jiffies=%ld\n", jiffies);
  set_current_state(TASK_INTERRUPTIBLE);
  schedule_timeout(1*HZ);

  gpio_set_value(gpio_out, value);
  value = 1 - value;
  //mod_timer(& timer, jiffies + 1*HZ);
  }
}

module_init(init_gpio);
module_exit(exit_gpio);
MODULE_LICENSE("GPL");


