#include "linux/device.h"
#include "linux/input.h"
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/input.h>

/*
* input_polled_dev has been deprecated
*/
struct nunchuk_dev {
	struct input_dev *polled_input;
	struct i2c_client *client;
};

static int nunchuk_read_registers(struct i2c_client *client, u8 *buf,
				  int buf_size)
{
	int status;
	mdelay(10);

	buf[0] = 0x00;
	status = i2c_master_send(client, buf, 1);
	if (status >= 0 && status != 1) {
		return -EIO;
	}
	if (status < 0) {
		return status;
	}
	mdelay(10);
	status = i2c_master_recv(client, buf, buf_size);
	if (status >= 0 && status != buf_size) {
		return -EIO;
	}
	return status;
}

static void nunchuk_poll(struct input_dev *polled_input)
{
	u8 buf[6];
	int joy_x, joy_y, c_button, z_button, accel_x, accel_y, accel_z;
	struct i2c_client *client;
	struct nunchuk_dev *nunchuk;
	nunchuk = input_get_drvdata(polled_input);
	client = nunchuk->client;
	if (nunchuk_read_registers(client, buf, ARRAY_SIZE(buf)) < 0) {
		dev_info(&client->dev,
			 "Error reading the nunchuk registers.\n");
		return;
	}
	joy_x = buf[0];
	joy_y = buf[1];
	z_button = (buf[5] & BIT(0) ? 0 : 1);
	c_button = (buf[5] & BIT(1) ? 0 : 1);
	accel_x = (buf[2] << 2) | ((buf[5] >> 2) & 0x3);
	accel_y = (buf[3] << 2) | ((buf[5] >> 4) & 0x3);
	accel_z = (buf[4] << 2) | ((buf[5] >> 6) & 0x3);
	input_report_abs(polled_input, ABS_X, joy_x);
	input_report_abs(polled_input, ABS_Y, joy_y);
	input_report_key(polled_input, BTN_Z, z_button);
	input_report_key(polled_input, BTN_C, c_button);
	input_report_abs(polled_input, ABS_RX, accel_x);
	input_report_abs(polled_input, ABS_RY, accel_y);
	input_report_abs(polled_input, ABS_RZ, accel_z);
	input_sync(polled_input);
}

static int nunchuk_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	int ret;
	u8 buf[2];
	struct device *dev = &client->dev;
	struct nunchuk_dev *nunchuk;
	struct input_dev *input;
	dev_info(dev, "enter nunchuk_probe");
	nunchuk = devm_kzalloc(dev, sizeof(struct nunchuk_dev), GFP_KERNEL);
	if (nunchuk == NULL) {
		dev_err(dev, "alloc mem for nunchuk failed!");
		return -ENOMEM;
	}
	i2c_set_clientdata(client, nunchuk);
	input = devm_input_allocate_device(dev);
	if (input == NULL) {
		dev_err(dev, "alloc mem for input device failed!");
		return -ENOMEM;
	}
	nunchuk->client = client;
	nunchuk->polled_input = input;
	input->name = "WII Nunchuk";
	input->id.bustype = BUS_I2C;

	input_set_drvdata(input, nunchuk);
	input_setup_polling(input, nunchuk_poll);
	input_set_poll_interval(input, 50);

	set_bit(EV_KEY, input->evbit);
	set_bit(BTN_C, input->keybit);
	set_bit(BTN_Z, input->keybit);
	set_bit(EV_ABS, input->evbit);
	set_bit(ABS_X, input->absbit); /* joystick */
	set_bit(ABS_Y, input->absbit);
	set_bit(ABS_RX, input->absbit); /* accelerometer */
	set_bit(ABS_RY, input->absbit);
	set_bit(ABS_RZ, input->absbit);

	input_set_abs_params(input, ABS_X, 0x00, 0xff, 0, 0);
	input_set_abs_params(input, ABS_Y, 0x00, 0xff, 0, 0);
	input_set_abs_params(input, ABS_RX, 0x00, 0x3ff, 0, 0);
	input_set_abs_params(input, ABS_RY, 0x00, 0x3ff, 0, 0);
	input_set_abs_params(input, ABS_RZ, 0x00, 0x3ff, 0, 0);

	buf[0] = 0xf0;
	buf[1] = 0x55;
	ret = i2c_master_send(client, buf, 2);
	if (ret >= 0 && ret != 2)
		return -EIO;
	if (ret < 0) {
		pr_err("handshake failed errcode: %d\n", ret);
		return ret;
	}
	udelay(1);
	buf[0] = 0xfb;
	buf[1] = 0x00;
	ret = i2c_master_send(client, buf, 1);
	if (ret >= 0 && ret != 1)
		return -EIO;
	if (ret < 0) {
		pr_err("handshake two phase failed errcode: %d\n", ret);
		return ret;
	}
	ret = input_register_device(input);
	return ret;
}

static void nunchuk_remove(struct i2c_client *client)
{
	struct nunchuk_dev *nunchuk;
	nunchuk = i2c_get_clientdata(client);
	input_unregister_device(nunchuk->polled_input);
	dev_info(&client->dev, "nunchuk_remove()\n");
}
/* Add entries to Device Tree */
static const struct of_device_id nunchuk_of_match[] = {
	{ .compatible = "nunchuk" },
	{},
};
MODULE_DEVICE_TABLE(of, nunchuk_of_match);

static const struct i2c_device_id nunchuk_id[] = {
	{ "nunchuk", 0 },
	{},
};
MODULE_DEVICE_TABLE(i2c, nunchuk_id);

static struct i2c_driver nunchuk_driver = {
	.driver = { .name = "nunchuk",
		    .owner = THIS_MODULE,
		    .of_match_table = nunchuk_of_match },
	.probe = nunchuk_probe,
	.remove = nunchuk_remove,
	.id_table = nunchuk_id,
};

module_i2c_driver(nunchuk_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("dandan");
MODULE_DESCRIPTION("This is a Nunchuk Wii I2C driver");