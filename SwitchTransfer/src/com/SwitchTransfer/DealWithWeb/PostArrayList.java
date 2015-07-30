package com.SwitchTransfer.DealWithWeb;

import java.util.ArrayList;

public class PostArrayList extends ArrayList<String> {
		 
	 /**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public synchronized boolean addPostArrayList(String buffer){
		 return this.add(buffer);
	}
	 
	 public synchronized String getPostArrayList(int ii){
		 return this.get(ii);
	 }
	 
	 public synchronized String rmPostArrayList(int ii){
		return  this.remove(ii);
	 }
	 
	 public synchronized boolean isPostArrayListEmpty(){
		 return this.isEmpty();
	 }
	 
}
