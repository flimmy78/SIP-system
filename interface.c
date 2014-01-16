/*
 * interface.c
 *
 *  Created on: 2014年1月2日
 *      Author: root
 */
#include "interface.h"

#include <stdio.h>
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <memory.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

pid_t pid = -1;

//////////////////////////////////////////////////////////////
//begin interface beteewn IPC and NVR
/*uac Transport beteewn IPC and NVR interface begin*/

int uac_get_Transportsdp(char *sdp_data)
{
	snprintf(sdp_data,21,"uac_get_Transportsdp\n");
	printf("uac_get_Transportsdp:%s\n",sdp_data);
	return 0;}

int uac_handle_Transportsdp(char *sdp_data)
{
	printf("uac_handle_Transportsdp:%s\n",sdp_data);
	return 0;}

int uac_send_Transportmedia(char * peer_location)
{
	printf("uac_send_Transportmedia\n");
	return 0;}

int uac_close_Transportmedia()
{
	printf("uac_close_Transportmedia\n");
	return 0;}

/*uac Transport beteewn IPC and NVR interface end*/

/*uas Transport beteewn IPC and NVR interface begin*/

int uas_handle_Transportsdp(char *sdp_data)
{
	printf("uas_handle_Transportsdp:%s\n",sdp_data);
	return 0;}

int uas_get_Transportsdp(char *sdp_data)
{
	snprintf(sdp_data,21,"uas_get_Transportsdp");
	printf("uas_get_Transportsdp:%s\n",sdp_data);
	return 0;}

int uas_receive_Transportmedia(char * peer_location)
{
	printf("uas_receive_Transportmedia\n");
	return 0;}

int uas_close_Transportmedia()
{
	printf("uas_close_Transportmedia\n");
	return 0;}

/*uas Transport beteewn IPC and NVR interface end*/

//end interface beteewn IPC and NVR
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//begin Play interface
//begin Play interface in uac
/*filled by yaoyao*/ // get sdp, fill in INVITE, send to media server by client
int uac_get_Playsdp(char *sdp_data)
{/*
	snprintf(sdp_data,1024,
			"v=0\r\n"
            "o=josua  0 0 IN IP4 192.168.1.1\r\n"
            "s=Playback\r\n"
			  "u=34020000001310000054:3\r\n"
            "c=IN IP4 192.168.1.1\r\n"
            "t=11111 22222\r\n"
            "m=audio 8000 RTP/AVP 0 8 101\r\n");*/
	snprintf(sdp_data,1024,
				"v=0 \r\n"
"o=- 0 0 IN IP4 127.0.0.1\r\n"
"s=Play\r\n"
"c=IN IP4 0.0.0.0\r\n"
"t=0 0\r\n"
"a=tool:libavformat 55.12.102\r\n"
"m=video 0 RTP/AVP 96\r\n"
"a=rtpmap:96 H264/90000\r\n"
"a=fmtp:96 packetization-mode=1\r\n"
"a=control:streamid=0\r\n"
);
	return 0;
}

/*filled by yaoyao*/ // handle sdp received from media server in client
int uac_handle_Playsdp(char *sdp_data)
{
	// nothing to do, just print
	printf("sdp_data in uac_handle_Playsdp: %s\n",sdp_data);
	return 0;
}

/*filled by yaoyao*/ // start request: media receiving process from media server in client
int uac_receive_Playmedia(char * peer_location)
{
	// start ffplay process
    if(pid < 0){
            char *ffplay_prog_dir="";//"/home/yaoyao/ffmpeg_sources/ffplay/";
            char ffplay_cmd[256];
            char *ffplay_cmd_ptr = ffplay_cmd;
            snprintf(ffplay_cmd_ptr, 255,
            "%sffplay rtsp://192.168.115.42:5454/live.h264 >/dev/null 2>/dev/null",
            ffplay_prog_dir);

            printf(ffplay_cmd_ptr);
            printf("\n");

            if((pid = fork()) < 0){
                    perror("fork()");
            }else if(pid == 0){
                    if(execl("/bin/sh", "sh", "-c", ffplay_cmd, (char *)0) < 0){
                            perror("execl failed");
                    }
                    pid++;
            }else{}
    }
	return 0;
}

/*filled by yaoyao*/ // close media receiving process from media server in client
int uac_close_Playmedia()
{

	// terminate ffplay process
    printf("kill %d\n",pid);
    kill(pid,SIGABRT);
    wait(NULL);
    pid++;
    printf("kill %d\n",pid);
    kill(pid,SIGABRT);
    wait(NULL);

    pid = -1;
	return 0;
}

