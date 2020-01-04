package pl.edu.mimuw.forum.undoRedoManager;

import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.controllers.MainPaneController;
import pl.edu.mimuw.forum.ui.models.SuggestionViewModel;

public class ChangeDataAccepted implements UndoRedoOperation {
	
	private SuggestionViewModel model;
	private boolean newValue;
	
	public ChangeDataAccepted(SuggestionViewModel model, boolean newValue) {
		this.model = model;
		this.newValue = newValue;
	}

	@Override
	public void makeChange(MainPaneController pane) throws ApplicationException {
		model.getIsResponseAccepted().set(newValue);
	}

	@Override
	public void revertChange(MainPaneController pane)
			throws ApplicationException {
		model.getIsResponseAccepted().set(!newValue);
		
	}

	@Override
	public boolean merge(UndoRedoOperation undoRedoOperation) {
		return false;
	}

}
