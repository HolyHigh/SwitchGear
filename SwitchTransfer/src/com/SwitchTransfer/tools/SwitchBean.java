package com.SwitchTransfer.tools;

public class SwitchBean {
	
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
	
	
	
	public SwitchBean(String s_name, String d_name) {
		super();
		this.s_name = s_name;
		this.d_name = d_name;
	}

	public SwitchBean(){
		
	}


	@Override
	public String toString() {
		return "SwitchBean [s_name=" + s_name + ", d_name=" + d_name + "]";
	}
	
	


	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((d_name == null) ? 0 : d_name.hashCode());
		result = prime * result + ((s_name == null) ? 0 : s_name.hashCode());
		return result;
	}
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		SwitchBean other = (SwitchBean) obj;
		if (d_name == null) {
			if (other.d_name != null)
				return false;
		} else if (!d_name.equals(other.d_name))
			return false;
		if (s_name == null) {
			if (other.s_name != null)
				return false;
		} else if (!s_name.equals(other.s_name))
			return false;
		return true;
	}




	public String s_name;	//¹ñÃû
	public String d_name;	//ËùÃû
	
	
}




