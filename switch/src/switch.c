/*
 ============================================================================
 Name        :
 Author      : zhich
 Version     :
 Copyright   : Your copyright notice
 Description : 主线程为tcp端，第二线程为udp端
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>

#include <curl/curl.h>
#include "cJSON.h"
#include <pthread.h>


#define DEBUG
#define DEBUG_TEST
#define SERVERPORT 55555	//TCP端口号
#define UDPPORT 60000		//UDP端口号
#define BACKLOG 1000     	//最大开关柜数量 （一个开关柜一个ARM控制中心）
#define MAXWAITING 4		//最大等待分钟数，超过这个时间，就会认为client连接非正常退出，关闭fd
							//由于是获取的是系统时间，所以这个时间b并不是整时间，例如3分钟，那么就是2-3分钟。
#define ENDTAG "\r\n"		//send分为给与arm的回复，和从web的控制命令。每发送一个send，之后同时send一个结束符。
#define WEBURL "http://112.124.123.169:80/CVoltage/updateCV/"
//#define FILENAME "curlfromweb.log"

/*全局变量区*/

/*
 * 两个线程共用的全局变量
 * 1.struct fd_2_switchgear
 * 2.buff_from_web
 * 3.fd
 *
 */
struct fd_buffer
{
	char buffer[1024*5];
};
struct fd_buffer fd_buffer[BACKLOG];


struct fd_2_switchgear
{
	int s_fd;
	char s_name[128];

};
struct fd_2_switchgear client[BACKLOG];


pthread_mutex_t mutex;

char temp_cmdID[128];      //获取下位机cmdID，并把这个cmdID存入temp_cmdID中，在answer_json_zip中体现，并传回下位机。
char answer_buf[128];
char buff_from_web[512];   //这个buf从web端接受数据，然后又通过tcp发往相应的sockfd。线程安全
//char tmp_buf[1024*5] = "ifuudu u!";
int flags = 0;     //标志位，判断是否把数据从服务器发往ARM
int flags1 = 0;
int fd;


/*
 *
 *
 */
#ifdef DEBUG

FILE *file = NULL;

char tmp_time[1024];

#endif
/*
 *
 *
 */


/*全局变量*/

//查找真正的数据，那些心跳包不存
int find_data(char *buffer);
//找到真正数据后，存入日志
void data_log(int fds,char *buf);

void data_log(int fds,char *buf)
{
	int flags = 0;
	char logname[512];
	char direct[512];
	memset(logname,0,sizeof(logname));
	memset(direct,0,sizeof(direct));
	cJSON *res = NULL,*item = NULL;
	FILE *df = NULL;

	flags = find_data(buf);
	if(flags)
	{

		res = cJSON_Parse(buf);

		item = cJSON_GetObjectItem(res,"SID");
		if(item != NULL)
		{
			strcpy(logname,item->valuestring);
		}


		sprintf(direct,"/alidata/www/wwwroot/switch_log/ArmData[%s].log",logname);
		df = fopen(direct,"at+");
		fprintf(df,">>>[%d] Data Recv: %s\n\n",fds,buf);
		fflush(df);
		fclose(df);
//		df = NULL;
		cJSON_Delete(res);
//		return;
	}
//	cJSON_Delete(res);

}

int find_data(char *buffer)
{
	cJSON *res = NULL,*item = NULL;
	res = cJSON_Parse(buffer);
	if (res == NULL)
	{
		printf("parse err!\n");
		fflush(stdout);
//		buff = NULL;
			return 0;

	}

	item = cJSON_GetObjectItem(res,"SGS");
	if(item != NULL)
	{
		cJSON_Delete(res);
		return 1;
	}

	cJSON_Delete(res);
	return 0;

}


//发送web的同时也发送到服务器上一份

