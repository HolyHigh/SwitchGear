package com.SwitchTransfer.DealWithArm;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.text.SimpleDateFormat;
import java.util.Date;

import com.SwitchTransfer.tools.GlobalValue;
import com.SwitchTransfer.tools.JsonAnswerBean;
import com.SwitchTransfer.tools.SwitchServerLog;
import com.SwitchTransfer.tools.doJsonAnswer;
import com.SwitchTransfer.tools.doJsonArm;

public class SocketThread extends Thread
{
	
	public SocketThread(Socket socket){
		this.socket = socket;
	}
	
	public int getTimer() {
		return timer;
	}

	public void setTimer(int timer) {
		this.timer = timer;
	}



	public String getS_name() {
		return s_name;
	}

	public void setS_name(String s_name) {
		this.s_name = s_name;
	}

	public String getD_name() {
		return d_name;
	}

	public void setD_name(String d_name) {
		this.d_name = d_name;
	}



	private String s_name;
	private String d_name;
	private int timer;
	private Socket socket;



	public void run(){		
		try{
			socket.setSoTimeout(GlobalValue.SERVERTIMEOUT); 	//��ʱ��⣬���ָ��ʱ��δ�����ݣ���readline�׳��쳣���ر�����
			BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			Writer writer = new OutputStreamWriter(socket.getOutputStream());
			BufferedWriter bw = new BufferedWriter(writer);
			
			while(true){
				System.out.println(">>> Thread TcpWithArm is working ["+socket+"]");

				
				String content;
				//���д�ϵͳ��������ȡ�����ݣ����readLine()�����ľ�����\r\n��β������ʡȥ��ȥ�������ݰ��Ĺ���
				//���͸�webʱ��Ҫ����\r\n�ſ���
				//�˴���������1.����json��������json����arrayslist��2.������Ӧ��Ϣ����λ����3.��һ����־�洢

				while ((content = reader.readLine()) != null) {
					timer = 0;				
					
					System.out.println(">*> ReceiveJson : " + content+"\n");
					
					//������
					doJsonArm dja = new doJsonArm();
					boolean flags = dja.analysisJsonbuffer(content, socket);
					String answerbuffer = null;

					//��Ӧ��Ϣ
					if (flags){
						doJsonAnswer answer = new doJsonAnswer();
						JsonAnswerBean jab = answer.defineSetAnswer(0, dja.getCID());
						answerbuffer = answer.getJsonAnswer(jab);
						this.s_name = dja.getS_name();
						this.d_name = dja.getD_name();

					}else{
						doJsonAnswer answer = new doJsonAnswer();
						JsonAnswerBean jab = answer.defineSetAnswer(1, dja.getCID());
						answerbuffer = answer.getJsonAnswer(jab);			
					}

					StringBuffer ab = new StringBuffer(answerbuffer	);
					bw.write(ab.append(GlobalValue.ENDTAG).toString());
					bw.flush();
					System.out.println(">>> send answer finished: "+ab.append(GlobalValue.ENDTAG).toString());
				}
				System.out.println("\n[�쳣] ����λ�������Ͽ����ӣ���ţ���"+s_name+"�ݣ����������"+d_name+"�ݣ�����ʱ�����߳�׼���ر�\n");
				//��־
				String dir = new String
						(GlobalValue.MAINPATH+"exception/"+new SimpleDateFormat("MM-dd").format(new Date()));
				SwitchServerLog.writebuffer(dir, dir+"/cutdown.log",
						"[�쳣] ��{"+new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())+
						"}��λ�������Ͽ����ӣ���ţ���"+s_name+"�ݣ����������"+d_name+"�ݣ�����ʱ�����߳�׼���ر�");
				break;
				 
			}

		}catch (SocketTimeoutException e){		
			System.out.println("\n [��ʱ] ���뿪�ع��"+s_name+"�ݣ��������"+d_name+"�����ӵȴ���ʱ�����ڹر�socket��ͬʱ�˳���ǰ�߳�\n");			
			String dir = new String
					(GlobalValue.MAINPATH+"exception/"+new SimpleDateFormat("MM-dd").format(new Date()));
			SwitchServerLog.writebuffer(dir, dir+"/timeout.log", 
					"[��ʱ] ��{"+new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())+
					"}�뿪�ع��"+s_name+"�ݣ��������"+d_name+"�����ӵȴ���ʱ�����ڹر�socket��ͬʱ�˳���ǰ�߳�");
			//��־
		}catch (IOException e) {
			e.printStackTrace();
			//��־
		}finally{
			if (socket != null) {
				if (!socket.isClosed()) {
					try {
						socket.close();
						System.out.println("\n>>> Close socket success , and this thread is over \n");
//						GlobalValue.socketThreads.remove(this);
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}

	}

}