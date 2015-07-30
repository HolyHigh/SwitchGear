package com.SwitchTransfer.tools;

import java.util.ArrayList;

import com.SwitchTransfer.DealWithArm.SocketThread;
import com.SwitchTransfer.DealWithWeb.PostArrayList;

public class GlobalValue {
	
	public static ArrayList<SocketThread> socketThreads = new ArrayList<>();
	public static PostArrayList pal = new PostArrayList();
	public static SwitchHashMap shm = new SwitchHashMap();
	public static final String WEBURL = "http://112.124.123.169:8066/CVoltage/updateCV/";
	public static final int MAXBUFFERSIZE = 1024*5;
	public static final int BUFFERSIZE = 1024;
	public static final String ENDTAG = "\r\n";
	public static final String SERVERIP = "112.124.123.169";
	public static final int	SERVERPORT = 55555;
	public static final int UDPPORT = 60000;
	public static final int SERVERTIMEOUT = 240000;
	public static final String MAINPATH = "/home/dfxServer/log/";

}
