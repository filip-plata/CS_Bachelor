package pl.edu.mimuw.forum.undoRedoManager;

import javafx.beans.property.StringProperty;
import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;

public class ChangeDataAuthor extends ChangeDataTextField {

	public ChangeDataAuthor(NodeViewModel node, String oldValue,
			String newValue, StringProperty stringProperty) {
		super(node, oldValue, newValue, stringProperty);
	}
	
	@Override
	public void makeChange(MainPaneController pane) throws ApplicationException {
		node.getAuthor().set(newValue);
	}

	@Override
	public void revertChange(MainPaneController pane) throws ApplicationException {
		node.getAuthor().set(oldValue);
	}

}
