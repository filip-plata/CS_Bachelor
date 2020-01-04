package pl.edu.mimuw.forum.undoRedoManager;

import javafx.beans.property.IntegerProperty;
import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;

public class ChangeDataLikes implements UndoRedoOperation {

	private final int likesOldValue;
	private final int likesNewValue;
	private final IntegerProperty integerProperty;
	
	public ChangeDataLikes(int oldValue, int newValue, IntegerProperty integerProperty) {
		this.integerProperty = integerProperty;
		this.likesNewValue = newValue;
		this.likesOldValue = oldValue;
	}
	
	@Override
	public void makeChange(MainPaneController pane) throws ApplicationException {
		integerProperty.set(likesNewValue);
	}

	@Override
	public void revertChange(MainPaneController pane)
			throws ApplicationException {
		integerProperty.set(likesOldValue);
		
	}

	@Override
	public boolean merge(UndoRedoOperation undoRedoOperation) {
		return false;
	}

}
