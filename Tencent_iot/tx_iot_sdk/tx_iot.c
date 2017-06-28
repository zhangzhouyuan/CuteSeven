/**
 * 这段代码的目的是用来演示：手Q向设备发送语音消息
 *
 * 手Q发送的语音消息实际上就是发送了一个AMR格式的文件。
 * 首先确保手Q与设备已经绑定，通过tx_init_file_transfer函数设置文件传输相关的回调函数。
 * 
 * 期望的结果是：
 * 设备收到文件后，ontransfercomplete函数中将有文件信息的打印，
 * tx_init_file_transfer第2个参数是接收文件的目录，该目录下将存在刚刚收到的文件。
 * 当然您也可以通过tx_send_file_to（或tx_send_file_to_ex）发送文件（视频，语音，图片等）给手Q。
 */
 
#include <stdio.h>
#include <string.h>
#include "TXSDKCommonDef.h"
#include "TXDeviceSDK.h"
#include "myfifo.h"
char *licence_file="/Robot/Tencent_iot_SDK/licence.sign.file.txt";
char *pem_file="/Robot/Tencent_iot_SDK/1700005043.pem";
char *guid_file="/Robot/Tencent_iot_SDK/GUID_file.txt";
char *device_name="CuteSevenTest" ;
unsigned int product_id=1700005043;
// 处理设备关心的控制端指令
void on_receive_datapoint(unsigned long long from_id, tx_data_point * data_points, int data_points_count)
{
  char cmd[1000];
  int i = 0;
  while (i < data_points_count) {
      if (data_points[i].id == 10000) {
	printf("receive a msg  \n\r");
	printf("dp_id=%d\n",data_points[i].id );
        sprintf(cmd,data_points[i].value);
	fifo_send(cmd);
	printf(cmd);
        printf("\r\n");
      }

      ++i;
  }
}
// DP处理 - 查询传感器列表
void on_query_subdevicelist(unsigned long long from_id, tx_data_point * datapoint) {
 //查询传感器的状态
 //get_sub_device_list();

 //将设备列表信息转化为json字符串
 //char* json_value = ；

 //指令处理结果回传到手机(注意seq一定要配对，不要随便乱填哦)
 //tx_ack_data_point(from_id, datapoint.property_id, json_value, datapoint.seq, ret, 0, 0);
}

/**
 * 登录完成的通知，errcode为0表示登录成功，其余请参考全局的错误码表
 */
void on_login_complete(int errcode) {
    printf("on_login_complete | code[%d]\n", errcode);
}

/**
 * 在线状态变化通知， 状态（status）取值为 11 表示 在线， 取值为 21 表示  离线
 * old是前一个状态，new是变化后的状态（当前）
 */
void on_online_status(int old, int new) {
	printf("online status: %s\n", 11 == new ? "true" : "false"); 
}

/**
 * 文件传输完成后的结果通知
 */
void ontransfercomplete(unsigned long long transfer_cookie, int err_code, tx_file_transfer_info tran_info) {
    printf("================ontransfer complete=====transfer_cookie == %lld ====================\n", transfer_cookie);
    // 这个 transfer_cookie 是SDK内部执行文件传输（接收或发送） 任务 保存的一个标记，在回调完这个函数后，transfer_cookie 将失效
    // 可以根据 transfer_cookie 查询文件的信息

    //在完成回调里直接传回 tran_info
    // tx_file_transfer_info ftInfo = {0};
    // tx_query_transfer_info(transfer_cookie, &ftInfo);

    printf("errcode %d, bussiness_name [%s], file path [%s]\n", err_code, tran_info.bussiness_name,  tran_info.file_path);
    printf("===============================================================================\n");
}

/**
 * 辅助函数: 从文件读取buffer
 * 这里用于读取 license 和 guid
 * 这样做的好处是不用频繁修改代码就可以更新license和guid
 */
bool readBufferFromFile(char *pPath, unsigned char *pBuffer, int nInSize, int *pSizeUsed) {
	if (!pPath || !pBuffer) {
		return false;
	}

	int uLen = 0;
	FILE * file = fopen(pPath, "rb");
	if (!file) {
	    return false;
	}

	fseek(file, 0L, SEEK_END);
	uLen = ftell(file);
	fseek(file, 0L, SEEK_SET);

	if (0 == uLen || nInSize < uLen) {
		printf("invalide file or buffer size is too small...\n");
		return false;
	}

	*pSizeUsed = fread(pBuffer, 1, uLen, file);
    // bugfix: 0x0a is a lineend char, no use.
    if (pBuffer[uLen-1] == 0x0a)
    {
        *pSizeUsed = uLen - 1;
        pBuffer[uLen -1] = '\0';
    }

    printf("len:%d, ulen:%d\n",uLen, *pSizeUsed);
	fclose(file);
	return true;
}

/**
 * 辅助函数：SDK的log输出回调
 * SDK内部调用改log输出函数，有助于开发者调试程序
 */
void log_func(int level, const char* module, int line, const char* message)
{
	printf("%s\n", message);
}

