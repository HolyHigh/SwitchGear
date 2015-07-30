package com.SwitchTransfer.tools;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

public class SwitchServerLog {

	public static boolean writeUnknownException(String buffer){
		try {
			FileOutputStream fo = new FileOutputStream("/home/dfxServer/log/unknownException/unknown.log",true);
			OutputStreamWriter fw = new OutputStreamWriter(fo, "UTF-8");
	        BufferedWriter bufferWritter = new BufferedWriter(fw);
	        StringBuffer sb = new StringBuffer(">>> : "+buffer);
	        sb.append(GlobalValue.ENDTAG);
	        
	        bufferWritter.write(sb.toString());
	        bufferWritter.flush();
	        System.out.println(sb.toString());
	        bufferWritter.close();
	        return true;
		} catch (IOException e) {
			
			e.printStackTrace();
			return false;
		}

	}
	
	public static boolean testwrite(String buffer)
	{
//		public static boolean writeUnknownException(String buffer){
			try {
				FileOutputStream fo = new FileOutputStream("/home/dfxServer/log/unknownException/test.log",true);
				OutputStreamWriter fw = new OutputStreamWriter(fo, "UTF-8");
		        BufferedWriter bufferWritter = new BufferedWriter(fw);
		        StringBuffer sb = new StringBuffer(">>> : "+buffer);
		        sb.append(GlobalValue.ENDTAG);
		        
		        bufferWritter.write(sb.toString());
		        bufferWritter.flush();
		        System.out.println(sb.toString());
		        bufferWritter.close();
		        return true;
			} catch (IOException e) {
				
				e.printStackTrace();
				return false;
			}

		
	}
	
	
	public static boolean writebuffer(String dir, String name, String buffer){
		File outfile = new File(dir);                 
		if(!outfile.isFile()){
			outfile.mkdir();
			System.out.println(">>>  Create direct success ");
		}

		try {
			FileOutputStream fo = new FileOutputStream(name,true);
			OutputStreamWriter fw = new OutputStreamWriter(fo, "UTF-8");
	        BufferedWriter bufferWritter = new BufferedWriter(fw);
	        StringBuffer sb = new StringBuffer(">>> : "+buffer);
	        sb.append(GlobalValue.ENDTAG);
	        sb.append(GlobalValue.ENDTAG);
	        
	        bufferWritter.write(sb.toString());
	        bufferWritter.flush();
	        System.out.println(sb.toString());
	        bufferWritter.close();
	        return true;
		} catch (IOException e) {
			
			e.printStackTrace();
			return false;
		}

	}
	
	

}
