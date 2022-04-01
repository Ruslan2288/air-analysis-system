

#include <cpp_main.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "ringbuffer.h"
#include "ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "nbt.h"


#define MSG_DATA_LEN (uint8_t)7

extern uint8_t RxBuffer[RxBufferSize];
struct ringbuffer rb;

ros::NodeHandle nh;
std_msgs::Float32MultiArray airSens_msg;

ros::Publisher airSens("airSens", &airSens_msg);


static nbt_t publish_nbt;
static nbt_t ros_nbt;


extern "C" void cdc_receive_put(uint8_t value)
{
	ringbuffer_putchar(&rb, value);
}

extern "C" void init_ROS()
{
	ringbuffer_init(&rb, RxBuffer, RxBufferSize);

	// Initialize ROS
	nh.initNode();

	nh.advertise(airSens);
	NBT_init(&publish_nbt, 500);
	NBT_init(&ros_nbt, 10);
}

extern "C" void airSens_handler()
{
	  if (NBT_handler(&publish_nbt))
	  {
		  airSens_msg.data_length = MSG_DATA_LEN;
		  airSens.publish(&airSens_msg);
	  }
}


extern "C" void spinOnce()
{
	  if (NBT_handler(&ros_nbt))
	  {
		nh.spinOnce();
	  }
}

