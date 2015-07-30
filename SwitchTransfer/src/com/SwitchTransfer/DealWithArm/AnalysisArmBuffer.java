package com.SwitchTransfer.DealWithArm;

import com.SwitchTransfer.tools.GlobalValue;

public class AnalysisArmBuffer {
	
	
	public RetAnalysisBuffer analysisBuffer(String buffer){
		
		RetAnalysisBuffer rab = new RetAnalysisBuffer();
		int value = buffer.indexOf(GlobalValue.ENDTAG);
		if( value == -1){
			rab.setFlags(false);
			rab.setIndex(value);
			return rab;
		}else{
			rab.setFlags(true);
			rab.setIndex(value);
			return rab;
		}
		
		
	}
}
