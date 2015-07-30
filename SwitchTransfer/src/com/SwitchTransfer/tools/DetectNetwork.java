package com.SwitchTransfer.tools;


import com.SwitchTransfer.DealWithArm.SocketThread;


/**
 * 实现对现有连接的网络状态检测，如一个连接在指定时间没有发送数据到服务器
 * 那么就断开连接，关闭socket
 * @author 
 *
 */
public class DetectNetwork extends Thread {

	public void run(){
		while(true){

			for( SocketThread socketThread : GlobalValue.socketThreads){
				if (socketThread.getTimer() >= 3){
					socketThread.interrupt();
				}else{	
					socketThread.setTimer(socketThread.getTimer()+1);
				}
			}					
			try {
				Thread.sleep(60000);
			} catch (InterruptedException e) {

				e.printStackTrace();
			}


		}


	}
}