//end Play interface in uac

//begin Play interface in uas
/*filled by yaoyao*/ // handle sdp data via INVITE received from client in media server
int uas_handle_Playsdp(char *sdp_data)
{
	// nothing to do, just print
	printf("sdp_data in uas_handle_Playsdp: %s\n",sdp_data);
	return 0;
}

/*filled by yaoyao*/ // get sdp data for sending to client in media server
/*filled by yaoyao*/ // p -> 1024 bytes
int uas_get_Playsdp(char *sdp_data)
{
	/*
	snprintf(sdp_data, 1024,
			"v=0\r\n"
			"o=%s 0 0 IN IP4 \r\n"
			"s=PLAY\r\n"
			"c=IN IP4 \r\n"
			"t=0 0\r\n"
			"m=video  STP/AVP 96\r\n"
			"a=sendonly\r\n"
			"a=rtpmap:96 H264/90000\r\n"
			"f=\r\n");*/
	snprintf(sdp_data,1024,
					"v=0 \r\n"
	"o=- 0 0 IN IP4 127.0.0.1 \r\n"
	"s=Play \r\n"
	"c=IN IP4 0.0.0.0 \r\n"
	"t=0 0 \r\n"
	"a=tool:libavformat 55.12.102 \r\n"
	"m=video 0 RTP/AVP 96 \r\n"
	"a=rtpmap:96 H264/90000 \r\n"
	"a=fmtp:96 packetization-mode=1 \r\n"
	"a=control:streamid=0 \r\n"
	);

	return 0;
}

/*filled by yaoyao*/ // start response: media sending process to client in media server
int uas_send_Playmedia(char * peer_location)
{
	// Nothing to do, because ffserver is running before uas started.
	return 0;
}

/*filled by yaoyao*/ // close media sending process to client in media server
int uas_close_Playmedia()
{
	// Nothing to do, because ffserver will be running all the time.
	return 0;
}
//end Play interface in uas
//end Play interface
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//begin History interface

//uac
// get rtsp data, fill in INFO for sending to media server by client
int uac_get_Historyrtsp(char *rtsp_data, struct st_rtsptype  *ptr_st_rtsptype)
{
	// if rtsp datatype in {"PLAY", "FAST", "SLOW"}: check scale
	snprintf(rtsp_data,1024,
				"this is test data!");
	printf("uac_get_Historyrtsp\n");
	return 0;
}

// handle MESSAGE, received from media server in client
int handle_HistoryEOFmessage(char *message)
{
	printf("uac_handle_HistoryEOFmessage\n");
	return 0;
}

//uas
// handle rtsp data via INFO, received from client by media server
int uas_handle_Historyrtsp(char *rtsp_data)
{
	printf("uas_handle_Historyrtsp\n");
	return 0;
}

// get MESSAGE for sending to client in media server
// p -> 1024 bytes
int get_HistoryEOFmessage(char *message, char *message_type)
{
	// message_type: "EOF"
	snprintf(message,1024,
			"<?xml version=\"1.0\"?>"
			"<Notify>"
			"<CmdType>MediaStatus</CmdType>"
			"<SN>8</SN>"
			"<DeviceID>000</DeviceID>"
			"<NotifyType>121</NotifyType>"
			"</Notify>");
	return 0;
}

//end History interface
//////////////////////////////////////////////////////////////



/*
int uas_function_run(funcP fun_name,void(*arg))
{
	(*fun_name)(arg);
	return 0;
	}
*/


int interface_init()
{
	user_type=0;
	call_type=0;

	invite_user_type=0;
	invite_type=0;
/*
	uas_handle_invite_sdp=uas_handle_Playsdp;
	uas_get_invite_sdp=uas_get_Playsdp;
	uas_start_transport=uas_send_Playmedia;
	uas_handle_Message=uas_handle_rtsp;
	uas_stop_transport=uas_close_Playmedia;
	*/
	//uas_get_info=uas_get_message;
	return 0;
}

int uac_get_sdp(char *sdp_data)
{
	if(user_type==USER_TYPE_IPC)
		;//uac_get_Transportsdp(sdp_data);
	else if(user_type==USER_TYPE_CLIENT)
	{
		if(call_type==CALL_TYPE_PLAY)
			uac_get_Playsdp(sdp_data);
		else if(call_type==CALL_TYPE_PLAYBACK)
			;//uac_get_Historysdp(sdp_data);
		else
			return -1;
	}
	else
		return -1;
	return 0;
	}

