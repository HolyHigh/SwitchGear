package com.SwitchTransfer.tools;

import net.sf.json.JSONObject;

public class doJsonAnswer {

	public doJsonAnswer(){
		
		
	}
	
	public JsonAnswerBean defineSetAnswer(int value, String CID){
		
		JsonAnswerBean jab = new JsonAnswerBean();
		
		switch(value)
		{
		case 0:
			setSuccessAnswer(jab,CID);
			break;
		default:
			setFailedAnswer(jab,CID);
			break;
		}
		
		return jab;
		
	}
	
	public String getJsonAnswer(JsonAnswerBean jab){
		String ret = JSONObject.fromObject(jab).toString();
		
		return ret;
	}
	
	
	private void setSuccessAnswer(JsonAnswerBean jab,String CID){
		jab.setTP("Ans");
		jab.setCID(CID);
		jab.setRES("success");
		jab.setRSN("NULL");
	}
	
	private	void setFailedAnswer(JsonAnswerBean jab,String CID){
		jab.setTP("Ans");
		jab.setCID(CID);
		jab.setRES("Failed");
		jab.setRSN("NULL");
	}
	
	
}
