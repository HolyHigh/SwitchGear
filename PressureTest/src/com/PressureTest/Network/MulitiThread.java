package com.PressureTest.Network;

import java.awt.List;

import net.sf.json.JSONObject;

import com.PressureTest.JsonTest.JsonTools;
import com.PressureTest.JsonTest.UpDetails;

public class MulitiThread {

	
	public static void main(String[] args) {
	
		for(int ii=1; ii < 3; ii++){
			if(ii == 11 || ii == 9) continue;
			Thread t = new MyThread(ii+1);
			t.start();
		}
		

	}

}


class MyThread extends Thread
{
	private int value;
	
	public void Tcpfunc(String value)
	{
		TcpClient tcpClient = new TcpClient();
		tcpClient.createClient(value);
	}

	public MyThread(int value)
	{
		this.value = value;
	}

	
	public void run(){
		
		StringBuffer sb = new StringBuffer("C");
		String s = String.valueOf(value);
		
		if(value>0 && value<10){		
			sb.append("00");
			sb.append(s);
		}else if(value >= 10 && value <100){
			sb.append("0");
			sb.append(s);
		}else{
			sb.append(s);
		}
			
		Tcpfunc(sb.toString());
//		System.out.println(sb.toString());
			//System.out.println("this is Thread ["+value+"] running ...");
//		}

	}
	
}