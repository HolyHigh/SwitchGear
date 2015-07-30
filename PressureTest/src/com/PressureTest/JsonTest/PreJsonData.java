package com.PressureTest.JsonTest;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;



public class PreJsonData
{
	
	public PreJsonData(UpDetails upDetails, String str )
	{

		DecimalFormat	df = new DecimalFormat("#.##");
		 	
		upDetails.setDetails();
		upDetails.setDid("S001");	      //������
		upDetails.setSid(str);  			 //���ع�
		upDetails.setTP("Up");
		upDetails.setUa(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(15))));
		upDetails.setUb(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(15))));
		upDetails.setUc(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(15))));
		upDetails.setIa(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(5))));
		upDetails.setIb(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(5))));
		upDetails.setIc(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(5))));
		upDetails.setPFa(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(2))));
		upDetails.setPFb(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(2))));
		upDetails.setPFc(Float.parseFloat(df.format(new Random().nextFloat()+new Random().nextInt(2))));
		upDetails.setS00(new Random().nextInt(2));
		upDetails.setS01(new Random().nextInt(2));
		upDetails.setS02(new Random().nextInt(2));
		upDetails.setS03(new Random().nextInt(2));
		upDetails.setS04(new Random().nextInt(2));
		upDetails.setS05(new Random().nextInt(2));
		upDetails.setS06(new Random().nextInt(2));
		upDetails.setS07(new Random().nextInt(2));
		upDetails.setS08(new Random().nextInt(2));
		upDetails.setS09(new Random().nextInt(2));
		upDetails.setS10(new Random().nextInt(2));
		upDetails.setS11(new Random().nextInt(2));
		upDetails.setS12(new Random().nextInt(2));
		upDetails.setS13(new Random().nextInt(2));
		upDetails.setS14(new Random().nextInt(2));
		upDetails.setS15(new Random().nextInt(2));
		upDetails.setSW(0);
		upDetails.setLG(0);
		upDetails.setHG(0);
		upDetails.setPT(0);
		upDetails.setAM(0);
		upDetails.setLV(0);
		upDetails.setHV(0);
		upDetails.setIII(0);
		upDetails.setII(0);
		upDetails.setI(0);
		upDetails.setIHz(0.0f);
		upDetails.setILz(0.0f);
		
		
	}



}