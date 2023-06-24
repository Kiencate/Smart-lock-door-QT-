#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x32e21920, "module_layout" },
	{ 0x6627bdce, "i2c_smbus_read_byte_data" },
	{ 0xe76d7921, "i2c_smbus_write_byte_data" },
	{ 0xdf8c695a, "__ndelay" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xc4847533, "i2c_put_adapter" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x5e60f2f1, "misc_register" },
	{ 0xefa56918, "i2c_new_client_device" },
	{ 0x37d70e96, "i2c_register_driver" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x3ae4522f, "i2c_get_adapter" },
	{ 0x7b52b91f, "misc_deregister" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:rc522");

MODULE_INFO(srcversion, "32AC379D1B59C655336942E");