int udp_debug()
{
	int udpsock = 0;
	struct sockaddr_in s;

	udpsock = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&s, 0, sizeof(struct sockaddr_in));
			//设置对方端口信息
	s.sin_family = AF_INET;
	s.sin_port = htons(60000);
	//s.sin_addr.s_addr = inet_addr("192.168.14.110");
	s.sin_addr.s_addr = inet_addr("114.215.119.112");
	connect(udpsock,(struct sockaddr *) &s,sizeof(s));

	return udpsock;

//	return udpsock;

}


//应答json包的封装。
char* answer_json_zip(int n)
{
	char json_buf[1024];
//	json_buf = (char*)malloc(sizeof(char)*128);
//	time_t t;
//	char time_buf[64];
	cJSON * root = NULL;

	memset(json_buf,0,sizeof(json_buf));
	memset(answer_buf,0,sizeof(answer_buf));
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "TP", "Ans");
//	t = time(&t);
//	sprintf(time_buf, "%ld", t);
	cJSON_AddStringToObject(root, "CID", temp_cmdID);
	if(n)
	{
		cJSON_AddStringToObject(root, "RES", "success");
	}else{
		cJSON_AddStringToObject(root, "RES", "fail");
	}

	cJSON_AddStringToObject(root, "RSN", "NULL");

	strcpy(json_buf,cJSON_PrintUnformatted(root));
	memcpy(answer_buf,json_buf,strlen(json_buf));
	answer_buf[strlen(json_buf)] = '\0';

	cJSON_Delete(root);
//	free(json_buf);
//	json_buf = NULL;

	return answer_buf;

}

//从udp解析的json字符串中找到我们所需要的开关柜的ID，再跟据这个ID从fd_2_switchgear中匹配找到对应的s_fd。
//返回fd
int webjson_2_string_2_fd(char* buff)
{
	cJSON *res = NULL,*item = NULL;
	char tmp_name2[1280];
	int n,ret,fd;

	res = cJSON_Parse(buff);
	if (res == NULL)
	{
		printf("parse err!\n");
		fflush(stdout);
//		buff = NULL;
		return 0;

	}
#ifdef DEBUG

	memset(tmp_time,0,sizeof(tmp_time));
	item = cJSON_GetObjectItem(res,"TM");
	if(item != NULL)
	{
		strcpy(tmp_time,item->valuestring);
		printf(">>>>>>>>>>>>>>>tmp_time>>>>>>>>>>>\n");
		fflush(stdout);
	}


#endif



	item = cJSON_GetObjectItem(res,"SID");
	if(item != NULL)
	{
		strcpy(tmp_name2,item->valuestring);
		for(n=0;n<BACKLOG;n++)
		{
			ret = strcasecmp(tmp_name2,client[n].s_name);
			if(ret == 0)
			{
				fd = client[n].s_fd;
				cJSON_Delete(res);
				return fd;
			}
		}
	}


	cJSON_Delete(res);
//	printf("end err\n");
	//fflush(stdout);
//	buff = NULL;
	return 0;
}


//解析json字符串，取出开关柜ID，解析成功返回1，失败返回0；
int armjson_2_string(int ii,int fd,char* buff)
{

	cJSON *res = NULL,*item = NULL;
	char tmp_name[128];
	memset(tmp_name,0,sizeof(tmp_name));
	memset(temp_cmdID,0,sizeof(temp_cmdID));

	res = cJSON_Parse(buff);
	if (res == NULL)
	{
//		buff = NULL;
		return 0;

	}

	item = cJSON_GetObjectItem(res,"SID");
	if(item != NULL)
	{

		strcpy(tmp_name,item->valuestring);

		item = cJSON_GetObjectItem(res,"CID");
		if(item != NULL)
		{
			client[ii].s_fd = fd;
			strcpy(temp_cmdID,item->valuestring);
			strcpy(client[ii].s_name,tmp_name);
		//	sprintf(client[ii].s_name,"%s",item->valuestring);
			printf("s_fd=%d,s_name=%s\n",client[ii].s_fd,client[ii].s_name);
			cJSON_Delete(res);
			return 1;
		}else{
//			buff = NULL;
			cJSON_Delete(res);
			return 0;
		}

	}else{
//		buff = NULL;
		cJSON_Delete(res);
		return 0;
	}

}


