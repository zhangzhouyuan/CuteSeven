#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread.h"
#include "TXNasSDK.h"
#include "string.h"
#include "utils.h"


#include "data_point_processor.h"

#define c_Print_Ctrl_Off        "\033[0m"
#define c_CharColor_Red         "\033[1;31m"

char* g_root = "./";
bool g_bLogin = false;    //用户是否已登录
DU_ThreadPool g_ThreadPool;  //线程池

/**
 * 发送datapoint的回调
 */
void send_dp_callback(unsigned int cookie, unsigned long long from_client, int err_code)
{
	TNOTICE(" cookie[%u] from_client[%llu] err_code[%d] \n",cookie,from_client, err_code);
}

/**
 * 复制datapoint
 */
tx_data_point* dup_data_point(const tx_data_point* data_points, int dp_count)
{
	if(dp_count <= 0 || data_points == NULL) return  NULL;
	tx_data_point* request = new tx_data_point[dp_count];
	memset(request,0,sizeof(tx_data_point)*dp_count);
	for(int i = 0; i < dp_count; ++i) {
		request[i].id = data_points[i].id;
		request[i].seq = data_points[i].seq;
		request[i].ret_code = data_points[i].ret_code;
		//value
		int val_len = strlen(data_points[i].value);
		request[i].value = new char[val_len+1];
		memset(request[i].value,0,val_len+1);
		strncpy(request[i].value,data_points[i].value,val_len);		
	}
	return request;
}

/**
 * 处理datapoint数据
 */
void process_data_point(unsigned long long from_client, tx_data_point * data_points, int data_points_count)
{
	for (int i=0; i<data_points_count; ++i)
	{
		//要返回的响应datapoint
		unsigned int cookie = 0;
		tx_data_point dp_to_send = {0};

		//根据id区分并处理不同的datapoint数据包。这里结合示例H5页面，简单实现了几个datapoint的处理
		data_point_processor::getInstance()->on_process(from_client,&data_points[i],&dp_to_send);

		//返回响应datapoint
		tx_ack_data_points(from_client,&dp_to_send,1,&cookie, send_dp_callback);		 //注意该函数的调用频率限制，每秒最多1次
	
		//释放内存
		if (dp_to_send.value) delete[] dp_to_send.value;
		if(data_points[i].value) delete[] data_points[i].value;
	}

	delete [] data_points;
	data_points = NULL;
}

/**
 * 收到datapoint数据回调
 */
void on_receive_data_point(unsigned long long from_client, tx_data_point * data_points, int data_points_count)
{
	TNOTICE("receive data point from client=%lld count=%d", from_client,data_points_count);
	
	//将datapoint的处理放到线程池中，避免线程堵塞
	DU_Functor<void, TL::TLMaker<unsigned long long , tx_data_point * , int >::Result> tf(process_data_point);
	DU_FunctorWrapper<
		DU_Functor<void, TL::TLMaker<unsigned long long , tx_data_point * , int>::Result> > tfw(tf, from_client,  dup_data_point(data_points, data_points_count), data_points_count);
	g_ThreadPool.exec(tfw);
}

/******************************************************************************************************************
 * relative2abspath\ notify_file_download_complete\ notify_user_login\ notify_user_logout\ notify_user_check_login
 * 传递给NasSDK的 设备相关回调，接口参数定义详见TXNasSDK.h
 *******************************************************************************************************************/
int relative2abspath(unsigned long long from_client, const char* relative_path, char* abs_path, int len)
{
	if(relative_path == NULL ) return -1;
	
	snprintf(abs_path,len,"%s%s",g_root,relative_path);
	TNOTICE("relative2abspath: relative_path = %s  ,  abs_path = %s",relative_path, abs_path);
	return 0;
}

