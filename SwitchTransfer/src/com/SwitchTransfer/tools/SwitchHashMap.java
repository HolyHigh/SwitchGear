package com.SwitchTransfer.tools;
import java.net.Socket;
//import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;

public class SwitchHashMap extends ConcurrentHashMap<SwitchBean, Socket>{

//	public static ConcurrentHashMap<SwitchBean, Socket> hm = new ConcurrentHashMap<>();
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;


	public  void putObject2Hashmap(SwitchBean sb, Socket socket){
		this.put(sb, socket);
	}
	
	public  Socket findObjectInHashmap(SwitchBean sb){
		Socket socket = this.get(sb);
		return socket;
	}
	
	public  void rmObjectInHashmap(SwitchBean sb	){
		this.remove(sb);
	}
	
}
