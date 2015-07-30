package com.PressureTest.JsonTest;
import java.text.SimpleDateFormat;
import java.util.Date;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

public class JsonTools
{
	
	public JsonTools()
	{
		
	}
	
	public String createJsonZip(String str)
	{
		UpDetails updetails = new UpDetails();
		new PreJsonData(updetails,str);
		
		JSONArray jsonArray = createJsonArray(updetails);
		
		JSONObject	jsonzip = new JSONObject();
		jsonzip.put("TP", updetails.getTP());
		jsonzip.put("CID", updetails.getCmdID());
		jsonzip.put("TM", updetails.getTime());
		jsonzip.put("DID",updetails.getDid());
		jsonzip.put("SID",updetails.getSid());
		jsonzip.put("SGS", jsonArray);
//		JSONObject.fromObject(object)
		
		return	 jsonzip.toString();
			

	}
	
	
	public JSONArray createJsonArray(UpDetails updetails)
	{
		JSONArray arrayObject = new JSONArray();
		arrayObject.add(createArraysObject1(updetails));
		arrayObject.add(createArraysObject2(updetails));
		arrayObject.add(createArraysObject3(updetails));
		
		return	arrayObject;
	}
	
	
	public JSONObject	createArraysObject1(UpDetails updetails)
	{
		JSONObject objectInArray1 = new JSONObject();
		objectInArray1.put("MTP","ZJ-1");
		objectInArray1.put("Ua", updetails.getUa());
		objectInArray1.put("Ub", updetails.getUb());
		objectInArray1.put("Uc", updetails.getUc());
		objectInArray1.put("Ia", updetails.getIa());
		objectInArray1.put("Ib", updetails.getIb());
		objectInArray1.put("Ic", updetails.getIc());
		objectInArray1.put("PFa", updetails.getPFa());
		objectInArray1.put("PFb", updetails.getPFb());
		objectInArray1.put("PFc", updetails.getPFc());
		
		return	objectInArray1;
	}
	
	public JSONObject	createArraysObject2(UpDetails updetails)
	{
		
		JSONObject objectInArray2 = new JSONObject();
		objectInArray2.put("MTP","9300");	
		objectInArray2.put("00", updetails.getS00());
		objectInArray2.put("01", updetails.getS01());
		objectInArray2.put("02", updetails.getS02());
		objectInArray2.put("03", updetails.getS03());
		objectInArray2.put("04", updetails.getS04());
		objectInArray2.put("05", updetails.getS05());
		objectInArray2.put("06", updetails.getS06());
		objectInArray2.put("07", updetails.getS07());
		objectInArray2.put("08", updetails.getS08());
		objectInArray2.put("09", updetails.getS09());
		objectInArray2.put("10", updetails.getS10());
		objectInArray2.put("11", updetails.getS11());
		objectInArray2.put("12", updetails.getS12());
		objectInArray2.put("13", updetails.getS13());
		objectInArray2.put("14", updetails.getS14());
		objectInArray2.put("15", updetails.getS15());
		
		return	objectInArray2;
	}
	
	public JSONObject	createArraysObject3(UpDetails updetails)
	{
		
		JSONObject objectInArray3 = new JSONObject();
		objectInArray3.put("MTP", "1BG");
		objectInArray3.put("SW", updetails.getSW());
		objectInArray3.put("LG", updetails.getLG());
		objectInArray3.put("HG", updetails.getHG());
		objectInArray3.put("PT", updetails.getPT());
		objectInArray3.put("AM", updetails.getAM());
		objectInArray3.put("LV", updetails.getLV());
		objectInArray3.put("HV", updetails.getHV());
		objectInArray3.put("III", updetails.getIII());
		objectInArray3.put("II",  updetails.getII()	);
		objectInArray3.put("I", updetails.getI());
		objectInArray3.put("Ia", updetails.getIa());
		objectInArray3.put("Ib", updetails.getIb());
		objectInArray3.put("Ic", updetails.getIc());
		objectInArray3.put("IHz", updetails.getIHz());
		objectInArray3.put("ILz",  updetails.getILz());
		
		return	objectInArray3;
	}
	
	
	
}