//文件下载完成的回调。NasSDK会在文件下载到设备后，调用该回调来通知用户保存文件
int notify_file_download_complete(unsigned long long from_client, const char* tmp_file_path, int file_type, const char* specified_dst_dir)
{
	if(tmp_file_path == NULL) return -1;

	//int ret = 0;
	char dst_dir[512] = {0};

	//如果spcified_dst_dir 为NULL，用户可以通过file_type来自定义文件保存位置，这里为演示，统一放到当前目录
	if(specified_dst_dir == NULL){
		snprintf(dst_dir,512,"./");
	}else{
		snprintf(dst_dir,512,"%s/%s",g_root,specified_dst_dir);           //存放到specified_dst_dir目录下. specified_dst_dir为相对路径，需转换为绝对路径                              
	}

	const char *filename = strrchr(tmp_file_path,'/');

	char dst_file[1024] = {0};
	snprintf(dst_file,1024,"%s%s",dst_dir,filename);
	char src_file[1024] = {0};
	snprintf(src_file,1024,"%s",tmp_file_path);

	TNOTICE(" copy src_file[%s] to dst_file[%s]",src_file, dst_file);	

	//复制或移动文件
	DU_File::copyFile(src_file,dst_file,true);

	//添加到最近上传文件列表中

	return 0;
}

//登录
int notify_user_login(unsigned long long tinyid,const char* user, const char* pwd, const char* extra_data)
{
	if(user == NULL || pwd == NULL) return -1;

	//使用用户名和密码登陆（这里只做一个简单的示例，假设设备只有一个用户admin，且密码为123456）
	TNOTICE("User Login:  user:%s, pwd:%s",user,pwd);

	if(strcmp(user,"admin") == 0 &&strcmp(pwd,"123456") == 0){		
		g_bLogin = true;//登陆成功，保存用户tinyid和user
		return 0;
	}

	return -1;
}

//登出
int notify_user_logout(unsigned long long tinyid, const char* extra_data)
{
	TNOTICE("logout success!");

	//退出，删除tinyid和user的绑定
	g_bLogin = false;

	return 0;
}

