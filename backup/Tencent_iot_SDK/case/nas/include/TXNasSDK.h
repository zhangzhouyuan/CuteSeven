#ifndef NAS_SDK_H
#define NAS_SDK_H

#include "TXSDKCommonDef.h"
#include "TXDeviceSDK.h"
#include "TXDataPoint.h"
#include "TXNasSDKCommDef.h"

CXX_EXTERN_BEGIN

/**
* ע���豸��صĻص�����
*/
typedef struct tag_tx_device_mgr_wrapper
{
	// ���·��ת��Ϊ����·��
	// from_client����ǰ���û�tinyid
	// relative_path���ļ����·��
	// abs_path���������ת����ľ���·��
	// len�� abs_path�Ļ����С���ֽڣ�
	// ����ֵ���ɹ�����0���Ƿ񷵻ط�0
	int (* relative2abspath)(unsigned long long from_client, const char* relative_path, char* abs_path, int len);

	// �ļ�������ɵĻص����û��ɱ����ļ����Զ���λ��
	// from_client: �û���tinyid
	// tmp_file_path:  Ϊԭʼ�ļ��������Ե�ַ
	// file_type��  �ļ������ͣ�file_type_pic��file_type_audio��file_type_video��file_type_file��file_type_other
	// specified_dst_dir:  ָ�����ļ���Ҫ�ŵ���Ŀ¼����Ե�ַ�������δָ�������ò���ΪNULL���û������ɾ����ļ����λ��
	// ����ֵ�� �ɹ�����0, ʧ�ܷ��ط�0
	int (* notify_file_download_complete)(unsigned long long from_client, const char* tmp_file_path, int file_type , const char* specified_dst_dir);

	// �û��ĵ�½֪ͨ
	// tinyid�������¼�û���tinyid
	// user ���û���
	// pwd������
	// extra_data�� �û������½ʱ�����͹����ĸ��ӵ����ݣ����������extra_data��������ΪNULL
	// ����ֵ�� ��½�ɹ�����0����½ʧ�ܷ���1
	int (* notify_user_login)(unsigned long long tinyid,const char* user, const char* pwd, const char* extra_data);

	// �û��ĵǳ�֪ͨ
	// tinyid�� Ҫ�ǳ����û���tinyid
	// extra_data���û�����ǳ�ʱ�����͹����ĸ��ӵ����ݣ����������extra_data��������ΪNULL
	// ����ֵ������ǳ��ɹ�����0���ǳ�ʧ�ܷ���1
	int (* notify_user_logout)(unsigned long long tinyid, const char* extra_data);

	//�û��ļ���½֪ͨ
	// tinyid���û���tinyid
	// extra_data���û��������½״̬ʱ�����͹����ĸ��ӵ����ݣ����������extra_data��������ΪNULL
	// user : ����ѵ�¼��user�����û�����user����ռ��ڵ��øú���ʱ�ѷ���
	// user_len: user �����ѷ���Ĵ�С
	// ����ֵ������ѵ�¼����0��δ��¼������ط�0
	int (* notify_user_check_login)(unsigned long long tinyid,const char* extra_data, char* user,int user_len);

}tx_nas_device_mgr_wrapper;


/**
* ��ʼ���豸SDK
* ����ͬtx_init_device
*/
SDK_API int tx_nas_init_device(tx_device_info *info, tx_device_notify *notify, tx_init_path* init_path); 


/**
* ��ʼ��data point ����
* ����ͬtx_init_data_point
*/
int tx_nas_init_data_point(const tx_data_point_notify *notify);

/**
* ��ʼ���ļ�����
* ��������ͬtx_init_file_transfer������ע�����������ָ�룡��
*/
SDK_API int tx_nas_init_file_transfer(tx_file_transfer_notify* notify, char * path_recv_file); 

/**
* ��ʼ��NasSDK
*/
SDK_API int tx_nas_init(tx_nas_device_mgr_wrapper* device_mgr_wrapper); 

/**
* �˳�NasSDK
*/
SDK_API int tx_nas_uninit();


/**
* �ϴ��ļ������������ϴ��ɹ��󣬿�ͨ��tx_nas_get_url��ȡ�ļ���url
* abspath���ļ��ľ��Ե�ַ
* filekey�� �ļ���filekey����������ͨ����filekey����ȡͼƬ��url
*/
SDK_API int tx_nas_upload_file(const char* abspath,const char* filekey);


/**
* ��ȡ�ļ���url
* filekey���ϴ��ļ�ʱ�����filekey
* url�� ���ڷ���url�����û�����
* len�� url����Ĵ�С���ֽڣ�
*/
SDK_API int tx_nas_get_url(const char* filekey, char* url, int len);

/**
* ɾ��filekeyָ����ļ���ɾ�����޷���ͨ��tx_nas_get_url ��ȡ��url
* filekey���ϴ��ļ�ʱ�����filekey
*/
SDK_API int tx_nas_delete_uploaded_file(const char* filekey);


CXX_EXTERN_END

#endif // UTILS_H
