package ai;

import core.Direction;
import core.Matrix;

/**
 * 此类是AI用来研究移动的矩阵
 * @author duan
 *
 */
public class AiMoveMatrix extends AiMatrix{
	private static final Direction[] dir={Direction.LEFT,Direction.UP,Direction.RIGHT,Direction.DOWN};
	private int curDir=0;
	public AiMoveMatrix(Matrix am,int score,int step) {
		super(am,score,step);
	}
	public AiMoveMatrix(AiMatrix am){
		super(am);
	}
	@Override
	void generateChildren() {
		curDir=0;
	}

	@Override
	AiMatrix getNextChild() {
		AiMatrix mat=new AiPutMatrix(this);
		//if(curDir>dir.length)
		int r=mat.move(dir[curDir]);
		curDir++;
		if(r>=0){
			/**
			 * 总分
			 */
			//mat.score+=r;
			/**
			 * 本次得分
			 */
			mat.score=r;
			mat.totalStep++;
			return mat;
		}
		return null;
	}

	@Override
	boolean childrenLeft() {
		return curDir<dir.length;
	}
	public Direction getCurrentDirection(){
		return dir[curDir-1];
	}
}