/*
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	FILE *fptr = (FILE*)userp;
	fwrite(buffer, size, nmemb, fptr);
}
*/


void post_2_web(char* buff)
{
	//TO-DO
	CURL *curl = NULL;
	CURLcode res;
//	FILE *fptr;
//	struct curl_slist *http_header = NULL;

//	if ((fptr = fopen(FILENAME, "w")) == NULL) {
//		fprintf(stderr, "fopen file error: %s\n", FILENAME);
//		exit(1);
//	}
	char new_buff[1024*5];

	memset(new_buff,0,sizeof(new_buff));
	sprintf(new_buff,"%s",buff);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, WEBURL);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, new_buff);
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
//	curl_easy_setopt(curl, CURLOPT_HEADER, 1);
//	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
//	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/usr/cookie.log");
	res = curl_easy_perform(curl);
	printf(".....%d .often warning about res.....\n",res);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

}


void udp_with_web()
{
    struct sockaddr_in s_addr;
    struct sockaddr_in c_addr;
    int sock;
    socklen_t addr_len;
    int len;
    char buff[1024*5];

//    char buf[128];
	char time_buffer[256];
	char dir_name[1024];
	struct tm *t;
	time_t timer;
	char cmd_buffer[1024];
//--------

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");

    }
    else
        printf("create socket succeed!!!\n");

    memset(&s_addr, 0, sizeof(struct sockaddr_in));


    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(UDPPORT);
    s_addr.sin_addr.s_addr = INADDR_ANY;


    if ((bind(sock, (struct sockaddr *) &s_addr, sizeof(s_addr))) == -1) {
        perror("bind");

    }
    else
    {
        printf("bind address to socket succeed.\n");
        printf("please launch the client of udp to continue...\n");
    }


    addr_len = sizeof(c_addr);

 //   int i;
//    int n;
    while(1)
    {

    	len = recvfrom(sock, buff, sizeof(buff) - 1, 0,(struct sockaddr *) &c_addr, &addr_len);


        if (len < 0)
        {
            printf("receive data from web error!\n");

        }

        else
        {
        	buff[len] = '\0';

            printf("UDP:received %s:%d     message:  %s\n",inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port), buff);
            printf("> * strlen = %d\n",strlen(buff));

//            fprintf(file,"")
//            strcpy(buf,"OKAY");
//            n=sendto(sock,buf,sizeof(buf),0,(struct sockaddr *)&c_addr,addr_len);
//            if(n<0)
//            {
//                 perror("sendto");
//                 continue;
//            }

        	time(&timer);
        	t= localtime(&timer);
        //	printf( "%04d-%02d-%02d %02d:%02d:%02d\n", t->tm_year + 1900,
        //		t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);


        	memset(time_buffer,0,sizeof(time_buffer));
        	memset(dir_name,0,sizeof(dir_name));
        	sprintf(time_buffer,"%04d-%02d-%02d", t->tm_year + 1900,t->tm_mon + 1, t->tm_mday);
        	sprintf(dir_name,"/alidata/www/wwwroot/switch_log/ServerLog[%s].log",time_buffer);
        	printf("%s\n",dir_name);







        }
