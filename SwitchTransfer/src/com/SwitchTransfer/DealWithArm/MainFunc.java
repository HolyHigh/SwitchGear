
package com.SwitchTransfer.DealWithArm;



//import java.io.File;
//import java.io.IOException;
//import java.text.SimpleDateFormat;
//import java.util.Date;

import java.text.SimpleDateFormat;
import java.util.Date;

import com.SwitchTransfer.DealWithWeb.Post2Web;
import com.SwitchTransfer.DealWithWeb.UdpWithWeb;
import com.SwitchTransfer.tools.DetectNetwork;
import com.SwitchTransfer.tools.SwitchServerLog;


/**
 * 
 *
 * @author Song Zhicheng
 * @date 2014年8月20日  @time 下午7:59:22
 * @function :
 *
 */
public class MainFunc {

	public static void main(String[] args) {

		System.out.println(">>> Program is starting ");

		new MulitiServer().start();	
		new Post2Web().start();
		new UdpWithWeb().start();
//		new DetectNetwork().start();
		//		String dir = new String(GlobalValue.MAINPATH+"armdata/"+new SimpleDateFormat("MM-dd").format(new Date()));
		//		for(int i = 0; i < 100 ; i++){
		//			SwitchServerLog.writebuffer("/home/sky", "/home/sky/abc.txt", "hello,world"+i);
		//		}
		while(true){
			SwitchServerLog.testwrite("ThreadCount is : "+Thread.activeCount());
			
			try {
				Thread.sleep(400000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
