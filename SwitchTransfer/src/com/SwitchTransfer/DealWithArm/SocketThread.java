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
			socket.setSoTimeout(GlobalValue.SERVERTIMEOUT); 	//超时检测，如果指定时间未发数据，则readline抛出异常，关闭链接
			BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			Writer writer = new OutputStreamWriter(socket.getOutputStream());
			BufferedWriter bw = new BufferedWriter(writer);
			
			while(true){
				System.out.println(">>> Thread TcpWithArm is working ["+socket+"]");

				
				String content;
				//整行从系统缓冲区中取出数据，这个readLine()读出的就是以\r\n结尾，所以省去了去分析数据包的工作
				//发送给web时，要加上\r\n才可以
				//此处的流程是1.分析json包，并把json放入arrayslist。2.发送响应信息给下位机。3.做一个日志存储

				while ((content = reader.readLine()) != null) {
					timer = 0;				
					
					System.out.println(">*> ReceiveJson : " + content+"\n");
					
					//分析包
					doJsonArm dja = new doJsonArm();
					boolean flags = dja.analysisJsonbuffer(content, socket);
					String answerbuffer = null;

					//响应信息
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
				System.out.println("\n[异常] ：下位机主动断开连接（柜号：［"+s_name+"］，配电所：［"+d_name+"］），此时服务线程准备关闭\n");
				//日志
				String dir = new String
						(GlobalValue.MAINPATH+"exception/"+new SimpleDateFormat("MM-dd").format(new Date()));
				SwitchServerLog.writebuffer(dir, dir+"/cutdown.log",
						"[异常] ：{"+new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())+
						"}下位机主动断开连接（柜号：［"+s_name+"］，配电所：［"+d_name+"］），此时服务线程准备关闭");
				break;
				 
			}

		}catch (SocketTimeoutException e){		
			System.out.println("\n [超时] ：与开关柜［"+s_name+"］，配电所［"+d_name+"］连接等待超时，正在关闭socket，同时退出当前线程\n");			
			String dir = new String
					(GlobalValue.MAINPATH+"exception/"+new SimpleDateFormat("MM-dd").format(new Date()));
			SwitchServerLog.writebuffer(dir, dir+"/timeout.log", 
					"[超时] ：{"+new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())+
					"}与开关柜［"+s_name+"］，配电所［"+d_name+"］连接等待超时，正在关闭socket，同时退出当前线程");
			//日志
		}catch (IOException e) {
			e.printStackTrace();
			//日志
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