package com.PressureTest.Network;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;  
import java.io.DataInputStream;
import java.io.DataOutputStream;  
import java.io.IOException;
import java.io.InputStream;  
import java.io.InputStreamReader;  
import java.io.OutputStream;  
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.net.InetAddress;  
import java.net.Socket;  
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.Arrays;

import com.PressureTest.JsonTest.JsonTools;

class TcpClient {

	public	TcpClient()
	{

	}

	public void	createClient(String value)
	{
		while(true){
			try {
				Socket client = new Socket(Define.SERVERIP, Define.SERVERPORT);
				client.setSoTimeout(10000);
				OutputStreamWriter write = new OutputStreamWriter(new BufferedOutputStream(client.getOutputStream()));

				InputStream is = client.getInputStream();
				InputStreamReader read = new InputStreamReader(new BufferedInputStream(is));

				boolean flags = true;
				while(flags)
				{	


					JsonTools jsonTools = new JsonTools();
					StringBuffer str = new StringBuffer( jsonTools.createJsonZip(value));
					String sendbuf = str.append("\r\n").toString();
					write.write(sendbuf);
					write.flush();
					if("bye".equals(sendbuf))
					{
						flags = false;
					}else{					

						try {

							char [ ] cbuf = new char[1000];
							Arrays.fill(cbuf, '\0');
							int len = read.read(cbuf,0,1000);
							StringBuilder sb = new StringBuilder(1000);
							sb.append(cbuf,0,len);						
							System.out.println(sb.toString());

						} catch (Exception e) {			
							System.out.println("exception");
						}	

					}
					Thread.sleep(40000);


				}
				if( client != null)
				{
					client.close();
				}


			} catch (Exception e) {
				e.printStackTrace();
			}


		}


	}



}
