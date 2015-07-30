package com.SwitchTransfer.DealWithWeb;


import com.SwitchTransfer.tools.GlobalValue;
import com.SwitchTransfer.tools.HttpFunc;
import com.SwitchTransfer.tools.SwitchServerLog;

public class Post2Web extends Thread{
	
	int i = 0;
	public void run()	{
		while(true){

			if ( GlobalValue.pal.size() < 300 ){
				try {
					Thread.sleep(1);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}

			if (GlobalValue.pal.isPostArrayListEmpty()){
				continue;
			}else{
				i++;
				System.out.println(">>> Thread Post2web is Working ");
				String sendbuffer = GlobalValue.pal.getPostArrayList(0);
				String str = HttpFunc.sendPost(GlobalValue.WEBURL, sendbuffer);
				System.out.println("^^> HttpServer's Action about post : "+str+ "  \n");
				System.out.println(">>> Post finished\n");
				if( i > 300 ){
					SwitchServerLog.writeUnknownException("this arraylist size is: "+GlobalValue.pal.size());
					i = 0;
				}
				GlobalValue.pal.rmPostArrayList(0);
			}
				
		}
	}
}
