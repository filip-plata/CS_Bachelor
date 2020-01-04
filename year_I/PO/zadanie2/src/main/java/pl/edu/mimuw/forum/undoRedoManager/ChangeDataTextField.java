package pl.edu.mimuw.forum.undoRedoManager;

import javafx.beans.property.StringProperty;
import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;

public class ChangeDataTextField implements UndoRedoOperation {
	
	protected String oldValue;
	protected String newValue;
	protected final NodeViewModel node;
	protected final StringProperty stringProperty;
	
	public ChangeDataTextField(NodeViewModel node, String oldValue, String newValue, StringProperty stringProperty) {
		this.node = node;
		this.oldValue = oldValue;
		this.newValue = newValue;
		this.stringProperty = stringProperty;
	}
	
	protected boolean wasLetterAdded() {
		return newValue.length() > oldValue.length();
	}
	
	@Override
	public void makeChange(MainPaneController pane) throws ApplicationException {
		stringProperty.set(newValue);
	}

	@Override
	public void revertChange(MainPaneController pane) throws ApplicationException {
		stringProperty.set(oldValue);
	}


	@Override
	public boolean merge(UndoRedoOperation undoRedoOperation) {
		if (undoRedoOperation instanceof ChangeDataTextField) {
			ChangeDataTextField change = (ChangeDataTextField) undoRedoOperation;
			if (this.node == change.node && this.stringProperty == change.stringProperty) {
				if (this.wasLetterAdded() && change.wasLetterAdded() && (this.newValue.charAt(this.newValue.length() - 1) == change.newValue.charAt(change.newValue.length() - 1)))  {
					this.oldValue = change.oldValue;
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		return false;
	}
}
