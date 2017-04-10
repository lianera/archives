import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.Scanner;


public class Assembly {
	public static void main(String[] args) throws FileNotFoundException{
		InputStream fis;
		Scanner sc=null;
		if(args.length>=1){
			fis = new FileInputStream(args[0]);
			sc=new Scanner(fis);
		} else {
			sc=new Scanner(System.in);
		}
		String instr;
		for(int i=0;sc.hasNext();i++){
			instr=sc.nextLine();
			
			try {
				Instruction ins=new Instruction(instr);
				System.out.print(ins.toString()+" ");
			} catch (Exception e) {
				System.out.println();
				System.out.println("µÚ"+i+"ÐÐ:"+instr+" ´íÎó:"+e.getMessage());
			}
			
		}
	}
}
