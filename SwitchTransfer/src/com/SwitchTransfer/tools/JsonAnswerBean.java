package com.SwitchTransfer.tools;

public class JsonAnswerBean {

	public String getTP() {
		return TP;
	}
	public void setTP(String tP) {
		TP = tP;
	}
	
	public String getCID() {
		return CID;
	}
	public void setCID(String cID) {
		CID = cID;
	}
	
	public String getRES() {
		return RES;
	}
	public void setRES(String rES) {
		RES = rES;
	}
	
	public String getRSN() {
		return RSN;
	}
	public void setRSN(String rSN) {
		RSN = rSN;
	}
	
	
	@Override
	public String toString() {
		return "JsonBean [TP=" + TP + ", CID=" + CID + ", RES=" + RES
				+ ", RSN=" + RSN + "]";
	}

	

	private	String TP;
	private	String CID;
	private	String RES;
	private	String RSN;
	
}
