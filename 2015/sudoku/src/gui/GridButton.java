package gui;

import javax.swing.JButton;

public class GridButton extends JButton{

	/**
	 * 
	 */
	private static final long serialVersionUID = 93884303487728230L;
	public int x,y;
	private boolean changable=true;
	public void setChangable(boolean c){
		if(changable==c)
			return;
		if(c){
			this.setIcon(Resource.imgGridNormal);
			this.setRolloverIcon(Resource.imgGridEnter);
			this.setPressedIcon(Resource.imgGridPress);
		}else{
			this.setIcon(Resource.imgDisabled);
			this.setRolloverIcon(Resource.imgDisabled);
			this.setPressedIcon(Resource.imgDisabled);			
		}
		changable=c;
	}
	public boolean getChangable(){
		return changable;
	}
	public GridButton() {
		changable=true;
		this.setIcon(Resource.imgGridNormal);
		this.setRolloverIcon(Resource.imgGridEnter);
		this.setPressedIcon(Resource.imgGridPress);		
	}
	
}
