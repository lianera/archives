package ai;

import util.SeqList;
import core.Element;

/**
 * 用来研究放置2或4的矩阵
 * @author duan
 *
 */
public class AiPutMatrix extends AiMatrix{

	private int curBlankId;
	private SeqList blanksX;
	private SeqList blanksY;
	private int curNum;
	public AiPutMatrix(AiMatrix am) {
		super(am);
	}

	@Override
	void generateChildren() {
		curBlankId=0;
		blanksX=new SeqList();
		blanksY=new SeqList();
		//把空元素加入顺序表中
		for(int i=0;i<Height;i++){
			for(int j=0;j<Width;j++){
				if(elems[i][j].empty()){
					blanksX.insert(j);
					blanksY.insert(i);
					
				}
			}
		}		
		curBlankId=0;
		curNum=2;
	}
	/**
	 * 获得放置的下一步
	 */
	@Override
	AiMatrix getNextChild() {
		AiMatrix mat=new AiMoveMatrix((AiMatrix)this);
		mat.putElement(new Element(curNum), (int)blanksX.get(curBlankId),(int)blanksY.get(curBlankId));
		if(curNum==2)
			curNum=4;
		else
			curBlankId++;
		return mat;
	}

	@Override
	boolean childrenLeft() {
		return !(curBlankId==blanksX.getSize() && curNum==4);
	}
	
}
