#ifndef XF_COMMOM_H
#define XF_COMMOM_H
/* for tts*/

#define  login_params          "appid = 573bdbff , work_dir = ."
//登录参数,appid与msc库绑定,请勿随意改动
#define  session_begin_params  "engine_type =cloud,text_encoding=UTF8,voice_name=mengmeng,sample_rate=16000,speed=90,volume =50,pitch=50,rdn=2"


/* */

/*for asr**********************************************************/
#define SAMPLE_RATE 16000
#define CHANNLE 1
#define FRAMES_SIZE 3200
#define FORMAT SND_PCM_FORMAT_S16_LE
#define PER_SAMPLE 2
#define DEVICE	"default"

#define SAMPLE_RATE_16K     (16000)
#define SAMPLE_RATE_8K      (8000)
#define MAX_GRAMMARID_LEN   (32)
#define MAX_PARAMS_LEN      (1024)

#define ASR_RES_PATH         "fo|/Robot/voice/bin/msc/res/asr/common.jet"
//离线语法识别资源路径
#define  GRM_BUILD_PATH       "/Robot/voice/bin/msc/res/asr/GrmBuilld"
//构建离线语法识别网络生成数据保存路径
#define  GRM_FILE             "/Robot/voice/call.bnf"
//构建离线识别语法网络所用的语法文件
#define  listened_file       "/Robot/voice/wav/ddhgdw.pcm"
#define login_config         "appid = 573bdbff"
//登录参数

/*end asr---------------------------------------------------------*/
#endif

