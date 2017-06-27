#ifndef COMMON_H
#define COMMON_H

#include "TXDeviceSDK.h"


/**************************************************************************************
 * ���������������Ӧ �ṹ�嶨��
 **************************************************************************************/
struct stop_http_server_req
{
	std::string skey;
	int port;
	stop_http_server_req() {
		port = 0;
	}
};

struct stop_http_server_rsp
{
	int port;
	stop_http_server_rsp() {
		port = 0;
	}
};

struct http_server_req
{
	int type;
	int port;
	http_server_req() {
		type = 0;
		port = 0;
	}
};

struct http_server_rsp
{
	int type;  //����type�����÷��ص�json��ʽ
};


/**************************************************************************************
 * �ļ����ص��������Ӧ �ṹ�嶨��
 **************************************************************************************/
struct file_dl_item
{
	std::string path;
	int type;
	std::string task_key; //PC���ɵ�task_key
	bool result;              //�Ƿ���ɹ�
	file_dl_item(const std::string& p, int t, std::string k) {
		path = p;
		type = t;
		task_key = k;
		result = false;
	}
	file_dl_item() {
		type = 0;
		result = false;
	}
};

struct file_dl_req
{
	std::vector<file_dl_item> dl_item;
	unsigned long long target_id;
	unsigned int platform; //0 δ֪��1 PCQQ��2 MQQ
	file_dl_req() {
		target_id = 0;
		platform = 0;
	}
};

struct file_dl_rsp
{
	unsigned long long cookie;
	file_dl_rsp() {
		cookie = 0;
	}
};


/**************************************************************************************
 * ȡ���ļ�������������Ӧ �ṹ�嶨��
 **************************************************************************************/
struct cancel_file_transfer_pair
{
	std::string task_key;
	bool result;
	cancel_file_transfer_pair()
	{
		result = false;
	}
	cancel_file_transfer_pair(const cancel_file_transfer_pair& p)
	{
		task_key = p.task_key;
		result = p.result;
	}
};

struct cancel_file_transfer_req
{
	std::string task_key; //
	std::vector<cancel_file_transfer_pair> task_key_list;
};

struct cancel_file_transfer_rsp
{
	//std::string task_key; //
};

struct file_transfer_detail_req
{
	std::string task_key; //
};

struct file_transfer_detail_rsp
{
	//std::string task_key; //
};

/**************************************************************************************
 * ���񹫹�datapoint���������Ӧ �ṹ�嶨��
 **************************************************************************************/

struct task_req
{
	unsigned int subid;
	task_req(){
		subid = 0;
	}
};

struct task_rsp
{
};

/**************************************************************************************
 * �˺Ź���datapoint���������Ӧ �ṹ�嶨��
 **************************************************************************************/

struct account_req
{
	unsigned int subid;
	account_req(){
		subid = 0;
	}
};

struct account_rsp
{
};


struct login_req
{
	std::string usr;
	std::string pwd;
	std::string extra_data;
};

struct login_rsp
{
	std::string sid;
};

struct logout_req
{
	std::string extra_data;
};

struct logout_rsp
{
	// empty
};

struct check_login_req
{
	std::string extra_data;
};

struct check_login_rsp
{
	bool is_login;
	std::string usr;
	check_login_rsp() {
		is_login = false;
	}
};



/**************************************************************************************
 * 
 **************************************************************************************/
struct ftn_file_upload_buffer_with_file
{
	std::string _task_key;
	std::string _dest_path;
};

//�ļ����Ͷ���
typedef enum enum_file_type_def {
	file_type_def_rich_lite = -2, // ͼƬ����Ƶ����Ƶ,(����)
	file_type_def_rich = -1, // ͼƬ����Ƶ����Ƶ
	file_type_def_doc = 1, //�ĵ�
	file_type_def_pic = 2, //ͼƬ
	file_type_def_audio = 3, //��Ƶ
	file_type_def_video = 4, //��Ƶ
	file_type_def_file = 5, //�ļ�
	file_type_def_dir = 6, //Ŀ¼
	file_type_def_other = 0 //�����ļ�
}file_type_t;

//����ͼ��ʽ����
typedef enum enum_thumb_type_def {
	thumb_type_def_jpg = 1, //jpg
	thumb_type_def_png = 2, //png
	thumb_type_def_bmp = 3, //bmp
	thumb_type_def_other = 0, //����
}thumb_type_t;

//���ظ�H5ҳ��Ĵ�����
typedef enum enum_device_error_code {
	device_error_code_success = 0, //�ɹ�
	device_error_code_unknown_property_id = -1, //δ֪������id
	device_error_code_request_invalid = -2, //����Ƿ�
	device_error_code_get_list = -3, //��ȡ�ļ��б�ʧ��
	device_error_code_file_not_exist = -4, //�ļ�������
	device_error_code_upload_failure = -5, //�ϴ��ļ����ļ�ͨ��ʧ��
	device_error_code_login_failure = -6, //�û���������������µ�¼ʧ��
	device_error_code_logout_failure = -7, //�û�δ��¼���޷��ȳ��û�
	device_error_code_file_exist = -8, //�ļ��Ѵ���

	device_error_code_part_success = -9998, //�������ֳɹ��������������غ�����ɾ��
	device_error_code_not_login = -9999, //�û�δ��½
	device_error_code_not_impl = -10000, //δʵ��
	device_error_code_unknown_error = -10001, //δ֪����
}device_error_code_t;


//���ߵĻ�����Ϣ
typedef struct binder_info {
	tx_binder_info* _binder_list;
	int _count;
	DU_ThreadLock _lock;
	binder_info() {
		_binder_list = NULL;
		_count = 0;
	}

	void free() {
		if(_binder_list != NULL) {
			delete [] _binder_list;
			_binder_list = NULL;
			_count = 0;
		}
	}

	void dup(const tx_binder_info* bf, int count) {
		DU_ThreadLock::Lock lock(_lock);
		//��������һ�ε�����
		free();
		if(count > 0 && bf != NULL) {
			_binder_list = new tx_binder_info[count];
			memcpy(_binder_list, bf, count * sizeof(tx_binder_info));
			_count = count;
		}
	}

	//����tinyid��ȡuin��Ϣ
	unsigned long long get_uin_by_tinyid(unsigned long long tinyid) {
		unsigned long long uin = 0;

		for(int i = 0; i < _count; ++i) {
			if(_binder_list[i].tinyid == tinyid) {
				uin = _binder_list[i].uin;
			}
		}

		return uin;
	}
} binder_info_t;



#endif // COMMON_H
