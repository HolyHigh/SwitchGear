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
				byte[] buf = new byte[GlobalValue.BUFFERSIZE];  //����byte����
				DatagramPacket packet = new DatagramPacket(buf, buf.length);  //����DatagramPacket����

				uSocket.receive(packet);  //ͨ���׽��ֽ�������

				String getMsg = new String(buf, 0, packet.getLength());
				System.out.println(">>> Recevie Control cmd from Web��\n" +"   " +getMsg+"\n");
				//��־

				String dir = new String(GlobalValue.MAINPATH+"controlData/");
				SwitchServerLog.writebuffer(dir, dir+"Server["+new SimpleDateFormat("yyyy-MM-dd")
				.format(new Date())+"].log", getMsg);
				//TO-DO
				//���Ϳ���ָ�����Ӧ�ڵ�
				doJsonWeb djw = new doJsonWeb();
				if (!djw.decodeWebJson(getMsg)){
					SwitchServerLog.writebuffer(dir, dir+"Server["+new SimpleDateFormat("yyyy-MM-dd")
					.format(new Date())+"].log", "����JSONʧ�ܣ�δ���͸����ع�");
				}else{
					SwitchBean sb = new SwitchBean(djw.getS_name(), djw.getD_name());
					System.out.println("^^^ ���ӣ�"+djw.getS_name()+"   ^^^ ������� "+djw.getD_name());
					Socket socket = GlobalValue.shm.findObjectInHashmap(sb);
					System.out.println(">>> *"+socket);
					StringBuffer str = new StringBuffer(getMsg);
					str.append(GlobalValue.ENDTAG);
					OutputStreamWriter write = new OutputStreamWriter(new BufferedOutputStream(socket.getOutputStream()));
					write.write(str.toString());
					write.flush();
					SwitchServerLog.writebuffer(dir, dir+"Server["+new SimpleDateFormat("yyyy-MM-dd")
					.format(new Date())+"].log", "�����ɹ������ɹ����͸����ع�!");

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