//        pthread_mutex_lock(&mutex);

        file = fopen(dir_name,"at+");
        strcpy(buff_from_web,buff);
        /*
         * 调用webjson_2_string_2_fd()解析。
         */
        fd = webjson_2_string_2_fd(buff);
        if(fd == 0)
        {
        	printf("web json zip parse error!\n");
#ifdef DEBUG
        	fprintf(file,">>***Time :[%s] Recv Buffer From Web:%s\n",tmp_time,buff);
        	fflush(file);
        	fprintf(file,">>   Time :[%s] Web Json Zip Parse Error\n",tmp_time);
        	fflush(file);
        	fclose(file);
        	file = NULL;
#endif
        }
        else{
        	printf("web json zip parse success!!!!\n");
#ifdef DEBUG
        	fprintf(file,">>***Time :[%s] Recv Buffer From Web:%s\n",tmp_time,buff);
        	fflush(file);
        	fprintf(file,">>   Time :[%s] Web Json Zip Parse Success\n",tmp_time);
        	fflush(file);
#endif
 //           pthread_mutex_lock(&mutex);

            memset(cmd_buffer,0,sizeof(cmd_buffer));
            sprintf(cmd_buffer,"%s%s",buff_from_web,ENDTAG);

            if(send(fd,cmd_buffer,strlen(cmd_buffer),0) < 0)
            {
#ifdef DEBUG
            	printf("[>>>]System Send ERR [>>>] Sending Sencond Time[>>>]\n");

                fprintf(file,">>＆  Time :[%s][%d] [>>>]System Send ERR [>>>] Sending Sencond Time[>>>]\n",tmp_time,fd);
                fflush(file);
#endif
            	if(send(fd,cmd_buffer,strlen(cmd_buffer),0) < 0)
                {
                	printf("send control cmd error\n"	);
#ifdef DEBUG
                	fprintf(file,">>   Time :[%s][%d] Send Control CMD Failed\n",tmp_time,fd);
                	fflush(file);
#endif
                }
                else{
    //            	send(fd,ENDTAG,strlen(ENDTAG),0);
                	printf("send[2] control cmd to ARM success!\n");
#ifdef DEBUG
                	fprintf(file,">>   Time :[%s][%d] Send[2]Control CMD Success\n",tmp_time,fd);
                	fflush(file);
                	printf("************************************************************\n");
#endif
                }
            }
            else{
//            	send(fd,ENDTAG,strlen(ENDTAG),0);
            	printf("[%d]send control cmd to ARM success!\n",fd);
            	fflush(stdout);
#ifdef DEBUG
            	fprintf(file,">>   Time :[%s][%d] Send Control CMD Success\n",tmp_time,fd);
            	fflush(file);
            	printf("************************************************************\n");
#endif
            }

            fclose(file);
            file = NULL;
//           	flags1 = 0;
//            pthread_mutex_unlock(&mutex);
        }

        printf("%s\n",buff_from_web);
//        pthread_mutex_unlock(&mutex);


        usleep(10000);
    }
    close(sock);
}



