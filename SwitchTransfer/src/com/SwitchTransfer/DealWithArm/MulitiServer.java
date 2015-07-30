package com.SwitchTransfer.DealWithArm;

import java.net.ServerSocket;
import java.net.Socket;

import com.SwitchTransfer.tools.GlobalValue;


public class MulitiServer extends Thread{
	private ServerSocket serverSocket;
	private Socket socket;
 	
	
	public void run() {
		try {
			serverSocket = new ServerSocket(GlobalValue.SERVERPORT);
		} catch (Exception e) {
			e.printStackTrace();
		}
		while (true) {
			System.out.println(">>> Waiting for a Client ");
			System.out.flush();
			try {
				socket = serverSocket.accept();
			} catch (Exception e) {
				e.printStackTrace();
			}
			SocketThread s = new SocketThread(socket);
			s.start();
//			GlobalValue.socketThreads.add(s);
		}
	}
}


