package pl.edu.mimuw.forum.undoRedoManager;

import javafx.beans.property.StringProperty;
import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.models.SuggestionViewModel;

public class ChangeDataSuggestion extends ChangeDataTextField {
	
	private SuggestionViewModel node;

	public ChangeDataSuggestion(NodeViewModel node, String oldValue,
			String newValue, StringProperty stringProperty) {
		super(node, oldValue, newValue, stringProperty);
		this.node = (SuggestionViewModel) node;
	}
	
	@Override
	public void makeChange(MainPaneController pane) throws ApplicationException {
		node.getResponse().set(newValue);
	}

	@Override
	public void revertChange(MainPaneController pane) throws ApplicationException {
		node.getResponse().set(oldValue);
	}

}
