package com.SwitchTransfer.DealWithWeb;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.SwitchTransfer.tools.GlobalValue;
import com.SwitchTransfer.tools.SwitchBean;
import com.SwitchTransfer.tools.SwitchServerLog;
import com.SwitchTransfer.tools.doJsonWeb;

public class UdpWithWeb extends Thread{



	public void run() {

		while(true){
			DatagramSocket uSocket = null;
			try {
				uSocket = new DatagramSocket(GlobalValue.UDPPORT);
				byte[] buf = new byte[GlobalValue.BUFFERSIZE];  //定义byte数组
				DatagramPacket packet = new DatagramPacket(buf, buf.length);  //创建DatagramPacket对象

				uSocket.receive(packet);  //通过套接字接收数据

				String getMsg = new String(buf, 0, packet.getLength());
				System.out.println(">>> Recevie Control cmd from Web：\n" +"   " +getMsg+"\n");
				//日志

				String dir = new String(GlobalValue.MAINPATH+"controlData/");
				SwitchServerLog.writebuffer(dir, dir+"Server["+new SimpleDateFormat("yyyy-MM-dd")
				.format(new Date())+"].log", getMsg);
				//TO-DO
				//发送控制指令给相应节点
				doJsonWeb djw = new doJsonWeb();
				if (!djw.decodeWebJson(getMsg)){
					SwitchServerLog.writebuffer(dir, dir+"Server["+new SimpleDateFormat("yyyy-MM-dd")
					.format(new Date())+"].log", "解析JSON失败，未发送给开关柜！");
				}else{
					SwitchBean sb = new SwitchBean(djw.getS_name(), djw.getD_name());
					System.out.println("^^^ 柜子："+djw.getS_name()+"   ^^^ 配电所： "+djw.getD_name());
					Socket socket = GlobalValue.shm.findObjectInHashmap(sb);
					System.out.println(">>> *"+socket);
					StringBuffer str = new StringBuffer(getMsg);
					str.append(GlobalValue.ENDTAG);
					OutputStreamWriter write = new OutputStreamWriter(new BufferedOutputStream(socket.getOutputStream()));
					write.write(str.toString());
					write.flush();
					SwitchServerLog.writebuffer(dir, dir+"Server["+new SimpleDateFormat("yyyy-MM-dd")
					.format(new Date())+"].log", "解析成功，并成功发送给开关柜!");

				}
			}catch (UnknownHostException e) {
				e.printStackTrace();
				SwitchServerLog.writeUnknownException(e.getMessage()+"\t[udpwithweb\tUnknownHostException] {"
													+new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())+" }");
			} catch(IOException e) {
				e.printStackTrace();
				SwitchServerLog.writeUnknownException(e.getMessage()+"\t[udpwithweb\tIOException] {"
													+new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())+" }");
			}catch(Exception e){
				e.printStackTrace();
				SwitchServerLog.writeUnknownException(e.getMessage()+"\t[udpwithweb\tException] {"
													+new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())+" }");
			}finally{
				if (uSocket!= null){
					uSocket.close();
				}
			}
		}

	}
}
