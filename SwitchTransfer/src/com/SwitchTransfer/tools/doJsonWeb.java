package com.SwitchTransfer.tools;

import net.sf.json.JSONObject;

public class doJsonWeb {

	
	public boolean decodeWebJson(String str){
		try {
			JSONObject jo = JSONObject.fromObject(str);
			String tmp = jo.getString("TP");
			if (tmp.equals("Ctrl") || tmp.equals("query")){
				this.s_name = jo.getString("SID");
				this.d_name = jo.getString("DID");
				return true;
			}else{
				return false;
			}
		} catch (Exception e) {
			
			return false;
		}

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


	private String s_name ;//¹ñÃû
	private String d_name;//ËùÃû
}
