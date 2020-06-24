enum FORMAT{
	TWOREG,REGCON,WMEM,RMEM,JUMP
}
public class Instruction {
	String codeStr;
	int code;
	OpWord opw;
	Operand[] oprs;
	FORMAT format;
	public Instruction(String ins) throws Exception{
		String[] s=ins.split(" |,");
		if(s==null)
			throw new Exception("语法错误");
		opw=new OpWord(s[0]);
		oprs=new Operand[s.length-1];
		for(int i=1;i<s.length;i++){
			oprs[i-1]=new Operand(s[i]);
		}
		format=getFormat();
		code=getCode();
	}
	
	public int toCode(){
		return code;
	}
	private int getCode() throws Exception{
		int opwcode=0;
		int fscode=0;
		int oprcode=0;
		switch(format){
		case TWOREG:{
			opwcode=0x0;
			fscode=opw.getValue();
			if(opw.getOp().equals(OPERS.NOT)||opw.getOp().equals(OPERS.INC)||opw.getOp().equals(OPERS.DEC)){
				oprcode=oprs[0].getValue()<<6 | oprs[1].getValue()<<4;
			}else if(opw.getOp().equals(OPERS.MOV))
				oprcode=oprs[0].getValue()<<6 |oprs[1].getValue()<<2;
			else 
				oprcode=oprs[0].getValue()<<6 | oprs[1].getValue()<<4 | oprs[2].getValue()<<2;
		}
			break;
		case REGCON:{
			opwcode=0x1;
			fscode=opw.getValue();
			if(opw.getOp().equals(OPERS.NOT)||opw.getOp().equals(OPERS.MOV)||opw.getOp().equals(OPERS.INC)||opw.getOp().equals(OPERS.DEC))
				oprcode=oprs[0].getValue()<<6 | (oprs[1].getValue()&0xf);
			else 
				oprcode=oprs[0].getValue()<<6 | oprs[1].getValue()<<4 | oprs[2].getValue();
		}
			break;
		case WMEM:
			opwcode=0x2;
			oprcode=oprs[0].getValue()<<4 | (oprs[1].getValue()&0xf)<<2;
			break;
		case RMEM:
			opwcode=0x3;
			oprcode=oprs[0].getValue()<<6 | (oprs[1].getValue()&0xf)<<4;
			break;
		case JUMP:
			opwcode=0x4;
			fscode=opw.getValue();
			oprcode=oprs[0].getValue()&0xff;
			break;
		default:
			break;
			
		}
		return (opwcode & 0x7)<<13|(fscode & 0x1f)<<8|(oprcode & 0xff);
	}
	private int getAddrNum(){
		return oprs.length;
	}
	private FORMAT getFormat() throws Exception{
		if(getAddrNum()==3){
			if(oprs[0].isReg() && oprs[1].isReg() && oprs[2].isReg())
				return FORMAT.TWOREG;
			else if(oprs[0].isReg() && oprs[1].isReg() && !oprs[2].isReg())
				return FORMAT.REGCON;
			else 
				throw new Exception("错误的格式");
		}else if(getAddrNum()==2){	//LOAD、STORE、NOT、MOV
			if(oprs[0].isReg() && oprs[1].isReg()){
				if(opw.getOp().equals(OPERS.LOAD))
					return FORMAT.RMEM;
				else if(opw.getOp().equals(OPERS.STORE))
					return FORMAT.WMEM;
				else if(opw.getOp().equals(OPERS.NOT)||opw.getOp().equals(OPERS.MOV)||
						opw.getOp().equals(OPERS.INC)||opw.getOp().equals(OPERS.DEC))
					return FORMAT.TWOREG;
				else 
					throw new Exception("错误的格式");
			}else if(oprs[0].isReg() && !oprs[1].isReg()){
				if(opw.getOp().equals(OPERS.NOT)||opw.getOp().equals(OPERS.MOV)||
						opw.getOp().equals(OPERS.INC)||opw.getOp().equals(OPERS.DEC))
					return FORMAT.REGCON;
				else 
					throw new Exception("错误的格式");
			}
		}else if(getAddrNum()==1){//移位、跳转
			if((	opw.getOp().equals(OPERS.SAL)||
					opw.getOp().equals(OPERS.SAR)||
					opw.getOp().equals(OPERS.SHL)||
					opw.getOp().equals(OPERS.SHR))&&
					oprs[0].isReg()){
				return FORMAT.TWOREG;
			}else if((opw.getOp().equals(OPERS.JMP)||
					opw.getOp().equals(OPERS.JC)||
					opw.getOp().equals(OPERS.JO)||
					opw.getOp().equals(OPERS.JS)||
					opw.getOp().equals(OPERS.JZ))&&
					!oprs[0].isReg()){
				return FORMAT.JUMP;
			}
		
		}
		throw new Exception("错误的格式");
	}
	
	public String toString(){
		
		//String s= Integer.toHexString(code);
		String s=String.format("%04x",code);
		return s;
	//	return Integer.toBinaryString(code);
	}
}
