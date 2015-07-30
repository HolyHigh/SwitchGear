package com.SwitchTransfer.tools;

import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;

import net.sf.json.JSONObject;

public class doJsonArm {
	
	public String getCID() {
		return CID;
	}


	public void setCID(String cID) {
		CID = cID;
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

	private String CID ;
	private String s_name;
	private String d_name;



	public boolean analysisJsonbuffer(String sin, Socket socketRet){
		JSONObject jo =null;		
		try {
			jo = JSONObject.fromObject(sin);
		} catch (Exception e) {
			
			return false;
		}		
		
		try {		
			String TP = jo.getString("TP");
			if(TP.equals("Up"))		{
				analysisUpJson(jo, socketRet);
				//post2web
				StringBuffer sb = new StringBuffer(sin);
				sb.append(GlobalValue.ENDTAG);
				if(GlobalValue.pal.add(sb.toString()))	System.out.println(">>> Send to Arraylist okay !\n");
				
				String dir = new String(GlobalValue.MAINPATH+"armdata/"+new SimpleDateFormat("MM-dd").format(new Date()));
				SwitchServerLog.writebuffer(dir, dir+"/arm["+d_name+"]["+s_name+"].log", sin);
			}
			else if(TP.equals("On"))  analysisOnJson(jo, socketRet);
			
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		
		return true;
	}
	
	
	public void analysisUpJson(JSONObject jo ,Socket socketRet ) throws Exception{
		CID = jo.getString("CID");
		s_name = jo.getString("SID");
		d_name = jo.getString("DID");
		SwitchBean sb = new SwitchBean(s_name, d_name);
		if( GlobalValue.shm.findObjectInHashmap(sb) != socketRet ){
			GlobalValue.shm.rmObjectInHashmap(sb);
			GlobalValue.shm.putObject2Hashmap(sb, socketRet);
			System.out.println(">>> Create hashmap finished ");
			//»’÷æ
		}
		
	}
	
	public void analysisOnJson(JSONObject jo, Socket socketRet ) throws Exception{
			CID = jo.getString("CID");
			s_name = jo.getString("SID");
			d_name = jo.getString("DID");
			SwitchBean sb = new SwitchBean(s_name, d_name);
			if( GlobalValue.shm.findObjectInHashmap(sb) != socketRet ){
				GlobalValue.shm.rmObjectInHashmap(sb);
				GlobalValue.shm.putObject2Hashmap(sb, socketRet);
			}
			
	}



}