//检查用户登录状态
int notify_user_check_login(unsigned long long tinyid,const char* extra_data, char* user,int user_len)
{
	if(g_bLogin){
		snprintf(user,user_len,"admin");
		return 0;
	}
	return -1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//收到文件(收到C2C transfer通知)
void on_file_in_come(unsigned long long transfer_cookie, const tx_ccmsg_inst_info * inst_info, const tx_file_transfer_info * tran_info)
{
    TNOTICE("file_in_come: cookie[%llu] from uin[%llu]", transfer_cookie, inst_info->target_id);
}

//文件传输进度
void on_transfer_progress(unsigned long long transfer_cookie, unsigned long long transfer_progress, unsigned long long max_transfer_progress)
{
    TNOTICE("========> on file progress %f%% cookie[%llu]", transfer_progress * 100.0 / max_transfer_progress, transfer_cookie);
}


//文件传输结果
void on_transfer_complete(unsigned long long transfer_cookie, int err_code, tx_file_transfer_info* tran_info)
{
    TNOTICE("==========================ontransfercomplete==============================\n");
    TNOTICE("errcode %d, cookie[%llu] bussiness_name [%s], file path[%s], file key[%s], file_type[%d], transtype[%d], channeltype[%d] \n", 
        err_code, transfer_cookie, tran_info->bussiness_name,  tran_info->file_path, tran_info->file_key, tran_info->file_type, tran_info->transfer_type, tran_info->channel_type);
    TNOTICE("===============================================================================\n");
}


/**
 * 登录完成的通知，errcode为0表示登录成功，其余请参考全局的错误码表
 */
void on_login_complete(int errcode) 
{
    if(errcode != 0) {
        TNOTICE("Login failure, error = %d", errcode);
		printf(c_CharColor_Red"[error]Login failure..."c_Print_Ctrl_Off);
		printf(" error_code [%d]\n",errcode);
    } else {
        TNOTICE("Login success");
    }
}

/**
 * 在线状态变化通知， 状态（status）取值为 11 表示 在线， 取值为 21 表示  离线
 * old是前一个状态，new是变化后的状态（当前）
 */
void on_online_status(int old_status, int new_status) 
{
    if(new_status == 11) {
        TNOTICE("device online, new status = %d, old status = %d, din = %llu\n", new_status, old_status, tx_get_self_din());
    } else if (new_status == 21 ) {   
        TNOTICE("device offline, new status = %d, old status = %d\n", new_status, old_status);
    } else {
        TNOTICE("device status unknown, new status = %d, old status = %d\n", new_status, old_status);
    }
}


// 绑定列表变化通知
void on_binder_list_change(int error_code, tx_binder_info * pBinderList, int nCount)
{
	int i = 0;
	for (i = 0; i < nCount; ++i )
	{
		TNOTICE(">>   binder uin[%llu], nick_name[%s]", pBinderList[i].uin, pBinderList[i].nick_name);
	}
}

int readBufferFromFile(char *pPath, char *pBuffer, int nInSize, int *pSizeUsed)
{
	if (!pPath || !pBuffer)
	{
		return -1;
	}

	int uLen = 0;
	FILE * file = fopen(pPath, "rb");
	if (!file)
	{
	    return -1;
	}

	do
	{
	    fseek(file, 0L, SEEK_END);
	    uLen = ftell(file);
	    fseek(file, 0L, SEEK_SET);

	    if (0 == uLen || uLen > nInSize)
	    {
	    	TNOTICE("invalide file or buffer size is too small...\n");
	        break;
	    }

	    *pSizeUsed = fread(pBuffer, 1, uLen, file);

	    fclose(file);
	    return 0;

	}while(0);

    fclose(file);
	return -1;
}

void log_func(int level, const char* module, int line, const char* message)
{
	TNOTICE("%s\t%d\t%s\n", module, line, message);
	//return;
	if (level == 1)
	{
	    FILE * file = fopen("./devicelog", "aw+");
	    if (file)
	    {
	    	fprintf(file, "%s\t%d\t%s\n", module, line, message);
	        fclose(file);
	    }
	}
}

bool initdevice()
{
	char license[256] = {0};
	int nLicenseSize = 0;
	if (0 != readBufferFromFile("./licence.sign.file.txt", license, sizeof(license), &nLicenseSize))
	{
		printf(c_CharColor_Red"[error]get license from file failed..."c_Print_Ctrl_Off"\n");
		return false;
	}

	char guid[32] = {0};
	int nGUIDSize = 0;
	if(0 != readBufferFromFile("./GUID_file.txt", guid, sizeof(guid), &nGUIDSize))
	{
		printf(c_CharColor_Red"[error]get guid from file failed..."c_Print_Ctrl_Off"\n");
		return false;
	}

	char svrPubkey[256] = {0};
	int nPubkeySize = 0;
    if (0 != readBufferFromFile("./1700001577.pem", svrPubkey, sizeof(svrPubkey), &nPubkeySize))
	{
		printf(c_CharColor_Red"[error]get svrPubkey from file failed..."c_Print_Ctrl_Off"\n");
		return false;
	}


	//设备信息
	tx_device_info info = {0};
	info.os_platform            = "Linux";
	info.device_name            = "ZJT-NAS";
	info.device_serial_number   = guid;
	info.device_license         = license;
	info.product_version        = 1;
	info.product_id             = 1700001577;
	info.server_pub_key         = svrPubkey;
	info.test_mode              = 0;
	info.network_type           = network_type_wifi;

	//设备登录、在线状态、消息等相关的事件通知
	//   注意事项：
	//   如下的这些notify回调函数，都是来自硬件SDK内部的一个线程，所以在这些回调函数内部的代码一定要注意线程安全问题
	//   比如在on_login_complete操作某个全局变量时，一定要考虑是不是您自己的线程也有可能操作这个变量
	tx_device_notify notify = {0};
	notify.on_login_complete       = on_login_complete;
	notify.on_online_status        = on_online_status;
	notify.on_binder_list_change = on_binder_list_change;

	// SDK初始化目录，写入配置、Log输出等信息
	//   为了了解设备的运行状况，存在上传异常错误日志 到 服务器的必要
	//   system_path：SDK会在该目录下写入保证正常运行必需的配置信息
	//   system_path_capicity：是允许SDK在该目录下最多写入多少字节的数据（最小大小：10K，建议大小：100K）
	//   app_path：用于保存运行中产生的log或者crash堆栈
	//   app_path_capicity：同上，（最小大小：300K，建议大小：1M）
	//   temp_path：可能会在该目录下写入临时文件
	//   temp_path_capicity：这个参数实际没有用的，可以忽略
	tx_init_path init_path = {0};
	init_path.system_path = "./";
	init_path.system_path_capicity  = 10240;
	init_path.app_path = "./";
	init_path.app_path_capicity  = 1024000;
	init_path.temp_path = "./";
	init_path.temp_path_capicity  = 102400;

	// 设置log输出函数，如果不想打印log，则无需设置。
	// 建议开发在开发调试阶段开启log，在产品发布的时候禁用log。
	//tx_set_log_func(log_func,1,1); //在函数中使用宏控制了，所以一定设置logfunc


	//[1/4]. 初始化设备SDK
	//       若初始化成功，则内部会启动一个线程去执行相关逻辑，该线程会持续运行，直到收到 exit 调用
	//       函数的错误返回值同tx_init_device
	int ret = tx_nas_init_device(&info, &notify, &init_path);
	if (err_null == ret) {
		TNOTICE("tx_nas_init_device success");
	}else {
		printf(c_CharColor_Red"[error]tx_nas_init_device failed..."c_Print_Ctrl_Off"\n");
		printf("error_code [%d]\n",ret);
		return false;
	}

	//[2/4]. 初始化文件传输
	tx_file_transfer_notify file_transfer_nofity = {0};
	file_transfer_nofity.on_transfer_complete 	= on_transfer_complete;
	file_transfer_nofity.on_file_in_come 		= on_file_in_come;
	file_transfer_nofity.on_transfer_progress 	= on_transfer_progress;

	ret = tx_nas_init_file_transfer(&file_transfer_nofity, "./QQ_Recv/");
	if (0 == ret) {
		TNOTICE("tx_nas_init_file_transfer success");
	}else {
		printf(c_CharColor_Red"[error]tx_nas_init_file_transfer failed..."c_Print_Ctrl_Off"\n");
		printf("error_code [%d]\n",ret);
		return false;
	}


	//[3/4]. 初始化datapoint
	tx_data_point_notify data_point_notify = {0};
	data_point_notify.on_receive_data_point = on_receive_data_point;

	ret = tx_nas_init_data_point(&data_point_notify);
	if (0 == ret) {
		TNOTICE("tx_nas_init_data_point success");
	}else {
		printf(c_CharColor_Red"[error]tx_nas_init_data_point failed..."c_Print_Ctrl_Off"\n");
		printf("error_code [%d]\n",ret);
		return false;
	}


	//[4/4]. 初始化 Nas
	//       如果没有账号管理，则user相关的三个回调可以不注册
	tx_nas_device_mgr_wrapper device_mgr_wrapper = {0};
	device_mgr_wrapper.relative2abspath                        = relative2abspath;
	device_mgr_wrapper.notify_file_download_complete = notify_file_download_complete;
	device_mgr_wrapper.notify_user_check_login            = notify_user_check_login;
	device_mgr_wrapper.notify_user_login                       = notify_user_login;
	device_mgr_wrapper.notify_user_logout                     = notify_user_logout;

	ret = tx_nas_init(&device_mgr_wrapper);
	if (0 == ret) {
		TNOTICE("tx_nas_init success");
	}
	else {
		printf(c_CharColor_Red"[error]tx_nas_init failed..."c_Print_Ctrl_Off"\n");
		printf("error_code [%d]\n",ret);
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	//初始化线程池
	try {
		g_ThreadPool.init(5);
		g_ThreadPool.start();
	} catch (DU_ThreadPool_Exception& e) {
		TERROR("Init thread pool failure, erorr = %s\n", e.what());
		return EXIT_FAILURE;
	}

	//初始化SDK
	if(!initdevice()){
		TERROR("Init device sdk error\n");
		return EXIT_FAILURE;
	}	

    // 你可以在做其他相关的事情
	// ...

	char input[100] = {0};
	while (scanf("%s", input)) {
		if ( !strcmp(input, "quit") ) {
			tx_nas_uninit();
			break;
		}
		sleep(1);
	}

	g_ThreadPool.stop();
	g_ThreadPool.waitForAllDone();	
}
