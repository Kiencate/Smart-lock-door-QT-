#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <asm/atomic.h>
#include <linux/mutex.h>
#include <linux/spi/spi.h>
#include "linux/i2c-dev.h"
#include "linux/i2c.h"
#include "rc522_api.h"
#include <linux/workqueue.h>


#define  N_1  1
#define  N_2  2
#define  N_3  3
#define  N_4  4
#define  N_NOP  255



#define  RELOAD_COUNT  0xfb    //18.432M  9600  0xfb
//         4800  0xf6 
#define CHANGE_PASSWD 1
#define CHANGE_BLOCK  2
#define READ_CARD     3  
#define WRITE_CARD    4
#define CHANGE_KEY    5
#define GET_ID 	      6

typedef unsigned char uchar;
uchar NewKey[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x07,0x80,0x69,0x00,0x00,0x00,0x00,0x00,0x00};

/*static DECLARE_WAIT_QUEUE_HEAD(rc522_wait);*/
static unsigned char Read_Data[16]={0x00};
static unsigned char read_data_buff[16];

static uchar PassWd[6]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static unsigned char RevBuffer[30];
static unsigned char MLastSelectedSnr[4];

uint KuaiN;
uchar operationcard;

struct workqueue_struct *rc522_wq;
struct work_struct rc522_work;
struct i2c_adapter *adapter = NULL; 
struct i2c_client *client = NULL;

void delay_ms(uint tms)
{
	mdelay(tms);
}
void InitRc522(void)
{
	unsigned char a;
	PcdReset();

	a = ReadRawRC(TReloadRegL);
	if(a != 30)
		printk(KERN_DEBUG"NO RC522%d\n",a);
	else
		printk(KERN_DEBUG"RC522 exist\n");
	PcdAntennaOff();  
	PcdAntennaOn();
	M500PcdConfigISOType( 'A' );
}

static char rc522_loop_work(void)
{
	char *pdata = read_data_buff;
	char status;

	PcdReset();
	status=PcdRequest(PICC_REQIDL,&RevBuffer[0]);
	if(status!=MI_OK)
	{
		printk(KERN_DEBUG"search card: no card\n");
		return -EFAULT;
	}
	status=PcdAnticoll(&RevBuffer[2]);
	if(status!=MI_OK)
	{
		printk(KERN_DEBUG"get card nu: no number\n");
		return -EFAULT;
	} 
	memcpy(MLastSelectedSnr,&RevBuffer[2],4);

	status=PcdSelect(MLastSelectedSnr);
	if(status!=MI_OK)
	{
		printk(KERN_DEBUG"select card: no card\n");
		return -EFAULT;            
	}

	status=PcdAuthState(PICC_AUTHENT1A,KuaiN,PassWd,MLastSelectedSnr);
	if(status!=MI_OK)
	{
		printk(KERN_DEBUG"read authorize card err\n");
		return -EFAULT;               
	}
	status=PcdRead(KuaiN,Read_Data);
	if(status!=MI_OK)
	{
		printk(KERN_DEBUG"read card err\n");
		return -EFAULT;
	} else {
		int i;
		memcpy(pdata, Read_Data, sizeof(Read_Data));
		/*wake_up_interruptible(&rc522_wait);*/
		printk(KERN_DEBUG"read block %d info:", KuaiN);
		for(i = 0; i < 16; i++) {
			printk(KERN_DEBUG"%2.2X",pdata[i]);
		}
		printk(KERN_DEBUG"\n");
	}
	
	PcdHalt();	
	return 0;
}



static int rc522_open(struct inode *inode,struct file *filp)
{

	InitRc522();
	printk(KERN_DEBUG"rc522 start work!\n");
	return 0;
}

static ssize_t rc522_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	if(rc522_loop_work())
		return 0;
	printk(KERN_DEBUG"card info:%2.2X\n",Read_Data[0]);
	printk(KERN_DEBUG"card info:%2.2X\n",Read_Data[1]);
	printk(KERN_DEBUG"card info:%2.2X\n",Read_Data[2]);
	printk(KERN_DEBUG"card info:%2.2X\n",Read_Data[3]);
	printk(KERN_DEBUG"card info:%2.2X\n",Read_Data[4]);
	printk(KERN_DEBUG"card info:%2.2X\n",Read_Data[5]);
	printk(KERN_DEBUG"card info:%2.2X\n",Read_Data[6]);
	if (copy_to_user(buf, read_data_buff, sizeof(read_data_buff))) {
		printk(KERN_DEBUG"copy card number to userspace err\n");
		return 0;
	}
	return sizeof(read_data_buff);
}


static int rc522_release(struct inode *inode,struct file *filp)
{
	printk(KERN_DEBUG"%s\n", __func__);
	return 0;
}


static unsigned int rc522_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;

	return mask;
}

static long rc522_ioctl(struct file *file, unsigned int cmd, unsigned long arg) 
{
	switch(cmd) {
		case READ_CARD:
			break;
		default:
			break;
	}
	return 0;
}


static struct file_operations rc522_fops = {
	.owner = THIS_MODULE,
	.open = rc522_open,
	.release = rc522_release, 
	.read = rc522_read,
	.poll = rc522_poll, 
	.unlocked_ioctl = rc522_ioctl,
};

static struct miscdevice rc522_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "rfid_rc522_dev",
	.fops = &rc522_fops,
};

// i2c driver
static const struct i2c_device_id rc522_id[] = {
        {"rc522", 0 },
        { }
};
MODULE_DEVICE_TABLE(i2c, rc522_id);

static int rc522_probe(struct i2c_client *client, const struct i2c_device_id *id){
        InitRc522();
        return 0;
}

static int rc522_remove(struct i2c_client *client){
    return 0;
}

static struct i2c_driver rc522_driver = {
        .driver = {
            .owner  = THIS_MODULE,
			.name	= "rc522",
		},
        .probe          = rc522_probe,
        .remove         = rc522_remove,
        .id_table	= rc522_id,
};



static struct i2c_board_info board_info = {
                    .type = "rc522",
                    .addr = 0x28, // dia chi cua slave rc522
};

static int RC522_init(void)
{
	int res;
	/* Register the character device (atleast try) */
	printk(KERN_DEBUG"rfid_rc522 module init.\n");

	res =  misc_register(&rc522_misc_device);
	if(res < 0) {
		printk(KERN_DEBUG"device register failed with %d.\n",res);
		return res;
	}
	res = misc_register(&rc522_misc_device);
	if(res < 0) {
		 printk(KERN_DEBUG"device register failed with %d.\n",res);
		return res;
	}
	adapter= i2c_get_adapter(1);
	if( adapter!= NULL ){
        client= i2c_new_client_device(adapter, &board_info);
        if(client!= NULL ){
                i2c_add_driver(&rc522_driver);
        }
        i2c_put_adapter(adapter);
	}

	
	return 0;
}

static void RC522_exit(void)
{
	printk(KERN_DEBUG"module is removed\n");
	misc_deregister(&rc522_misc_device);
}

module_init(RC522_init);
module_exit(RC522_exit);

MODULE_AUTHOR("Kien Truong Long");
MODULE_LICENSE("Dual BSD/GPL");	
