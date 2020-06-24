
abstract public class Word {
	abstract int getValue();
}


enum OPERS{
	MOV("00000"),
	SHL("00010"),	
	SAL("00010"),	
	SHR("00100"),	
	SAR("00110"),	
	ADD("10000"),	
	SUB("10011"),	
	INC("10101"),	
	DEC("10110"),	
	AND("11000"),	
	OR("11010"),	
	NOT("11100"),	
	XOR("11110"),	
	JMP("00000"),	
	JO("00010"),	
	JS("00100"),	
	JC("00110"),	
	JZ("01000"),
	LOAD("00000"),
	STORE("00000");
    private int val;
    private OPERS(String code){
        this.val=Integer.valueOf(code,2);
    } 
    public int getV(){
        return this.val;
    }
}

class OpWord extends Word{
	OPERS op;
	public OpWord(String str) throws Exception{
		boolean finded=false;
		for(OPERS oper:OPERS.values())
			if(str.equals(oper.name())){
				op=oper;
				finded=true;
				break;
			}
		if(!finded)
			throw new Exception("不能识别的操作符");
	}
	@Override
	public int getValue(){
		return op.getV();
	}
	public OPERS getOp(){
		return op;
	}
}

class Operand extends Word{
	boolean reg;
	public int val;
	public Operand(String name) throws Exception{
		if(name.charAt(0)=='R'){
			val=Integer.parseInt(name.substring(1));
			reg=true;
			if(val>3)
				throw new Exception("不存在这个寄存器");
		}else{
			val=Integer.parseInt(name);
		}
	}
	boolean isReg(){
		return reg;
	}
	@Override
	int getValue() {
		return val;
	}
}
