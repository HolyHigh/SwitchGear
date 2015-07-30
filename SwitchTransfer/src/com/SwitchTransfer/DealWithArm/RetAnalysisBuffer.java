package com.SwitchTransfer.DealWithArm;

public class RetAnalysisBuffer {

	public RetAnalysisBuffer(){
		
	}
	
	public RetAnalysisBuffer(int index, boolean flags) {
		super();
		this.index = index;
		this.flags = flags;
	}
	public int getIndex() {
		return index;
	}
	public void setIndex(int index) {
		this.index = index;
	}
	public boolean isFlags() {
		return flags;
	}
	public void setFlags(boolean flags) {
		this.flags = flags;
	}
	@Override
	public String toString() {
		return "RetAnalysisBuffer [index=" + index + ", flags=" + flags + "]";
	}
	private int index;
	private boolean flags;
	
	
}
