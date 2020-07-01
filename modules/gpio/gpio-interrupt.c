/***************************************************************************
 *      Organisation    : Kernel Masters, KPHB, Hyderabad, India.          *
 *      facebook page   : www.facebook.com/kernelmasters                   *
 *      Website		: www.kernelmasters.org                            *
 *  Conducting Workshops on - Embedded Linux & Device Drivers Training.    *
 *  -------------------------------------------------------------------    *
 *  Tel : 91-9949062828, Email : kernelmasters@gmail.com       		   *	
 *									   *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/moduleparam.h>


int gpio_in=11;
int gpio_out=10;
module_param(gpio_in,int,0);
module_param(gpio_out,int,0);


irqreturn_t handler_irq_gpio(int irq, void * ident)
{
  static int value = 1;
  gpio_set_value(gpio_out, value);
  printk ("ISR\n");
  value = 1 - value;
  return IRQ_HANDLED;
}

static int gpio_irq = 0;

int example_init (void)
{
  int err;

  gpio_irq = gpio_to_irq(gpio_in);

   printk("interrupt request number:%d",gpio_irq);

  if ((err = gpio_request(gpio_out, THIS_MODULE->name)) != 0)
    return err;
  if ((err = gpio_direction_output(gpio_out, 0)) != 0) {
    gpio_free(gpio_out);
    return err;
  }

  if ((err = gpio_request(gpio_in, THIS_MODULE->name)) != 0) {
    gpio_free(gpio_out);
    return err;
  }
  if ((err = gpio_direction_input(gpio_in)) != 0) {
    gpio_free(gpio_in);
    gpio_free(gpio_out);
    return err;
  }

  if ((err = request_irq(gpio_irq, handler_irq_gpio, 0  , THIS_MODULE->name, THIS_MODULE->name)) != 0) {
    gpio_free(gpio_in);
    gpio_free(gpio_out);
    return err;
  }
irq_set_irq_type(gpio_irq, IRQF_TRIGGER_FALLING);

  return 0;

}

void example_exit (void)
{
  free_irq(gpio_irq, THIS_MODULE->name);
  gpio_free(gpio_in);
  gpio_free(gpio_out);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");

