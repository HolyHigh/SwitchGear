package com.PressureTest.JsonTest;

import java.text.SimpleDateFormat;
import java.util.Date;

public class UpDetails
{
	
	
	public String getTime()
	{
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//�������ڸ�ʽ
		String time = df.format(new Date());		
		return time;		
	}
	
	public String getCmdID()
	{
		Long value = System.currentTimeMillis();
		String time = value.toString();
		return time;		
	}
	
	
	
	public void setDetails()
	{
		this.CID =	getCmdID();
		this.time = getTime();
	}
	
	





	public float getUa() {
		return Ua;
	}




	public void setUa(float ua) {
		Ua = ua;
	}




	public float getUb() {
		return Ub;
	}




	public void setUb(float ub) {
		Ub = ub;
	}




	public float getUc() {
		return Uc;
	}




	public void setUc(float uc) {
		Uc = uc;
	}




	public float getIa() {
		return Ia;
	}




	public void setIa(float ia) {
		Ia = ia;
	}




	public float getIb() {
		return Ib;
	}




	public void setIb(float ib) {
		Ib = ib;
	}




	public float getIc() {
		return Ic;
	}

	public int getSW() {
		return SW;
	}


	public void setSW(int sW) {
		SW = sW;
	}


	public int getLG() {
		return LG;
	}


	public void setLG(int lG) {
		LG = lG;
	}


	public int getHG() {
		return HG;
	}


	public void setHG(int hG) {
		HG = hG;
	}


	public int getPT() {
		return PT;
	}


	public void setPT(int pT) {
		PT = pT;
	}


	public int getAM() {
		return AM;
	}


	public void setAM(int aM) {
		AM = aM;
	}


	public int getLV() {
		return LV;
	}


	public void setLV(int lV) {
		LV = lV;
	}


	public int getHV() {
		return HV;
	}


	public void setHV(int hV) {
		HV = hV;
	}


	public int getIII() {
		return III;
	}


	public void setIII(int iII) {
		III = iII;
	}


	public int getII() {
		return II;
	}


	public void setII(int iI) {
		II = iI;
	}


	public int getI() {
		return I;
	}


	public void setI(int i) {
		I = i;
	}


	public float getIHz() {
		return IHz;
	}


	public void setIHz(float iHz) {
		IHz = iHz;
	}


	public float getILz() {
		return ILz;
	}


	public void setILz(float iLz) {
		ILz = iLz;
	}




	public void setIc(float ic) {
		Ic = ic;
	}




	public float getPFa() {
		return PFa;
	}




	public void setPFa(float pFa) {
		PFa = pFa;
	}




	public float getPFb() {
		return PFb;
	}




	public void setPFb(float pFb) {
		PFb = pFb;
	}




	public float getPFc() {
		return PFc;
	}




	public void setPFc(float pFc) {
		PFc = pFc;
	}




	public int getS00() {
		return s00;
	}




	public void setS00(int s00) {
		this.s00 = s00;
	}




	public int getS01() {
		return s01;
	}




	public void setS01(int s01) {
		this.s01 = s01;
	}




	public int getS02() {
		return s02;
	}




	public void setS02(int s02) {
		this.s02 = s02;
	}




	public int getS03() {
		return s03;
	}




	public void setS03(int s03) {
		this.s03 = s03;
	}




	public int getS04() {
		return s04;
	}




	public void setS04(int s04) {
		this.s04 = s04;
	}




	public int getS05() {
		return s05;
	}




	public void setS05(int s05) {
		this.s05 = s05;
	}




	public int getS06() {
		return s06;
	}




	public void setS06(int s06) {
		this.s06 = s06;
	}




	public int getS07() {
		return s07;
	}




	public void setS07(int s07) {
		this.s07 = s07;
	}




	public int getS08() {
		return s08;
	}




	public void setS08(int s08) {
		this.s08 = s08;
	}




	public int getS09() {
		return s09;
	}




	public void setS09(int s09) {
		this.s09 = s09;
	}




	public int getS10() {
		return s10;
	}




	public void setS10(int s10) {
		this.s10 = s10;
	}




	public int getS11() {
		return s11;
	}




	public void setS11(int s11) {
		this.s11 = s11;
	}




	public int getS12() {
		return s12;
	}




	public void setS12(int s12) {
		this.s12 = s12;
	}




	public int getS13() {
		return s13;
	}




	public void setS13(int s13) {
		this.s13 = s13;
	}




	public int getS14() {
		return s14;
	}




	public void setS14(int s14) {
		this.s14 = s14;
	}




	public int getS15() {
		return s15;
	}




	public void setS15(int s15) {
		this.s15 = s15;
	}

//	sys


	@Override
	public String toString() {
		return "UpDetails [time=" + time + ", CID=" + CID + ", did=" + did
				+ ", sid=" + sid + ", Ua=" + Ua + ", Ub=" + Ub + ", Uc=" + Uc
				+ ", Ia=" + Ia + ", Ib=" + Ib + ", Ic=" + Ic + ", PFa=" + PFa
				+ ", PFb=" + PFb + ", PFc=" + PFc + ", s00=" + s00 + ", s01="
				+ s01 + ", s02=" + s02 + ", s03=" + s03 + ", s04=" + s04
				+ ", s05=" + s05 + ", s06=" + s06 + ", s07=" + s07 + ", s08="
				+ s08 + ", s09=" + s09 + ", s10=" + s10 + ", s11=" + s11
				+ ", s12=" + s12 + ", s13=" + s13 + ", s14=" + s14 + ", s15="
				+ s15 + "]";
	}



	public UpDetails()
	{
		
	}
	
	
	public String getDid() {
		return did;
	}

	public void setDid(String did) {
		this.did = did;
	}




	public String getSid() {
		return sid;
	}

	public void setSid(String sid) {
		this.sid = sid;
	}

	public String getTP() {
		return TP;
	}


	public void setTP(String tP) {
		TP = tP;
	}


	private	String TP;
	private	String time;
	private	String CID;
	private	String	did;
	private	String	sid;
	private	float	Ua;
	private	float	Ub;
	private	float	Uc;
	private	float 	Ia;
	private	float	Ib;
	private	float	Ic;
	private	float	PFa;
	private	float	PFb;
	private	float	PFc;
	private	int	s00;
	private	int	s01;
	private	int	s02;
	private	int	s03;
	private	int	s04;
	private	int	s05;
	private	int	s06;
	private	int	s07;
	private	int	s08;
	private	int	s09;
	private	int	s10;
	private	int	s11;
	private	int	s12;
	private	int	s13;
	private	int	s14;
	private	int	s15;
	private	int	SW;
	private	int	LG;
	private	int	HG;
	private	int	PT;
	private	int	AM;
	private	int	LV;
	private	int	HV;
	private	int	III;
	private	int	II;
	private	int	I;
	private	float IHz;
	private	float ILz;
//	private SGS SGS;
	
	
}