void tcp_with_arm()
{
    int fd_A[BACKLOG] = {0},n;
	struct sockaddr_in server;
	struct sockaddr_in client_addr;
	int sock_fd,new_fd;
	int ret,conn_amount,maxsock;
	fd_set fdsr;
	socklen_t sin_size;
	char buf[1024*5];
	struct timeval timeout;
	int i,ii;
	char tmp_buff[1024*5];
	char parameter_buff[1024*5];
	char answer_buff[128];
	char new_buff[256];

//	char cmd_buffer[1024];
	char log_buffer[1024];

	int udpsock = 0;
	int fds;

//	char temptest[1025*5] = "={    'Type':'upload',    'cmdID':'123',    'Datetime':'2014-4-16 20:34:20',    'DepartmentID':'s006',    'DepartmentName':'station6',    'SwitchgearID':'c007',    'Switchgears':[        { 'SwitchgearID':'c007', 'Ua':'40',  'Ub':'11.1', 'Uc':'12.1',  'Ia':'39', 'Ib':'21.1', 'Ic':'22.1',  'PFa':'30', 'PFb':'31.1',  'PFc':'32.2', },  {  'SwitchgearID':'c007',  'BreakerPos':0,  'HandPos':0,  'TestPos':1, 'GroundKnife':0,  'Energy':1, 'Switch7':1, 'EA':1,  'EB':0,  'EC':0,   'Lock':1,   'Sensor1':1,   'Sensor2':0,  'Load1':1,   'Load2':0,  'Loop1':1,  'Loop2':0,  }  ]}";

	time_t t;
	struct tm *tt;
	int last_min;
	int flag_minute = 0;
	int con_time[BACKLOG] ;

	/*创建socket*/
	if((sock_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket!");
		exit(1);
	}else{
		printf("create socket succeed\n");
	}

	/*设置端口信息*/
	bzero(&server,sizeof(struct sockaddr_in));
	server.sin_family=AF_INET;

	server.sin_port = htons(SERVERPORT);

	server.sin_addr.s_addr= INADDR_ANY;


	/*绑定本地端口等信息*/
	if((bind(sock_fd,(struct sockaddr*) &server,sizeof(server)))==-1)
	{
		perror("bind!");
		exit(1);
	}else{
		printf("bind succeed!\n");
	}


	/*监听套接字*/
	if(listen(sock_fd,BACKLOG)==-1){
		perror("listen");
		exit(1);
	}else{
		printf("listen succeed!\n");
	}

	sin_size=sizeof(struct sockaddr_in);

	conn_amount=0;
	maxsock=sock_fd;
	time(&t);
	tt = localtime(&t);
	last_min = tt->tm_min;
//	printf("------1-------%s\n",buff_from_web);

	//循环接收发送数据，处理过程，也是整个通信的核心。
    while (1)
    {
    	time(&t);
    	tt = localtime(&t);
    	if(tt->tm_min != last_min)
    	{
    		printf("no ~!\n");
    		last_min = tt->tm_min;
    		flag_minute = 1;
    	}

    	//初始化描述服集
        FD_ZERO(&fdsr);
        FD_SET(sock_fd, &fdsr);
        // 阻塞时间设置为1妙
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000000;//一秒 = 1000 毫秒 = 一百万微秒
        // 设置一个sock_fd的数组，这个数组存放socket描述符。这里非常关键
        //fd这个数组在以后的处理过程中就相当于处理那些具体的数据，他跟新的描述符关联。
        for (i = 0; i < BACKLOG; i++)
        {
//        	printf("for backlog\n");
            if (fd_A[i] != 0)
            {
//            	printf("for backlog if --i=%d--fd_[i]=%d\n",i,fd_A[i]);
                FD_SET(fd_A[i], &fdsr);
                if(flag_minute == 1)
                {
                	printf("goto  timeout progress\n");
                	con_time[i]--;
                	if(con_time[i] <= 0)
                	{
        				printf("client[%d] maybe not ok,close this fd\n", i);
						close(fd_A[i]);
						FD_CLR(fd_A[i], &fdsr);
						fd_A[i] = 0;
						conn_amount--;
						//TO-DO memset struct
			            pthread_mutex_lock(&mutex);
						memset(&client[i],0,sizeof(struct fd_2_switchgear));
			            pthread_mutex_unlock(&mutex);
						memset(fd_buffer[i].buffer, 0 ,sizeof(fd_buffer[i].buffer));

                	}
                }
            }
        }

//        printf("maxsock %d\n",maxsock);

        /*
         *
         *
         */

//        post_2_web(temptest);
//        if(flags1)
//        {
//
//
//
//        }


        flag_minute = 0;
        ret = select(maxsock + 1, &fdsr, NULL, NULL, &timeout);
        printf("select over\n");
 //       printf("ret %d\n",ret);

        if (ret < 0)
        {
            perror("select");
//            break;
        } else if (ret == 0)
        {
//            printf("Waiting for client connecting...\n");
            continue;
        }

        // 检查每一个fd_A[i]描述符是否为真，如果为真，则接受buf中的数据并予以显示。
        //这里的i<BACKLOG,暂时还没有好的办法，之前用的是conn_amount，
        //又由于conn_amount动态变化，这样有可能还在连接fd[i]中的i比conn_amount大，导致不能进入下边的if，从而
        //使得，select能检查到，但是下边的isset不能检测到。但是这样的话导致此for循环次数变得很大，作了很多无用功。
        for (i = 0; i < BACKLOG; i++)
        {
//        	printf("for conn_amount,i=%d,conn_amount=%d\n",i,conn_amount);
//        	client[i].start = time(NULL);
			if (FD_ISSET(fd_A[i], &fdsr))
			{
				memset(buf,0,sizeof(buf));
				ret = recv(fd_A[i], buf, sizeof(buf),0);
//				printf("recv over\n");
				if (ret <= 0)
				{
					//没有受到任何数据，则显示客户端关闭。
					printf("client[%d] close\n", i);
					close(fd_A[i]);
					FD_CLR(fd_A[i], &fdsr);
					fd_A[i] = 0;
					conn_amount--;
					//TO-DO memset struct
		            pthread_mutex_lock(&mutex);
					memset(&client[i],0,sizeof(struct fd_2_switchgear));
		            pthread_mutex_unlock(&mutex);
					memset(fd_buffer[i].buffer, 0 ,sizeof(fd_buffer[i].buffer));


				} else
				{   //接受数据并显示数据
					if (ret < 1024*5)
						buf[ret]='\0';
					printf("client[%d] send:%s\n", i, buf);
					printf("--------------------------------------\n");
					con_time[i] = MAXWAITING;

					/******test*******/
					//根据buf的内容按照定义好的字符串分隔符进行字符串分割，使得分割后的字符串存入tmp_buff中
					//并调用json解析函数，解析json，目的为把当前的fd与解析后的json包中开关柜ID匹配。armjson_2_string(i,fd_A[i],buf);


					strcat(fd_buffer[i].buffer,buf);
					printf("@@@@@fd_buffer = %s,strlen=%d\n",fd_buffer[i].buffer,strlen(fd_buffer[i].buffer));
					for(ii=0;ii<strlen(fd_buffer[i].buffer);ii++)
					{
						if(strlen(fd_buffer[i].buffer) > 1500)	{
							memset(fd_buffer[i].buffer, 0 ,sizeof(fd_buffer[i].buffer));
							break;
						}



						if((fd_buffer[i].buffer[ii] == '\r')&&(fd_buffer[i].buffer[ii+1] == '\n'))
						{
							memset(tmp_buff,0,sizeof(tmp_buff));//关键所在！！！！
							strncpy(tmp_buff,&fd_buffer[i].buffer[0],ii+2);
							printf("^^^^^^^tmp_buff = %s,sizeof=%d,strlen=%d^^^^^^^\n",tmp_buff,sizeof(tmp_buff),strlen(tmp_buff));
							memset(parameter_buff,0,sizeof(parameter_buff));
							strcpy(parameter_buff,tmp_buff);
							/*
							 *
							 * TO-DO
							 * 1.调用POST使得tmp_buff发送至web
							 * 2.调用json_2_string（）进行数据的解析
							 *
							 */


							pthread_mutex_lock(&mutex);

							if(armjson_2_string(i,fd_A[i],parameter_buff))
							{
								printf("Analysis Success!\n");
								//调用一个json函数，该函数实现 成功 应答信息的封装。返回一个字符串
								//TO-DO
								memset(answer_buff,0,sizeof(answer_buff));
								memset(new_buff,0,sizeof(new_buff));
								strcpy(answer_buff,answer_json_zip(1));
								//send() 给相应的fd
								sprintf(new_buff,"%s%s",answer_buff,ENDTAG);
								if(send(fd_A[i],new_buff,strlen(new_buff),0)<0)
									printf("send answer json failed");
								else{
								//	send(fd_A[i],ENDTAG,strlen(ENDTAG),0);
									printf("send ok-answer json success!\n");
						            printf("> * answer buffer length = %d\n",strlen(answer_buff));

								}
								//调用http post
#ifdef DEBUG_TEST
								fds = fd_A[i];
								memset(log_buffer,0,sizeof(log_buffer));
								strcpy(log_buffer,tmp_buff);
								data_log(fds,log_buffer);

								//调试信息
								if(udpsock == 0)
									udpsock = udp_debug();

								send(udpsock, tmp_buff, strlen(tmp_buff), 0);
#endif
								post_2_web(tmp_buff);
							}else{
								printf("Analysis Failed!\n");
								//调用一个json函数，改函数实现 失败 应答信息的封装。返回一个字符串
								//TO-DO
								memset(answer_buff,0,sizeof(answer_buff));
								memset(new_buff,0,sizeof(new_buff));
								strcpy(answer_buff,answer_json_zip(0));
								sprintf(new_buff,"%s%s",answer_buff,ENDTAG);

								//send() 给相应的fd
								if(send(fd_A[i],new_buff,strlen(new_buff),0)<0)
									printf("send answer json failed");
								else{
						//			send(fd_A[i],ENDTAG,strlen(ENDTAG),0);
									printf("send err-answer json success!\n");
						            printf("> * answer buffer length = %d\n",strlen(answer_buff));

								}
							}
							pthread_mutex_unlock(&mutex);


							/****************************************/

							memmove(fd_buffer[i].buffer,&fd_buffer[i].buffer[ii+2], \
									(strlen(fd_buffer[i].buffer)-strlen(tmp_buff)));
							printf("often err\n");
							fd_buffer[i].buffer 	\
							[(strlen(fd_buffer[i].buffer)-strlen(tmp_buff))] = '\0';
							printf("*****new_fd_buffer = %s********\n",fd_buffer[i].buffer);
//							turn = 1;
							ii = 0; //关键所在！！！！！

						}
					}

				}
			}

        }
   //
        printf("**********check new-connection***********\n");

        // 查看是否有新的客户端来连接（即老的套接字sock_fd)
        if (FD_ISSET(sock_fd, &fdsr))
        {
//        	printf("fd_isset \n");
            new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size);

//            printf("new_fd %d\n",new_fd);

            if (new_fd <= 0)
            {
                perror("accept");
                continue;
            }
            // 如果conn_amount还能连接，则继续把新的连接加入到fd_A[i]数组中。
            if (conn_amount < BACKLOG)
            {
            	for(i = 0;i < BACKLOG;i++)
            	{
            		if(fd_A[i] == 0)
            		{
            			fd_A[i] = new_fd;
            			con_time[i] = MAXWAITING; //每次新建立连接，设置该连接的超时数，如果此连接此后MAXWAITING秒内无反应，关闭该连接
            			break;
            		}
            	}
            	conn_amount++;
                printf("new connection client[%d] %s:%d\n", conn_amount-1,
                        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                if (new_fd > maxsock)
                    maxsock = new_fd;
            }
            else
            {
                printf("max connections arrive, exit\n");
                close(new_fd);
                continue;
            }
        }
    }

    //释放资源
    for (n = 0; n < BACKLOG;n++)
    {
    	if (fd_A[n] != 0)
    	{
    		close(fd_A[n]);
    	}
    }
	close(sock_fd);

}



int main(int argc,char *argv[])
{
//	get_head_thread();
	pthread_t udp;
	int ret3;

	int value;
	for(value = 0; value < BACKLOG; value++)
	{
		memset(fd_buffer[value].buffer, 0 ,sizeof(fd_buffer[value].buffer));
	}

	pthread_mutex_init(&mutex,NULL);
	ret3 = pthread_create(&udp,NULL,(void*)udp_with_web,NULL);
	if(ret3 != 0)
		printf("create pthread success!\n");


	tcp_with_arm();

	pthread_mutex_destroy(&mutex);

//	fclose(file);
	return 0;
}

