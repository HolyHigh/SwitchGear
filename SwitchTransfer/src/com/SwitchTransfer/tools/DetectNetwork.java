package com.SwitchTransfer.tools;


import com.SwitchTransfer.DealWithArm.SocketThread;


/**
 * ʵ�ֶ��������ӵ�����״̬��⣬��һ��������ָ��ʱ��û�з������ݵ�������
 * ��ô�ͶϿ����ӣ��ر�socket
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