/**
 * SDK初始化
 * 例如：
 * （1）填写设备基本信息
 * （2）打算监听哪些事件，事件监听的原理实际上就是设置各类消息的回调函数，
 *  例如设置Datapoint消息通知回调：
 *  开发者应该定义如下的 my_on_receive_data_point 函数，将其赋值tx_data_point_notify对象中对应的函数指针，并初始化：
 *
 *          tx_data_point_notify msgOnRecv= {0};
 *          msgOnRecv.on_receive_data_point = my_on_receive_data_point;
 *          tx_init_data_point(&msgOnRecv);
 *
 *  那么当SDK内部的一个线程收到对方发过来的DataPoint消息后（通过服务器转发），将同步调用 msgOnRecv.on_receive_data_point 
 */
bool initDevice() {
	// 读取 license
    unsigned char license[256] = {0};
    int nLicenseSize = 0;
    if (!readBufferFromFile(licence_file, license, sizeof(license), &nLicenseSize)) {
        printf("[error]get license from file failed...\n");
        return false;
    }

    // 读取guid
    unsigned char guid[32] = {0};
    int nGUIDSize = 0;
    if(!readBufferFromFile(guid_file, guid, sizeof(guid), &nGUIDSize)) {
        printf("[error]get guid from file failed...\n");
        return false;
    }

    char svrPubkey[256] = {0};
    int nPubkeySize = 0;
    if (!readBufferFromFile(pem_file, svrPubkey, sizeof(svrPubkey), &nPubkeySize))
    {
        printf("[error]get svrPubkey from file failed...\n");
        return NULL;
    }

    // 设备的基本信息
    tx_device_info info = {0};
    info.os_platform            = "Linux";

    info.device_name            = device_name;
    info.device_serial_number   = guid;
    info.device_license         = license;
    info.product_version        = 1;
    info.network_type           = network_type_wifi;
    info.product_id             = product_id;
    info.server_pub_key         = svrPubkey;

	// 设备登录、在线状态、消息等相关的事件通知
	// 注意事项：
	// 如下的这些notify回调函数，都是来自硬件SDK内部的一个线程，所以在这些回调函数内部的代码一定要注意线程安全问题
	// 比如在on_login_complete操作某个全局变量时，一定要考虑是不是您自己的线程也有可能操作这个变量
    tx_device_notify notify      = {0};
    notify.on_login_complete     = on_login_complete;
    notify.on_online_status      = on_online_status;
    notify.on_binder_list_change = NULL;
    
    // SDK初始化目录，写入配置、Log输出等信息
    // 为了了解设备的运行状况，存在上传异常错误日志 到 服务器的必要
    // system_path：SDK会在该目录下写入保证正常运行必需的配置信息
    // system_path_capicity：是允许SDK在该目录下最多写入多少字节的数据（最小大小：10K，建议大小：100K）
    // app_path：用于保存运行中产生的log或者crash堆栈
    // app_path_capicity：同上，（最小大小：300K，建议大小：1M）
    // temp_path：可能会在该目录下写入临时文件
    // temp_path_capicity：这个参数实际没有用的，可以忽略
    tx_init_path init_path = {0};
    init_path.system_path = "./";
    init_path.system_path_capicity = 100 * 1024;
    init_path.app_path = "./";
    init_path.app_path_capicity = 1024 * 1024;
    init_path.temp_path = "./";
    init_path.temp_path_capicity = 10 * 1024;

    // 设置log输出函数，如果不想打印log，则无需设置。
    // 建议开发在开发调试阶段开启log，在产品发布的时候禁用log。
    //tx_set_log_func(log_func, 1, 1);

    // 初始化SDK，若初始化成功，则内部会启动一个线程去执行相关逻辑，该线程会持续运行，直到收到 exit 调用
	int ret = tx_init_device(&info, &notify, &init_path);
	if (err_null == ret) {
		printf(" >>> tx_init_device success\n");
	}
	else {
		printf(" >>> tx_init_device failed [%d]\n", ret);
		return false;
	}
	
   // 设置文件传输通知，并设置接收文件的目录，这里设置为 ./recv/
    tx_file_transfer_notify fileTransferNotify = {0};
    fileTransferNotify.on_transfer_complete = ontransfercomplete;
    tx_init_file_transfer(fileTransferNotify, "./recv/");
   //挂接datapoint的处理函数
   tx_data_point_notify api_data_point = {0};
   api_data_point.on_receive_data_point = on_receive_datapoint;
   tx_init_data_point(&api_data_point);
	return true;
}


/****************************************************************
*  测试代码：
*
*  （1）while循环的作用仅仅是使 Demo进程不会退出，实际使用SDK时一般不需要
*
*  （2） 输入 "quit" 将会退出当前进程，这段逻辑存在的原因在于：
*     					在某些芯片上，直接用Ctrl+C 退出易产生僵尸进程
*
*  （3）while循环里面的sleep(1)在这里是必须的，因为如果Demo进程后台运行，scanf没有阻塞作用，会导致当前线程跑满CPU
*
*****************************************************************/
int main(int argc, char* argv[]) {
	if ( !initDevice() ) {
		return -1;
	}
	
	// 你可以在做其他相关的事情
	// ...
	fifo_init(fifoname);  
	char input[100];
	while (scanf("%s", input)) {
		if ( !strcmp(input, "quit") ) {
			tx_exit_device();
			break;
		}
		sleep(1);
	}
	
	return 0;
}