int uac_handle_sdp(char *sdp_data)
{
	if(user_type==USER_TYPE_IPC)
		;//uac_handle_Transportsdp(sdp_data);
	else if(user_type==USER_TYPE_CLIENT)
	{
		if(call_type==CALL_TYPE_PLAY)
			uac_handle_Playsdp(sdp_data);
		else if(call_type==CALL_TYPE_PLAYBACK)
			;//uac_handle_Historysdp(sdp_data);
		else
			return -1;
	}
	else
		return -1;
	return 0;
}

int uac_start_media(char * peer_location)
{printf("uac_start_media:%s",peer_location);
	if(user_type==USER_TYPE_IPC)
		;//uac_send_Transportmedia(peer_location);
	else if(user_type==USER_TYPE_CLIENT)
	{
		if(call_type==CALL_TYPE_PLAY)
			uac_receive_Playmedia(peer_location);
		else if(call_type==CALL_TYPE_PLAYBACK)
			;//uac_receive_Historymedia(peer_location);
		else
			return -1;
	}
	else
		return -1;
	return 0;
	}

int uac_close_media()
{
	if(user_type==USER_TYPE_IPC)
		;//uac_close_Transportmedia();
	else if(user_type==USER_TYPE_CLIENT)
	{
		if(call_type==CALL_TYPE_PLAY)
			uac_close_Playmedia();
		else if(call_type==CALL_TYPE_PLAYBACK)
			;//uac_close_Historymedia();
		else
			return -1;
	}
	else
		return -1;
	return 0;
}

//uas

int uas_handle_sdp(char *sdp_data)
{
	if(invite_user_type==INVITE_USER_TYPE_IPC)
		;//uas_handle_Transportsdp(sdp_data);
	else if(invite_user_type==INVITE_USER_TYPE_CLIENT)
	{
		if(invite_type==INVITE_TYPE_PLAY)
			uas_handle_Playsdp(sdp_data);
		else if(invite_type==INVITE_TYPE_PLAYBACK)
			;//uas_handle_Historysdp(sdp_data);
		else
			return -1;
	}
	else
		return -1;
	return 0;}

int uas_get_sdp(char *sdp_data)
{
	if(invite_user_type==INVITE_USER_TYPE_IPC)
		;//uas_get_Transportsdp(sdp_data);
	else if(invite_user_type==INVITE_USER_TYPE_CLIENT)
	{
		if(invite_type==INVITE_TYPE_PLAY)
			uas_get_Playsdp(sdp_data);
		else if(invite_type==INVITE_TYPE_PLAYBACK)
			;//uas_get_Historysdp(sdp_data);
		else
			return -1;
	}
	else
		return -1;
	return 0;}

int uas_start_media(char *peer_location)
{
	printf("uas_start_media:%s\n",peer_location);
	if(invite_user_type==INVITE_USER_TYPE_IPC)
		;//uas_receive_Transportmedia(peer_location);
	else if(invite_user_type==INVITE_USER_TYPE_CLIENT)
	{
		if(invite_type==INVITE_TYPE_PLAY)
			uas_send_Playmedia(peer_location);
		else if(invite_type==INVITE_TYPE_PLAYBACK)
			;//uas_send_Historymedia(peer_location);
		else
			return -1;
	}
	else
		return -1;
	return 0;}

int uas_close_media()
{

	if(invite_user_type==INVITE_USER_TYPE_IPC)
		;//uas_close_Transportmedia();
	else if(invite_user_type==INVITE_USER_TYPE_CLIENT)
	{
		if(invite_type==INVITE_TYPE_PLAY)
			uas_close_Playmedia();
		else if(invite_type==INVITE_TYPE_PLAYBACK)
			;//uas_close_Historymedia();
		else
			return -1;
	}
	else
		return -1;
	return 0;}

//end uas interface


//begin register interface

int handle_401_Unauthorized_data(void *data)
{
	printf("handle_401_Unauthorized_data:%s\n",data);
	return 0;}

int get_register2_data(void *data)
{
	memcpy(data,"+register2_data+", 17);
	printf("get_register2_data:%s\n",data);
	return 0;}

int handle_response_data(void *data)
{
	printf("handle_response_data:%s\n",data);
	return 0;}

//end register interface

